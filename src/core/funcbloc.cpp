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
#include "core/funcbloc.h"
#include "core/dataconn.h"
#include "arch/datatype.h"
#include "core/eventconn.h"
#include "core/adapter.h"

#include <stdlib.h>
#include <string.h>
#include "arch/timerha.h"
#include "core/device.h"
#include "core/util/string_utils.h"

using namespace std::string_literals;

CFunctionBlock::CFunctionBlock(forte::core::CFBContainer &paContainer,
                               const SFBInterfaceSpec &paInterfaceSpec,
                               forte::core::StringId paInstanceNameId) :
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
      eoConn->disconnect(connPoint.getFB(), std::array{connPoint.getPortId()});
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
    forte::ISocketPin *skt = getSocketPinUnchecked(aiId);
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

CEventConnection *CFunctionBlock::getEOConnection(forte::core::StringId paEONameId) {
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

CDataConnection *CFunctionBlock::getDIConnection(forte::core::StringId paDINameId) {
  TPortId diPortID = getFBInterfaceSpec().getDIID(paDINameId);
  if (diPortID != cgInvalidPortId) {
    return *getDIConUnchecked(diPortID);
  }
  return nullptr;
}

CDataConnection *CFunctionBlock::getDOConnection(forte::core::StringId paDONameId) {
  TPortId doPortID = getFBInterfaceSpec().getDOID(paDONameId);
  if (doPortID != cgInvalidPortId) {
    return getDOConUnchecked(doPortID);
  }
  return nullptr;
}

CInOutDataConnection *CFunctionBlock::getDIOInConnection(forte::core::StringId paDIONameId) {
  TPortId doPortID = getFBInterfaceSpec().getDIOID(paDIONameId);
  if (doPortID != cgInvalidPortId) {
    return *getDIOInConUnchecked(doPortID);
  }
  return nullptr;
}

CInOutDataConnection *CFunctionBlock::getDIOOutConnection(forte::core::StringId paDIONameId) {
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

CIEC_ANY *CFunctionBlock::getDataOutput(forte::core::StringId paDONameId) {
  TPortId unDID = getFBInterfaceSpec().getDOID(paDONameId);
  if (unDID != cgInvalidPortId) {
    return getDO(unDID);
  }
  return nullptr;
}

CIEC_ANY *CFunctionBlock::getDataInput(forte::core::StringId paDINameId) {
  TPortId unDID = getFBInterfaceSpec().getDIID(paDINameId);
  if (unDID != cgInvalidPortId) {
    return getDI(unDID);
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

CIEC_ANY *CFunctionBlock::getVar(forte::core::StringId *paNameList, unsigned int paNameListSize) {
  if (1 == paNameListSize) {
    TPortId portId = getFBInterfaceSpec().getDIID(*paNameList);
    if (cgInvalidPortId != portId) {
      return getDI(portId);
    }
    portId = getFBInterfaceSpec().getDOID(*paNameList);
    if (cgInvalidPortId != portId) {
      return getDO(portId);
    }
    portId = getFBInterfaceSpec().getDIOID(*paNameList);
    if (cgInvalidPortId != portId) {
      return getDIO(portId);
    }
  }
  return nullptr;
}

forte::IPlugPin *CFunctionBlock::getPlugPin(forte::core::StringId paPlugNameId) {
  TPortId plugPortId = getFBInterfaceSpec().getPlugID(paPlugNameId);
  if (plugPortId != cgInvalidPortId) {
    return getPlugPinUnchecked(plugPortId);
  }
  return nullptr;
}

forte::ISocketPin *CFunctionBlock::getSocketPin(forte::core::StringId paSocketNameId) {
  TPortId sktPortId = getFBInterfaceSpec().getSocketID(paSocketNameId);
  if (sktPortId != cgInvalidPortId) {
    return getSocketPinUnchecked(sktPortId);
  }
  return nullptr;
}

void CFunctionBlock::sendAdapterEvent(forte::CAdapter &paAdapter,
                                      TEventID paEID,
                                      CEventChainExecutionThread *const paECET) {
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

CConnection *CFunctionBlock::getInputConnection(const std::span<const forte::core::StringId> paDstNameList) {
  if (paDstNameList.empty()) {
    return nullptr;
  }
  forte::core::StringId name = paDstNameList.front();
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

CConnection::Wrapper CFunctionBlock::getOutputConnection(const std::span<const forte::core::StringId> paSrcNameList) {
  if (paSrcNameList.empty()) {
    return {};
  }
  forte::core::StringId name = paSrcNameList.front();
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
  mForces.resize(getFBInterfaceSpec().getNumDIs() + getFBInterfaceSpec().getNumDOs() +
                 getFBInterfaceSpec().getNumDIOs());
}

void CFunctionBlock::freeEventMonitoringData() {
  mEventMonitorCount.clear();
  mForces.clear();
}

TForteUInt32 &CFunctionBlock::getEIMonitorData(TEventID paEIID) {
  return mEventMonitorCount[paEIID];
}

TForteUInt32 &CFunctionBlock::getEOMonitorData(TEventID paEOID) {
  return mEventMonitorCount[getFBInterfaceSpec().getNumEIs() + paEOID];
}

TAbsDataPortNum CFunctionBlock::getAbsDataPortNum(forte::core::StringId paPortNameId) const {
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

void CFunctionBlock::setForce(TAbsDataPortNum paAbsDataPortNum, bool paForceValue) {
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
}

void CFunctionBlock::toString(std::string &paTargetBuf) const {
  paTargetBuf += '(';

  for (size_t i = 0; i < getFBInterfaceSpec().getNumDIs(); ++i) {
    forte::core::util::writeToStringNameValuePair(paTargetBuf, getFBInterfaceSpec().mDINames[i], getDI(i));
    paTargetBuf += csmToStringSeparator;
  }

  for (size_t i = 0; i < getFBInterfaceSpec().getNumDOs(); ++i) {
    forte::core::util::writeToStringNameValuePair(paTargetBuf, getFBInterfaceSpec().mDONames[i], getDO(i));
    paTargetBuf += csmToStringSeparator;
  }

  for (size_t i = 0; i < getFBInterfaceSpec().getNumDIOs(); ++i) {
    forte::core::util::writeToStringNameValuePair(paTargetBuf, getFBInterfaceSpec().mDIONames[i], getDIO(i));
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
  if (auto &tracer = getResource()->getTracer(); tracer.isEnabled()) {
    tracer.traceReceiveInputEvent(getFBTypeName(), getFullQualifiedApplicationInstanceName('.').c_str(),
                                  static_cast<uint64_t>(paEIID));
    traceInstanceData();
  }
}

void CFunctionBlock::traceReadData(TPortId paDINum, CIEC_ANY &paValue) {
  if (auto &tracer = getResource()->getTracer(); tracer.isEnabled()) {

    std::string valueString;
    paValue.toString(valueString);
    tracer.traceInputData(getFBTypeName(), getFullQualifiedApplicationInstanceName('.').c_str(),
                          static_cast<uint64_t>(paDINum), valueString.c_str());
  }
}

void CFunctionBlock::traceWriteData(TPortId paDONum, CIEC_ANY &paValue) {
  if (auto &tracer = getResource()->getTracer(); tracer.isEnabled()) {

    std::string valueString;
    paValue.toString(valueString);
    tracer.traceOutputData(getFBTypeName(), getFullQualifiedApplicationInstanceName('.').c_str(),
                           static_cast<uint64_t>(paDONum), valueString.c_str());
  }
}

#endif
