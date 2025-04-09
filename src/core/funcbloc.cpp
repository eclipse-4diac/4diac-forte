/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN, fortiss GmbH,
 *                          Johannes Kepler University
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Gunnar Grabmaier, Alois Zoitl, Smodic Rene, Ingo Hegny,
 *    Gerhard Ebenhofer, Michael Hofmann, Martin Melik Merkumians, Monika Wenger,
 *    Matthias Plasch
 *      - initial implementation and rework communication infrastructure
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *    Martin Jobst - add CTF tracing integration
 *                 - account for data type size in FB initialization
 *******************************************************************************/
#include "funcbloc.h"
#include "dataconn.h"
#include "eventconn.h"

USE_STRING_ID(ARRAY);
USE_STRING_ID(Event);

#include <stdlib.h>
#include <string.h>
#include "../arch/timerha.h"
#include "adapter.h"
#include "adapterconn.h"
#include "device.h"

#include "forte_array_dynamic.h"

CFunctionBlock::CFunctionBlock(forte::core::CFBContainer &paContainer,
                               const SFBInterfaceSpec &paInterfaceSpec,
                               CStringDictionary::TStringId paInstanceNameId) :
    CFBContainer(paInstanceNameId, paContainer), mInterfaceSpec(paInterfaceSpec),
#ifdef FORTE_SUPPORT_MONITORING
    mEOMonitorCount(nullptr), mEIMonitorCount(nullptr),
#endif
    mFBState(E_FBStates::Idle), // put the FB in the idle state to avoid a useless reset after creation
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
  
  //disconnect all event connections
  for(TPortId eoId = 0; eoId < getFBInterfaceSpec().mNumEOs; eoId++) {
    CEventConnection *eoConn = getEOConUnchecked(eoId); 
    for(auto connPoint : eoConn->getDestinationList()) {
      eoConn->disconnect(connPoint.getFB(), connPoint.getPortId());
    }
  }
  
  //disconnect all data input connections
  for(TPortId diId = 0; diId < getFBInterfaceSpec().mNumDIs; diId++) {
    CDataConnection *diConn = *getDIConUnchecked(diId);
    if(diConn != nullptr) {
      diConn->disconnect(*this, getFBInterfaceSpec().mDINames[diId]);
      if(diConn->isDelegating()){
        delete diConn;
      }
    }
  }

  //disconnect all adapter input connections
  for(TPortId aiId = 0; aiId < getFBInterfaceSpec().mNumAdapters; aiId++) {
    CAdapter *adp = getAdapterUnchecked(aiId);
    if(adp->isSocket() && adp->getAdapterConnection() != nullptr) {
      adp->getAdapterConnection()->disconnect(*this, getFBInterfaceSpec().mAdapterInstanceDefinition[aiId].mAdapterNameID);
    }
  }
  
  //disconnect all dio input connections  
  for(TPortId dioId = 0; dioId < getFBInterfaceSpec().mNumDIOs; dioId++) {
    CDataConnection *dioConn = *getDIOInConUnchecked(dioId);
    if(dioConn != nullptr){
       dioConn->disconnect(*this, getFBInterfaceSpec().mDIONames[dioId]);
    }
  }
}

CTimerHandler &CFunctionBlock::getTimer() {
  return getDevice()->getTimer();
}

CStringDictionary::TStringId CFunctionBlock::getEIType(TEventID paEIID) const {
  if (getFBInterfaceSpec().mEITypeNames != nullptr) {
    return getFBInterfaceSpec().mEITypeNames[paEIID];
  }
  return STRID(Event);
}

CStringDictionary::TStringId CFunctionBlock::getEOType(TEventID paEOID) const {
  if (getFBInterfaceSpec().mEOTypeNames != nullptr) {
    return getFBInterfaceSpec().mEOTypeNames[paEOID];
  }
  return STRID(Event);
}

CEventConnection *CFunctionBlock::getEOConnection(CStringDictionary::TStringId paEONameId) {
  CEventConnection *retVal = nullptr;
  TPortId portId = getPortId(paEONameId, getFBInterfaceSpec().mNumEOs, getFBInterfaceSpec().mEONames);
  if (cgInvalidPortId != portId) {
    retVal = getEOConUnchecked(portId);
  }
  return retVal;
}

