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
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include "funcbloc.h"
#include "dataconn.h"
#include "datatype.h"
#include "eventconn.h"

#include <stdlib.h>
#include <string.h>
#include "../arch/timerha.h"
#include "adapter.h"
#include "adapterconn.h"
#include "device.h"
#include "string_utils.h"

using namespace std::string_literals;

CFunctionBlock::CFunctionBlock(forte::core::CFBContainer &paContainer,
                               const SFBInterfaceSpec &paInterfaceSpec,
                               CStringDictionary::TStringId paInstanceNameId) :
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
#ifdef FORTE_SUPPORT_MONITORING
  setupEventMonitoringData();
#endif // FORTE_SUPPORT_MONITORING
  setupInputConnectionTrackingData();
  return true;
}

CFunctionBlock::~CFunctionBlock() {
#ifdef FORTE_SUPPORT_MONITORING
  freeEventMonitoringData();
#endif // FORTE_SUPPORT_MONITORING
}

void CFunctionBlock::deinitialize() {
  CFBContainer::deinitialize();

  // disconnect all event connections
  for (TPortId eoId = 0; eoId < getFBInterfaceSpec().mNumEOs; eoId++) {
    CEventConnection *eoConn = getEOConUnchecked(eoId);
    for (auto connPoint : eoConn->getDestinationList()) {
      eoConn->disconnect(connPoint.getFB(), connPoint.getPortId());
    }
  }

  // disconnect all data input connections
  for (TPortId diId = 0; diId < getFBInterfaceSpec().mNumDIs; diId++) {
    CDataConnection *diConn = *getDIConUnchecked(diId);
    if (diConn != nullptr) {
      diConn->disconnect(*this, getFBInterfaceSpec().mDINames[diId]);
      if (diConn->isDelegating()) {
        delete diConn;
      }
    }
  }

  // disconnect all adapter input connections
  for (TPortId aiId = 0; aiId < getFBInterfaceSpec().mNumAdapters; aiId++) {
    CAdapter *adp = getAdapterUnchecked(aiId);
    if (adp->isSocket() && adp->getAdapterConnection() != nullptr) {
      adp->getAdapterConnection()->disconnect(*this,
                                              getFBInterfaceSpec().mAdapterInstanceDefinition[aiId].mAdapterNameID);
    }
  }

  // disconnect all dio input connections
  for (TPortId dioId = 0; dioId < getFBInterfaceSpec().mNumDIOs; dioId++) {
    CDataConnection *dioConn = *getDIOInConUnchecked(dioId);
    if (dioConn != nullptr) {
      dioConn->disconnect(*this, getFBInterfaceSpec().mDIONames[dioId]);
    }
  }
}

CTimerHandler &CFunctionBlock::getTimer() {
  return getDevice()->getTimer();
}

CEventConnection *CFunctionBlock::getEOConnection(CStringDictionary::TStringId paEONameId) {
  CEventConnection *retVal = nullptr;
  TPortId portId = getFBInterfaceSpec().getEOID(paEONameId);
  if (cgInvalidPortId != portId) {
    retVal = getEOConUnchecked(portId);
  }
  return retVal;
}

