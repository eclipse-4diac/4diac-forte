/*******************************************************************************
 * Copyright (c) 2005, 2025 Profactor GmbH, ACIN, fortiss GmbH,
 *                          Johannes Kepler University,
 *                          Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Gunnar Grabmaier, Alois Zoitl, Smodic Rene, Ingo Hegny,
 *     Gerhard Ebenhofer, Michael Hofmann, Martin Melik Merkumians, Monika Wenger,
 *     Matthias Plasch
 *                - initial implementation and rework communication infrastructure
 *   Alois Zoitl  - introduced new CGenFB class for better handling generic FBs
 *   Martin Jobst - add CTF tracing integration
 *                - account for data type size in FB initialization
 *                - handle in-out connections with variable arrays
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include "forte/funcbloc.h"

#include "forte/dataconn.h"
#include "forte/datatype.h"
#include "forte/eventconn.h"
#include "forte/adapter.h"

#include <stdlib.h>
#include <string.h>
#include "forte/timerha.h"
#include "forte/device.h"
#include "forte/datatypes/forte_array.h"
#include "forte/util/string_utils.h"

#include "resource_internal.h"

using namespace forte;
using namespace std::string_literals;

CFunctionBlock::CFunctionBlock(CFBContainer &paContainer,
                               const SFBInterfaceSpec &paInterfaceSpec,
                               StringId paInstanceNameId) :
    CFBContainer(paInstanceNameId, paContainer),
    mInterfaceSpec(paInterfaceSpec),
    mFBState(E_FBStates::Idle), // put the FB in the idle state to avoid a useless reset after creation
    mConnRefCount(0),
    mDeletable(true) {
}

bool CFunctionBlock::initialize() {
  if (!CFBContainer::initialize()) {
    return false;
  }
  setupEventMonitoringData();
  setupInputConnectionTrackingData();
  return true;
}

CFunctionBlock::~CFunctionBlock() {
  freeEventMonitoringData();
}

void CFunctionBlock::deinitialize() {
  CFBContainer::deinitialize();

  // disconnect all event connections
  for (TPortId eoId = 0; eoId < getFBInterfaceSpec().getNumEOs(); eoId++) {
    CEventConnection *eoConn = getEOConUnchecked(eoId);
    for (auto connPoint : eoConn->getDestinationList()) {
      if ((connPoint.getPortId() & cgInternal2InterfaceMarker) == 0) {
        auto &dstFb = connPoint.getFB();
        eoConn->disconnect(dstFb, std::array{dstFb.getFBInterfaceSpec().mEINames[connPoint.getPortId()]});
      }
    }
  }

  // disconnect all data input connections
  for (TPortId diId = 0; diId < getFBInterfaceSpec().getNumDIs(); diId++) {
    CDataConnection *diConn = *getDIConUnchecked(diId);
    if (diConn != nullptr) {
      diConn->disconnect(*this, std::array{getFBInterfaceSpec().mDINames[diId]});
      if (diConn->isDelegating()) {
        delete diConn;
      }
    }
  }

  // disconnect all adapter input connections
  for (TPortId aiId = 0; aiId < getFBInterfaceSpec().mSocketNames.size(); aiId++) {
    ISocketPin *skt = getSocketPinUnchecked(aiId);
    if (skt->getAdapterCon() != nullptr) {
      skt->getAdapterCon()->disconnect(*this, std::array{getFBInterfaceSpec().mSocketNames[aiId]});
    }
  }

  // disconnect all dio input connections
  for (TPortId dioId = 0; dioId < getFBInterfaceSpec().getNumDIOs(); dioId++) {
    CDataConnection *dioConn = *getDIOInConUnchecked(dioId);
    if (dioConn != nullptr) {
      dioConn->disconnect(*this, std::array{getFBInterfaceSpec().mDIONames[dioId]});
    }
  }
}

CTimerHandler &CFunctionBlock::getTimer() {
  return getDevice()->getTimer();
}

CEventConnection *CFunctionBlock::getEOConnection(StringId paEONameId) {
  CEventConnection *retVal = nullptr;
  TPortId portId = getFBInterfaceSpec().getEOID(paEONameId);
  if (cgInvalidPortId != portId) {
    retVal = getEOConUnchecked(portId);
  }
  return retVal;
}

bool CFunctionBlock::connectDI(TPortId paDIPortId, CDataConnection *paDataCon) {
  if (paDIPortId >= getFBInterfaceSpec().getNumDIs()) { // catch invalid ID
    return false;
  }

  CDataConnection **conn = getDIConUnchecked(paDIPortId);
  if (!paDataCon) { // disconnect?
    *conn = nullptr;
    return true;
  }
  if (*conn && paDataCon != *conn) { // already connected to different connection?
    DEVLOG_ERROR("%s cannot connect input data %s to more sources, using the latest connection attempt\n",
                 getInstanceName(), getFBInterfaceSpec().mDINames[paDIPortId].data());
    return false;
  }
  *conn = paDataCon;
  return configureGenericDI(paDIPortId, paDataCon->getValue());
}

bool CFunctionBlock::configureGenericDI(TPortId paDIPortId, const CIEC_ANY &paRefValue) {
  CIEC_ANY *di = getDI(paDIPortId);
  if (di->getDataTypeID() == CIEC_ANY::e_ANY) {
    di->setValue(paRefValue.unwrap());
  }
  return true;
}

bool CFunctionBlock::connectDIO(TPortId paDIOPortId, CInOutDataConnection *paDataCon) {
  if (paDIOPortId >= getFBInterfaceSpec().getNumDIOs()) { // catch invalid ID
    return false;
  }

  CInOutDataConnection **conn = getDIOInConUnchecked(paDIOPortId);
  if (!paDataCon) { // disconnect?
    *conn = nullptr;
    return true;
  }
  if (*conn && paDataCon != *conn) { // already connected to different connection?
    DEVLOG_ERROR("%s cannot connect InOut data %s to more sources, using the latest connection attempt\n",
                 getInstanceName(), getFBInterfaceSpec().mDIONames[paDIOPortId].data());
    return false;
  }
  *conn = paDataCon;
  getDIOOutConUnchecked(paDIOPortId)->setValue(&paDataCon->getValue());
  return configureGenericDIO(paDIOPortId, paDataCon->getValue());
}

bool CFunctionBlock::configureGenericDIO(TPortId paDIOPortId, const CIEC_ANY &paRefValue) {
  CIEC_ANY *dio = getDIO(paDIOPortId);
  switch (dio->getDataTypeID()) {
    case CIEC_ANY::e_ANY: dio->setValue(paRefValue.unwrap()); break;
    case CIEC_ANY::e_ARRAY: {
      auto &dioArrayValue = static_cast<CIEC_ARRAY &>(*dio);
      auto &refArrayValue = static_cast<const CIEC_ARRAY &>(paRefValue.unwrap());
      if (dioArrayValue.hasVariableBounds() && !refArrayValue.hasVariableBounds()) {
        dioArrayValue.setBounds(refArrayValue);
      }
      break;
    }
    default: break;
  }
  return true;
}

CDataConnection *CFunctionBlock::getDIConnection(StringId paDINameId) {
  TPortId diPortID = getFBInterfaceSpec().getDIID(paDINameId);
  if (diPortID != cgInvalidPortId) {
    return *getDIConUnchecked(diPortID);
  }
  return nullptr;
}

CDataConnection *CFunctionBlock::getDOConnection(StringId paDONameId) {
  TPortId doPortID = getFBInterfaceSpec().getDOID(paDONameId);
  if (doPortID != cgInvalidPortId) {
    return getDOConUnchecked(doPortID);
  }
  return nullptr;
}

CInOutDataConnection *CFunctionBlock::getDIOInConnection(StringId paDIONameId) {
  TPortId doPortID = getFBInterfaceSpec().getDIOID(paDIONameId);
  if (doPortID != cgInvalidPortId) {
    return *getDIOInConUnchecked(doPortID);
  }
  return nullptr;
}

CInOutDataConnection *CFunctionBlock::getDIOOutConnection(StringId paDIONameId) {
  TPortId doPortID = getFBInterfaceSpec().getDIOID(paDIONameId);
  if (doPortID != cgInvalidPortId) {
    return getDIOOutConUnchecked(doPortID);
  }
  return nullptr;
}

bool CFunctionBlock::configureGenericDO(TPortId paDOPortId, const CIEC_ANY &paRefValue) {
  if (paDOPortId >= getFBInterfaceSpec().getNumDOs()) {
    return false;
  }
  CIEC_ANY *dataOutput = getDO(paDOPortId);
  if (dataOutput->getDataTypeID() == CIEC_ANY::e_ANY) {
    dataOutput->setValue(paRefValue.unwrap());
  }
  return true;
}

CIEC_ANY *CFunctionBlock::getDataInput(const StringId paDINameId) {
  if (const TPortId portId = getFBInterfaceSpec().getDIID(paDINameId); portId != cgInvalidPortId) {
    return getDI(portId);
  }
  return nullptr;
}

CIEC_ANY *CFunctionBlock::getDataOutput(const StringId paDONameId) {
  if (const TPortId portId = getFBInterfaceSpec().getDOID(paDONameId); portId != cgInvalidPortId) {
    return getDO(portId);
  }
  return nullptr;
}

CIEC_ANY *CFunctionBlock::getDataInOut(const StringId paDIONameId) {
  if (const TPortId portId = getFBInterfaceSpec().getDIOID(paDIONameId); portId != cgInvalidPortId) {
    return getDIO(portId);
  }
  return nullptr;
}

CIEC_ANY *CFunctionBlock::getDIFromPortId(TPortId paDIPortId) {
  if (paDIPortId < getFBInterfaceSpec().getNumDIs()) {
    return getDI(paDIPortId);
  }
  return nullptr;
}

CIEC_ANY *CFunctionBlock::getDOFromPortId(TPortId paDOPortId) {
  if (paDOPortId < getFBInterfaceSpec().getNumDOs()) {
    return getDO(paDOPortId);
  }
  return nullptr;
}

CIEC_ANY *CFunctionBlock::getDIOFromPortId(TPortId paDIPortId) {
  if (paDIPortId < getFBInterfaceSpec().getNumDIOs()) {
    return getDIO(paDIPortId);
  }
  return nullptr;
}

CIEC_ANY *CFunctionBlock::getVar(const std::span<const StringId> paNameList) {
  if (paNameList.empty()) {
    return nullptr;
  }
  const StringId name = paNameList.front();
  if (const auto var = getDataInput(name)) {
    return var->getVar(paNameList.subspan(1));
  };
  if (const auto var = getDataOutput(name)) {
    return var->getVar(paNameList.subspan(1));
  };
  if (const auto var = getDataInOut(name)) {
    return var->getVar(paNameList.subspan(1));
  };
  return CFBContainer::getVar(paNameList);
}

IPlugPin *CFunctionBlock::getPlugPin(StringId paPlugNameId) {
  TPortId plugPortId = getFBInterfaceSpec().getPlugID(paPlugNameId);
  if (plugPortId != cgInvalidPortId) {
    return getPlugPinUnchecked(plugPortId);
  }
  return nullptr;
}

ISocketPin *CFunctionBlock::getSocketPin(StringId paSocketNameId) {
  TPortId sktPortId = getFBInterfaceSpec().getSocketID(paSocketNameId);
  if (sktPortId != cgInvalidPortId) {
    return getSocketPinUnchecked(sktPortId);
  }
  return nullptr;
}

void CFunctionBlock::sendAdapterEvent(CAdapter &paAdapter, TEventID paEID, CEventChainExecutionThread *const paECET) {
  paAdapter.receiveInputEvent(paEID, paECET);
}

bool CFunctionBlock::configureFB(const char *) {
  return true;
}

void CFunctionBlock::setInitialValues() {
  // currently nothing to do here
}

EMGMResponse CFunctionBlock::changeExecutionState(EMGMCommandType paCommand) {
  EMGMResponse nRetVal = EMGMResponse::InvalidState;
  switch (paCommand) {
    case EMGMCommandType::Start:
      if ((E_FBStates::Idle == mFBState) || (E_FBStates::Stopped == mFBState)) {
        mFBState = E_FBStates::Running;
        nRetVal = EMGMResponse::Ready;
      }
      break;
    case EMGMCommandType::Stop:
      if (E_FBStates::Running == mFBState) {
        mFBState = E_FBStates::Stopped;
        nRetVal = EMGMResponse::Ready;
      }
      break;
    case EMGMCommandType::Kill:
      if (E_FBStates::Running == mFBState) {
        mFBState = E_FBStates::Killed;
        nRetVal = EMGMResponse::Ready;
      }
      break;
    case EMGMCommandType::Reset:
      if ((E_FBStates::Stopped == mFBState) || (E_FBStates::Killed == mFBState)) {
        mFBState = E_FBStates::Idle;
        nRetVal = EMGMResponse::Ready;
        setInitialValues();
      }
      break;
    default: nRetVal = EMGMResponse::InvalidOperation; break;
  }

  if (EMGMResponse::Ready == nRetVal) {
    nRetVal = CFBContainer::changeExecutionState(paCommand);
  }

  return nRetVal;
}

CConnection *CFunctionBlock::getInputConnection(const std::span<const StringId> paDstNameList) {
  if (paDstNameList.empty()) {
    return nullptr;
  }
  StringId name = paDstNameList.front();
  if (const auto conn = getDIConnection(name); conn) {
    return conn->getMemberConnection(paDstNameList.subspan(1));
  };
  if (const auto conn = getDIOInConnection(name); conn) {
    if (paDstNameList.size() == 1) {
      return conn;
    }
  };
  if (const auto skt = getSocketPin(name); skt) {
    if (paDstNameList.size() == 1) {
      return skt->getAdapterCon();
    }
  };
  return CFBContainer::getInputConnection(paDstNameList);
}

CConnection::Wrapper CFunctionBlock::getOutputConnection(const std::span<const StringId> paSrcNameList) {
  if (paSrcNameList.empty()) {
    return {};
  }
  StringId name = paSrcNameList.front();
  if (const auto conn = getEOConnection(name); conn) {
    return conn->getDelegatingConnection(paSrcNameList.subspan(1));
  };
  if (const auto conn = getDOConnection(name); conn) {
    return conn->getDelegatingConnection(paSrcNameList.subspan(1));
  };
  if (const auto conn = getDIOOutConnection(name); conn) {
    return conn->getDelegatingConnection(paSrcNameList.subspan(1));
  };
  if (const auto plug = getPlugPin(name); plug) {
    return plug->getAdapterCon().getDelegatingConnection(paSrcNameList.subspan(1));
  };
  return CFBContainer::getOutputConnection(paSrcNameList);
}

//********************************** below here are monitoring specific functions
//**********************************************************
void CFunctionBlock::setupEventMonitoringData() {
  freeEventMonitoringData();
  mEventMonitorCount.resize(getFBInterfaceSpec().getNumEIs() + getFBInterfaceSpec().getNumEOs());
}

void CFunctionBlock::freeEventMonitoringData() {
  mEventMonitorCount.clear();
}

TForteUInt32 &CFunctionBlock::getEIMonitorData(TEventID paEIID) {
  return mEventMonitorCount[paEIID];
}

TForteUInt32 &CFunctionBlock::getEOMonitorData(TEventID paEOID) {
  return mEventMonitorCount[getFBInterfaceSpec().getNumEIs() + paEOID];
}

TAbsDataPortNum CFunctionBlock::getAbsDataPortNum(StringId paPortNameId) const {
  TPortId paPortId = getFBInterfaceSpec().getDIID(paPortNameId);
  if (paPortId != cgInvalidPortId) {
    return paPortId;
  }

  auto &interfaceSpec = getFBInterfaceSpec();
  paPortId = getFBInterfaceSpec().getDOID(paPortNameId);
  if (paPortId != cgInvalidPortId) {
    return interfaceSpec.getNumDIs() + paPortId;
  }

  paPortId = getFBInterfaceSpec().getDIOID(paPortNameId);
  if (paPortId != cgInvalidPortId) {
    return interfaceSpec.getNumDIs() + interfaceSpec.getNumDOs() + paPortId;
  }

  return INVALID_ABS_DATA_PORT_ID;
}

bool CFunctionBlock::setForced(const std::span<const StringId> paNameList, const bool paForce) {
  if (paNameList.empty()) {
    return false;
  }
  const StringId name = paNameList.front();
  if (const auto absDataPortNum = getAbsDataPortNum(name);
      absDataPortNum != INVALID_ABS_DATA_PORT_ID && paNameList.size() == 1) {
    setForced(absDataPortNum, paForce);
    return true;
  }
  return CFBContainer::setForced(paNameList, paForce);
}

bool CFunctionBlock::setForced(TAbsDataPortNum paAbsDataPortNum, bool paForceValue) {
  if (paAbsDataPortNum >= mForces.size()) {
    return false;
  }

  mForces[paAbsDataPortNum] = paForceValue;

  if (paForceValue) {
    auto &interfaceSpec = getFBInterfaceSpec();

    if ((interfaceSpec.getNumDIs() <= paAbsDataPortNum) &&
        (paAbsDataPortNum < interfaceSpec.getNumDIs() + interfaceSpec.getNumDOs())) {
      // it is a data output mirror the forced value there
      TPortId doPortId = paAbsDataPortNum - interfaceSpec.getNumDIs();
      CDataConnection *con = getDOConUnchecked(doPortId);
      con->writeData(*getDO(doPortId));
    }
  }
  return true;
}

void CFunctionBlock::resetForcedOutputs() {
  const std::size_t numDIs = getFBInterfaceSpec().getNumDIs();
  const std::size_t numDOs = getFBInterfaceSpec().getNumDOs();
  for (TPortId index = 0; index < numDOs; ++index) {
    if (isForced(numDIs + index)) {
      // when forcing we write back the value from the connection to keep the forced value on the output
      getDOConUnchecked(index)->readData(*getDO(index));
    }
  }
}

void CFunctionBlock::toString(std::string &paTargetBuf) const {
  paTargetBuf += '(';

  for (size_t i = 0; i < getFBInterfaceSpec().getNumDIs(); ++i) {
    util::writeToStringNameValuePair(paTargetBuf, getFBInterfaceSpec().mDINames[i], getDI(i));
    paTargetBuf += csmToStringSeparator;
  }

  for (size_t i = 0; i < getFBInterfaceSpec().getNumDOs(); ++i) {
    util::writeToStringNameValuePair(paTargetBuf, getFBInterfaceSpec().mDONames[i], getDO(i));
    paTargetBuf += csmToStringSeparator;
  }

  for (size_t i = 0; i < getFBInterfaceSpec().getNumDIOs(); ++i) {
    util::writeToStringNameValuePair(paTargetBuf, getFBInterfaceSpec().mDIONames[i], getDIO(i));
    paTargetBuf += csmToStringSeparator;
  }

  if (paTargetBuf.back() != '(') {
    // if we have more  then just the opening '(' was added remove the last to string separator
    paTargetBuf.resize(paTargetBuf.size() - 2);
  }

  paTargetBuf += ')';
}

//********************************** below here are CTF Tracing specific functions
//**********************************************************
#ifdef FORTE_TRACE_CTF
void CFunctionBlock::traceInputEvent(TEventID paEIID) {
  if (auto &tracer = getResource()->getInternal().getTracer(); tracer.isEnabled()) {
    tracer.traceReceiveInputEvent(getFBTypeName(), getFullQualifiedApplicationInstanceName('.').c_str(),
                                  static_cast<uint64_t>(paEIID));
    traceInstanceData();
  }
}

void CFunctionBlock::traceReadData(TPortId paDINum, CIEC_ANY &paValue) {
  if (auto &tracer = getResource()->getInternal().getTracer(); tracer.isEnabled()) {

    std::string valueString;
    paValue.toString(valueString);
    tracer.traceInputData(getFBTypeName(), getFullQualifiedApplicationInstanceName('.').c_str(),
                          static_cast<uint64_t>(paDINum), valueString.c_str());
  }
}

void CFunctionBlock::traceWriteData(TPortId paDONum, CIEC_ANY &paValue) {
  if (auto &tracer = getResource()->getInternal().getTracer(); tracer.isEnabled()) {

    std::string valueString;
    paValue.toString(valueString);
    tracer.traceOutputData(getFBTypeName(), getFullQualifiedApplicationInstanceName('.').c_str(),
                           static_cast<uint64_t>(paDONum), valueString.c_str());
  }
}

#endif