const CEventConnection *CFunctionBlock::getEOConnection(CStringDictionary::TStringId paEONameId) const {
  const CEventConnection *retVal = nullptr;
  TPortId portId = getPortId(paEONameId, getFBInterfaceSpec().mNumEOs, getFBInterfaceSpec().mEONames);
  if (cgInvalidPortId != portId) {
    retVal = const_cast<CFunctionBlock *>(this)->getEOConUnchecked(portId);
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
  CDataConnection *retVal = nullptr;
  TPortId diPortID = getDIID(paDINameId);
  if (cgInvalidPortId != diPortID) {
    retVal = *getDIConUnchecked(diPortID);
  }
  return retVal;
}

const CDataConnection *CFunctionBlock::getDIConnection(CStringDictionary::TStringId paDINameId) const {
  const CDataConnection *retVal = nullptr;
  TPortId diPortID = getDIID(paDINameId);
  if (cgInvalidPortId != diPortID) {
    retVal = *const_cast<CFunctionBlock *>(this)->getDIConUnchecked(diPortID);
  }
  return retVal;
}

CDataConnection *CFunctionBlock::getDOConnection(CStringDictionary::TStringId paDONameId) {
  CDataConnection *retVal = nullptr;
  TPortId doPortID = getDOID(paDONameId);
  if (cgInvalidPortId != doPortID) {
    retVal = getDOConUnchecked(doPortID);
  }
  return retVal;
}

const CDataConnection *CFunctionBlock::getDOConnection(CStringDictionary::TStringId paDONameId) const {
  const CDataConnection *retVal = nullptr;
  TPortId doPortID = getDOID(paDONameId);
  if (cgInvalidPortId != doPortID) {
    retVal = const_cast<CFunctionBlock *>(this)->getDOConUnchecked(doPortID);
  }
  return retVal;
}

CInOutDataConnection *CFunctionBlock::getDIOInConnection(CStringDictionary::TStringId paDIONameId) {
  CInOutDataConnection *retVal = nullptr;
  TPortId doPortID = getDIOID(paDIONameId);
  if (cgInvalidPortId != doPortID) {
    retVal = *getDIOInConUnchecked(doPortID);
  }
  return retVal;
}

const CInOutDataConnection *CFunctionBlock::getDIOInConnection(CStringDictionary::TStringId paDIONameId) const {
  const CInOutDataConnection *retVal = nullptr;
  TPortId doPortID = getDIOID(paDIONameId);
  if (cgInvalidPortId != doPortID) {
    retVal = *const_cast<CFunctionBlock *>(this)->getDIOInConUnchecked(doPortID);
  }
  return retVal;
}

CInOutDataConnection *CFunctionBlock::getDIOOutConnection(CStringDictionary::TStringId paDIONameId) {
  CInOutDataConnection *retVal = nullptr;
  TPortId doPortID = getDIOID(paDIONameId);
  if (cgInvalidPortId != doPortID) {
    retVal = getDIOOutConUnchecked(doPortID);
  }
  return retVal;
}

const CInOutDataConnection *CFunctionBlock::getDIOOutConnection(CStringDictionary::TStringId paDIONameId) const {
  const CInOutDataConnection *retVal = nullptr;
  TPortId doPortID = getDIOID(paDIONameId);
  if (cgInvalidPortId != doPortID) {
    retVal = const_cast<CFunctionBlock *>(this)->getDIOOutConUnchecked(doPortID);
  }
  return retVal;
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
  CIEC_ANY *poRetVal = nullptr;
  TPortId unDID = getDOID(paDONameId);

  if (cgInvalidPortId != unDID) {
    poRetVal = getDO(unDID);
  }
  return poRetVal;
}

CIEC_ANY *CFunctionBlock::getDataInput(CStringDictionary::TStringId paDINameId) {
  CIEC_ANY *poRetVal = nullptr;
  TPortId unDID = getDIID(paDINameId);

  if (cgInvalidPortId != unDID) {
    poRetVal = getDI(unDID);
  }
  return poRetVal;
}

CIEC_ANY *CFunctionBlock::getDIFromPortId(TPortId paDIPortId) {
  CIEC_ANY *retVal = nullptr;
  if (paDIPortId < getFBInterfaceSpec().mNumDIs) {
    retVal = getDI(paDIPortId);
  }
  return retVal;
}

CIEC_ANY *CFunctionBlock::getDOFromPortId(TPortId paDOPortId) {
  CIEC_ANY *retVal = nullptr;
  if (paDOPortId < getFBInterfaceSpec().mNumDOs) {
    retVal = getDO(paDOPortId);
  }
  return retVal;
}

CIEC_ANY *CFunctionBlock::getDIOFromPortId(TPortId paDIPortId) {
  if (paDIPortId < getFBInterfaceSpec().mNumDIOs) {
    return getDIO(paDIPortId);
  }
  return nullptr;
}

CIEC_ANY *CFunctionBlock::getVar(CStringDictionary::TStringId *paNameList, unsigned int paNameListSize) {

  if (1 == paNameListSize) {
    TPortId portId = getDIID(*paNameList);
    if (cgInvalidPortId != portId) {
      return getDI(portId);
    }
    portId = getDOID(*paNameList);
    if (cgInvalidPortId != portId) {
      return getDO(portId);
    }
    portId = getDIOID(*paNameList);
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
  const CStringDictionary::TStringId *pnDataIds;

  pnDataIds = getFBInterfaceSpec().mDIDataTypeNames;
  for (TPortId i = 0; i < getFBInterfaceSpec().mNumDIs; ++i) {
    TForteByte *varsData = nullptr;
    CIEC_ANY *value = createDataPoint(pnDataIds, varsData);
    if (value != nullptr) {
      getDI(i)->setValue(*value);
    }
    delete value;
  }

  pnDataIds = getFBInterfaceSpec().mDODataTypeNames;
  for (TPortId i = 0; i < getFBInterfaceSpec().mNumDOs; ++i) {
    TForteByte *varsData = nullptr;
    CIEC_ANY *value = createDataPoint(pnDataIds, varsData);
    if (value != nullptr) {
      getDO(i)->setValue(*value);
    }
    delete value;
  }
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
    default:
      nRetVal = EMGMResponse::InvalidOperation;
      break;
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

size_t CFunctionBlock::getDataPointSize(const CStringDictionary::TStringId *&paDataTypeIds) {
  CStringDictionary::TStringId dataTypeId = *paDataTypeIds;
  auto *entry = static_cast<CTypeLib::CDataTypeEntry *>(CTypeLib::findType(dataTypeId, CTypeLib::getDTLibStart()));
  nextDataPoint(paDataTypeIds);
  return nullptr != entry ? entry->getSize() : 0;
}

CIEC_ANY *CFunctionBlock::createDataPoint(const CStringDictionary::TStringId *&paDataTypeIds, TForteByte *&paDataBuf) {
  CStringDictionary::TStringId dataTypeId = *paDataTypeIds;
  CIEC_ANY *poRetVal = CTypeLib::createDataTypeInstance(dataTypeId, paDataBuf);
  if (nullptr != poRetVal) {
    if (STRID(ARRAY) == dataTypeId) {
      static_cast<CIEC_ARRAY_DYNAMIC *>(poRetVal)->setup(paDataTypeIds + 1);
    }
    paDataBuf += poRetVal->getSizeof();
  }
  nextDataPoint(paDataTypeIds);
  return poRetVal;
}

void CFunctionBlock::nextDataPoint(const CStringDictionary::TStringId *&paDataTypeIds) {
  while (*(paDataTypeIds++) == STRID(ARRAY)) {
    paDataTypeIds += 2;
  }
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

TPortId CFunctionBlock::getPortId(CStringDictionary::TStringId paPortNameId,
                                  TPortId paMaxPortNames,
                                  const CStringDictionary::TStringId *paPortNames) {
  for (TPortId i = 0; i < paMaxPortNames; ++i) {
    if (paPortNameId == paPortNames[i]) {
      return i;
    }
  }
  return cgInvalidPortId;
}

//********************************** below here are monitoring specific functions
//**********************************************************
#ifdef FORTE_SUPPORT_MONITORING
void CFunctionBlock::setupEventMonitoringData() {
  freeEventMonitoringData();

  if (0 != getFBInterfaceSpec().mNumEIs) {
    mEIMonitorCount = new TForteUInt32[getFBInterfaceSpec().mNumEIs]{};
  }

  if (0 != getFBInterfaceSpec().mNumEOs) {
    mEOMonitorCount = new TForteUInt32[getFBInterfaceSpec().mNumEOs]{};
  }
}

void CFunctionBlock::freeEventMonitoringData() {
  delete[] mEOMonitorCount;
  mEOMonitorCount = nullptr;
  delete[] mEIMonitorCount;
  mEIMonitorCount = nullptr;
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
  return mEIMonitorCount[paEIID];
}

TForteUInt32 &CFunctionBlock::getEOMonitorData(TEventID paEOID) {
  return mEOMonitorCount[paEOID];
}

#endif // FORTE_SUPPORT_MONITORING

int CFunctionBlock::writeToStringNameValuePair(char *paValue,
                                               size_t paBufferSize,
                                               const CStringDictionary::TStringId variableNameId,
                                               const CIEC_ANY *const variable) const {
  size_t usedBuffer = 0;
  const char *const variableName = CStringDictionary::get(variableNameId);
  size_t nameLength = strlen(variableName);
  if ((paBufferSize - usedBuffer) < nameLength + 3) { // := and \0
    return -1;
  }
  strcpy(paValue + usedBuffer, variableName);
  strcpy(paValue + usedBuffer + nameLength, ":=");
  usedBuffer += nameLength + 2;
  int result = variable->toString(paValue + usedBuffer, paBufferSize - usedBuffer);
  if (result < 0) {
    return -1;
  }
  usedBuffer += static_cast<size_t>(result);
  return static_cast<int>(usedBuffer);
}

int CFunctionBlock::toString(char *paValue, size_t paBufferSize) const {
  size_t usedBuffer = 0;
  if (paBufferSize < 1) {
    return -1;
  }
  *paValue = '(';
  ++usedBuffer;
  for (size_t i = 0; i < getFBInterfaceSpec().mNumDIs; ++i) {
    const CIEC_ANY *const variable = getDI(i);
    const CStringDictionary::TStringId nameId = getFBInterfaceSpec().mDINames[i];
    int result = writeToStringNameValuePair(paValue + usedBuffer, paBufferSize - usedBuffer, nameId, variable);
    if (result >= 0) {
      usedBuffer += static_cast<size_t>(result);
    } else {
      return -1;
    }
    if (paBufferSize - usedBuffer >= sizeof(csmToStringSeparator)) {
      strncpy(paValue + usedBuffer, csmToStringSeparator, paBufferSize - usedBuffer);
      usedBuffer += sizeof(csmToStringSeparator) - 1;
    } else {
      return -1;
    }
  }
  for (size_t i = 0; i < getFBInterfaceSpec().mNumDOs; ++i) {
    const CIEC_ANY *const variable = getDO(i);
    const CStringDictionary::TStringId nameId = getFBInterfaceSpec().mDONames[i];
    int result = writeToStringNameValuePair(paValue + usedBuffer, paBufferSize - usedBuffer, nameId, variable);
    if (result >= 0) {
      usedBuffer += static_cast<size_t>(result);
    } else {
      return -1;
    }
    if (paBufferSize - usedBuffer >= sizeof(csmToStringSeparator)) {
      strncpy(paValue + usedBuffer, csmToStringSeparator, paBufferSize - usedBuffer);
      usedBuffer += sizeof(csmToStringSeparator) - 1;
    } else {
      return -1;
    }
  }
  for (size_t i = 0; i < getFBInterfaceSpec().mNumDIOs; ++i) {
    const CIEC_ANY *const variable = getDIO(i);
    const CStringDictionary::TStringId nameId = getFBInterfaceSpec().mDIONames[i];
    int result = writeToStringNameValuePair(paValue + usedBuffer, paBufferSize - usedBuffer, nameId, variable);
    if (result >= 0) {
      usedBuffer += static_cast<size_t>(result);
    } else {
      return -1;
    }
    if (paBufferSize - usedBuffer >= sizeof(csmToStringSeparator)) {
      strncpy(paValue + usedBuffer, csmToStringSeparator, paBufferSize - usedBuffer);
      usedBuffer += sizeof(csmToStringSeparator) - 1;
    } else {
      return -1;
    }
  }
  usedBuffer = std::max(
      usedBuffer, std::size_t{3}); // ensure usedBuffer is at least 3 in case nothing was added beyond the opening '('
  strncpy(paValue + (usedBuffer - 2), ")",
          paBufferSize - (usedBuffer - 2)); // overwrite the last two bytes with the closing ')'
  return static_cast<int>(usedBuffer - 1);
}

size_t CFunctionBlock::getToStringBufferSize() const {
  size_t bufferSize = 3;
  for (size_t i = 0; i < getFBInterfaceSpec().mNumDIs; ++i) {
    const CIEC_ANY *const variable = getDI(i);
    const CStringDictionary::TStringId nameId = getFBInterfaceSpec().mDINames[i];
    const char *varName = CStringDictionary::get(nameId);
    bufferSize +=
        strlen(varName) + 4 + variable->getToStringBufferSize(); // compensation for := and , for every variable
  }
  for (size_t i = 0; i < getFBInterfaceSpec().mNumDOs; ++i) {
    const CIEC_ANY *const variable = getDO(i);
    const CStringDictionary::TStringId nameId = getFBInterfaceSpec().mDONames[i];
    const char *varName = CStringDictionary::get(nameId);
    bufferSize +=
        strlen(varName) + 4 + variable->getToStringBufferSize(); // compensation for := and , for every variable
  }
  for (size_t i = 0; i < getFBInterfaceSpec().mNumDIOs; ++i) {
    const CIEC_ANY *const variable = getDIO(i);
    const CStringDictionary::TStringId nameId = getFBInterfaceSpec().mDIONames[i];
    const char *varName = CStringDictionary::get(nameId);
    bufferSize +=
        strlen(varName) + 4 + variable->getToStringBufferSize(); // compensation for := and , for every variable
  }
  return bufferSize;
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