bool CFunctionBlock::connectDI(TPortId paDIPortId, CDataConnection *paDataCon) {
  if (paDIPortId >= getFBInterfaceSpec().mNumDIs) { // catch invalid ID
    return false;
  }

  CDataConnection **conn = getDIConUnchecked(paDIPortId);
  if (!paDataCon) { // disconnect?
    *conn = nullptr;
    return true;
  }
  if (*conn && paDataCon != *conn) { // already connected to different connection?
    DEVLOG_ERROR("%s cannot connect input data %s to more sources, using the latest connection attempt\n",
                 getInstanceName(), CStringDictionary::get(getFBInterfaceSpec().mDINames[paDIPortId]));
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
  if (paDIOPortId >= getFBInterfaceSpec().mNumDIOs) { // catch invalid ID
    return false;
  }

  CInOutDataConnection **conn = getDIOInConUnchecked(paDIOPortId);
  if (!paDataCon) { // disconnect?
    *conn = nullptr;
    return true;
  }
  if (*conn && paDataCon != *conn) { // already connected to different connection?
    DEVLOG_ERROR("%s cannot connect InOut data %s to more sources, using the latest connection attempt\n",
                 getInstanceName(), CStringDictionary::get(getFBInterfaceSpec().mDIONames[paDIOPortId]));
    return false;
  }
  *conn = paDataCon;
  getDIOOutConUnchecked(paDIOPortId)->setValue(&paDataCon->getValue());
  return configureGenericDIO(paDIOPortId, paDataCon->getValue());
}

bool CFunctionBlock::configureGenericDIO(TPortId paDIOPortId, const CIEC_ANY &paRefValue) {
  CIEC_ANY *dio = getDIO(paDIOPortId);
  if (dio->getDataTypeID() == CIEC_ANY::e_ANY) {
    dio->setValue(paRefValue.unwrap());
  }
  return true;
}

CDataConnection *CFunctionBlock::getDIConnection(CStringDictionary::TStringId paDINameId) {
  TPortId diPortID = getFBInterfaceSpec().getDIID(paDINameId);
  if (diPortID != cgInvalidPortId) {
    return *getDIConUnchecked(diPortID);
  }
  return nullptr;
}

CDataConnection *CFunctionBlock::getDOConnection(CStringDictionary::TStringId paDONameId) {
  TPortId doPortID = getFBInterfaceSpec().getDOID(paDONameId);
  if (doPortID != cgInvalidPortId) {
    return getDOConUnchecked(doPortID);
  }
  return nullptr;
}

CInOutDataConnection *CFunctionBlock::getDIOInConnection(CStringDictionary::TStringId paDIONameId) {
  TPortId doPortID = getFBInterfaceSpec().getDIOID(paDIONameId);
  if (doPortID != cgInvalidPortId) {
    return *getDIOInConUnchecked(doPortID);
  }
  return nullptr;
}

CInOutDataConnection *CFunctionBlock::getDIOOutConnection(CStringDictionary::TStringId paDIONameId) {
  TPortId doPortID = getFBInterfaceSpec().getDIOID(paDIONameId);
  if (doPortID != cgInvalidPortId) {
    return getDIOOutConUnchecked(doPortID);
  }
  return nullptr;
}

bool CFunctionBlock::configureGenericDO(TPortId paDOPortId, const CIEC_ANY &paRefValue) {
  if (paDOPortId >= getFBInterfaceSpec().mNumDOs) {
    return false;
  }
  CIEC_ANY *dataOutput = getDO(paDOPortId);
  if (dataOutput->getDataTypeID() == CIEC_ANY::e_ANY) {
    dataOutput->setValue(paRefValue.unwrap());
  }
  return true;
}

CIEC_ANY *CFunctionBlock::getDataOutput(CStringDictionary::TStringId paDONameId) {
  TPortId unDID = getFBInterfaceSpec().getDOID(paDONameId);
  if (unDID != cgInvalidPortId) {
    return getDO(unDID);
  }
  return nullptr;
}

CIEC_ANY *CFunctionBlock::getDataInput(CStringDictionary::TStringId paDINameId) {
  TPortId unDID = getFBInterfaceSpec().getDIID(paDINameId);
  if (unDID != cgInvalidPortId) {
    return getDI(unDID);
  }
  return nullptr;
}

CIEC_ANY *CFunctionBlock::getDIFromPortId(TPortId paDIPortId) {
  if (paDIPortId < getFBInterfaceSpec().mNumDIs) {
    return getDI(paDIPortId);
  }
  return nullptr;
}

CIEC_ANY *CFunctionBlock::getDOFromPortId(TPortId paDOPortId) {
  if (paDOPortId < getFBInterfaceSpec().mNumDOs) {
    return getDO(paDOPortId);
  }
  return nullptr;
}

CIEC_ANY *CFunctionBlock::getDIOFromPortId(TPortId paDIPortId) {
  if (paDIPortId < getFBInterfaceSpec().mNumDIOs) {
    return getDIO(paDIPortId);
  }
  return nullptr;
}

CIEC_ANY *CFunctionBlock::getVar(CStringDictionary::TStringId *paNameList, unsigned int paNameListSize) {
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

CAdapter *CFunctionBlock::getAdapter(CStringDictionary::TStringId paAdapterNameId) {
  TPortId adpPortId = getAdapterPortId(paAdapterNameId);
  if (cgInvalidPortId != adpPortId) {
    return getAdapterUnchecked(adpPortId);
  }
  return nullptr;
}

const CAdapter *CFunctionBlock::getAdapter(CStringDictionary::TStringId paAdapterNameId) const {
  TPortId adpPortId = getAdapterPortId(paAdapterNameId);
  if (cgInvalidPortId != adpPortId) {
    return const_cast<CFunctionBlock *>(this)->getAdapterUnchecked(adpPortId);
  }
  return nullptr;
}

TPortId CFunctionBlock::getAdapterPortId(CStringDictionary::TStringId paAdapterNameId) const {
  for (TPortId i = 0; i < getFBInterfaceSpec().mNumAdapters; ++i) {
    if (getFBInterfaceSpec().mAdapterInstanceDefinition[i].mAdapterNameID == paAdapterNameId) {
      return i;
    }
  }
  return cgInvalidPortId;
}

void CFunctionBlock::sendAdapterEvent(TPortId paAdapterID, TEventID paEID, CEventChainExecutionThread *const paECET) {
  if (paAdapterID < getFBInterfaceSpec().mNumAdapters) {
    if (CAdapter *adapter = getAdapterUnchecked(paAdapterID); adapter != nullptr) {
      adapter->receiveInputEvent(paEID, paECET);
    }
  }
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
    for (TPortId i = 0; i < getFBInterfaceSpec().mNumAdapters; ++i) {
      if (CAdapter *adapter = getAdapterUnchecked(i); adapter != nullptr) {
        adapter->changeExecutionState(paCommand);
      }
    }
  }

  if (EMGMResponse::Ready == nRetVal) {
    nRetVal = CFBContainer::changeExecutionState(paCommand);
  }

  return nRetVal;
}

CAdapter *CFunctionBlock::createAdapter(const SAdapterInstanceDef &paAdapterInstanceDefinition,
                                        TForteUInt8 paParentAdapterlistID) {
  CAdapter *adapter = CTypeLib::createAdapter(paAdapterInstanceDefinition.mAdapterNameID,
                                              paAdapterInstanceDefinition.mAdapterTypeNameID, *this,
                                              paAdapterInstanceDefinition.mIsPlug);
  if (adapter) {
    adapter->setParentFB(this, paParentAdapterlistID);
  }
  return adapter;
}

void CFunctionBlock::destroyAdapter(CAdapter *paAdapter) {
  delete paAdapter;
}

CConnection *CFunctionBlock::getInputConnection(forte::core::TNameIdentifier &paDstNameList) {
  if (paDstNameList.empty()) {
    return nullptr;
  }
  CStringDictionary::TStringId name = paDstNameList.front();
  if (const auto conn = getDIConnection(name); conn) {
    return conn;
  };
  if (const auto conn = getDIOInConnection(name); conn) {
    return conn;
  };
  if (const auto adapter = getAdapter(name); adapter) {
    return adapter->getAdapterConnection();
  };
  return CFBContainer::getInputConnection(paDstNameList);
}

CConnection::Wrapper CFunctionBlock::getOutputConnection(forte::core::TNameIdentifier &paSrcNameList) {
  if (paSrcNameList.empty()) {
    return CConnection::Wrapper();
  }
  CStringDictionary::TStringId name = paSrcNameList.front();
  if (const auto conn = getEOConnection(name); conn) {
    paSrcNameList.erase(paSrcNameList.cbegin());
    return conn->getDelegatingConnection(paSrcNameList);
  };
  if (const auto conn = getDOConnection(name); conn) {
    paSrcNameList.erase(paSrcNameList.cbegin());
    return conn->getDelegatingConnection(paSrcNameList);
  };
  if (const auto conn = getDIOOutConnection(name); conn) {
    paSrcNameList.erase(paSrcNameList.cbegin());
    return conn->getDelegatingConnection(paSrcNameList);
  };
  if (const auto adapter = getAdapter(name); adapter) {
    paSrcNameList.erase(paSrcNameList.cbegin());
    return adapter->getAdapterConnection()->getDelegatingConnection(paSrcNameList);
  };
  return CFBContainer::getOutputConnection(paSrcNameList);
}

//********************************** below here are monitoring specific functions
//**********************************************************
#ifdef FORTE_SUPPORT_MONITORING
void CFunctionBlock::setupEventMonitoringData() {
  freeEventMonitoringData();
  mEventMonitorCount.resize(getFBInterfaceSpec().mNumEIs + getFBInterfaceSpec().mNumEOs);
  mForces.resize(getFBInterfaceSpec().mNumDIs + getFBInterfaceSpec().mNumDOs + getFBInterfaceSpec().mNumDIOs);
}

void CFunctionBlock::freeEventMonitoringData() {
  mEventMonitorCount.clear();
  mForces.clear();
}

CFunctionBlock *CFunctionBlock::getFB(NameIterator &paNameListIt, NameIterator paNameListEnd) {
  CFunctionBlock *retVal = nullptr;

  if (paNameListIt + 1 == paNameListEnd) {
    // only check for adpaters if it we have the last entry in the line
    retVal = getAdapter(*paNameListIt);
  }
  if (retVal == nullptr) {
    retVal = CFBContainer::getFB(paNameListIt, paNameListEnd);
  }

  return retVal;
}

TForteUInt32 &CFunctionBlock::getEIMonitorData(TEventID paEIID) {
  return mEventMonitorCount[paEIID];
}

TForteUInt32 &CFunctionBlock::getEOMonitorData(TEventID paEOID) {
  return mEventMonitorCount[getFBInterfaceSpec().mNumEIs + paEOID];
}

TAbsDataPortNum CFunctionBlock::getAbsDataPortNum(CStringDictionary::TStringId paPortNameId) const {
  TPortId paPortId = getFBInterfaceSpec().getDIID(paPortNameId);
  if (paPortId != cgInvalidPortId) {
    return paPortId;
  }

  auto &interfaceSpec = getFBInterfaceSpec();
  paPortId = getFBInterfaceSpec().getDOID(paPortNameId);
  if (paPortId != cgInvalidPortId) {
    return interfaceSpec.mNumDIs + paPortId;
  }

  paPortId = getFBInterfaceSpec().getDIOID(paPortNameId);
  if (paPortId != cgInvalidPortId) {
    return interfaceSpec.mNumDIs + interfaceSpec.mNumDOs + paPortId;
  }

  return INVALID_ABS_DATA_PORT_ID;
}

void CFunctionBlock::setForce(TAbsDataPortNum paAbsDataPortNum, bool paForceValue) {
  mForces[paAbsDataPortNum] = paForceValue;

  if (paForceValue) {
    auto &interfaceSpec = getFBInterfaceSpec();

    if ((interfaceSpec.mNumDIs <= paAbsDataPortNum) &&
        (paAbsDataPortNum < interfaceSpec.mNumDIs + interfaceSpec.mNumDOs)) {
      // it is a data output mirror the forced value there
      TPortId doPortId = paAbsDataPortNum - interfaceSpec.mNumDIs;
      CDataConnection *con = getDOConUnchecked(doPortId);
      con->writeData(*getDO(doPortId));
    }
  }
}

#endif // FORTE_SUPPORT_MONITORING

void CFunctionBlock::toString(std::string &paTargetBuf) const {
  paTargetBuf += '(';

  for (size_t i = 0; i < getFBInterfaceSpec().mNumDIs; ++i) {
    forte::core::util::writeToStringNameValuePair(paTargetBuf, getFBInterfaceSpec().mDINames[i], getDI(i));
    paTargetBuf += csmToStringSeparator;
  }

  for (size_t i = 0; i < getFBInterfaceSpec().mNumDOs; ++i) {
    forte::core::util::writeToStringNameValuePair(paTargetBuf, getFBInterfaceSpec().mDONames[i], getDO(i));
    paTargetBuf += csmToStringSeparator;
  }

  for (size_t i = 0; i < getFBInterfaceSpec().mNumDIOs; ++i) {
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
    tracer.traceReceiveInputEvent(getFBTypeName() ?: "null",
                                  getFullQualifiedApplicationInstanceName('.').c_str() ?: "null",
                                  static_cast<uint64_t>(paEIID));
    traceInstanceData();
  }
}

void CFunctionBlock::traceReadData(TPortId paDINum, CIEC_ANY &paValue) {
  if (auto &tracer = getResource()->getTracer(); tracer.isEnabled()) {
    std::string valueString;
    valueString.reserve(paValue.getToStringBufferSize());
    paValue.toString(valueString.data(), valueString.capacity());
    tracer.traceInputData(getFBTypeName() ?: "null", getFullQualifiedApplicationInstanceName('.').c_str() ?: "null",
                          static_cast<uint64_t>(paDINum), valueString.c_str());
  }
}

void CFunctionBlock::traceWriteData(TPortId paDONum, CIEC_ANY &paValue) {
  if (auto &tracer = getResource()->getTracer(); tracer.isEnabled()) {
    std::string valueString;
    valueString.reserve(paValue.getToStringBufferSize());
    paValue.toString(valueString.data(), valueString.capacity());
    tracer.traceOutputData(getFBTypeName() ?: "null", getFullQualifiedApplicationInstanceName('.').c_str() ?: "null",
                           static_cast<uint64_t>(paDONum), valueString.c_str());
  }
}

#endif
