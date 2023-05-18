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
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "funcbloc_gen.cpp"
#endif
#include "adapter.h"
#include "device.h"
#include "connectiondestinationtype.h"
#include "utils/criticalregion.h"
#include "../arch/timerha.h"
#include <string.h>
#include <stdlib.h>

#include "forte_array_dynamic.h"

#ifdef FORTE_TRACE_CTF
#include "trace/barectf_platform_forte.h"
#endif

CFunctionBlock::CFunctionBlock(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec,
                               CStringDictionary::TStringId pa_nInstanceNameId) :
        mInterfaceSpec(pa_pstInterfaceSpec),
        mEOConns(nullptr), mDIConns(nullptr), mDOConns(nullptr), mDIs(nullptr), mDOs(nullptr),
        mInvokingExecEnv(nullptr), m_apoAdapters(nullptr),
        mFBConnData(nullptr), mFBVarsData(nullptr),
        mResource(pa_poSrcRes), mContainer(nullptr),
#ifdef FORTE_SUPPORT_MONITORING
        mEOMonitorCount(nullptr), mEIMonitorCount(nullptr),
#endif
        mFBInstanceName(pa_nInstanceNameId),
        mFBState(E_FBStates::Idle), // put the FB in the idle state to avoid a useless reset after creation
        mDeletable(true) {
}

bool CFunctionBlock::initialize() {
  setupFBInterface(mInterfaceSpec);
  return true;
}

CFunctionBlock::~CFunctionBlock(){
  freeAllData();
}

void CFunctionBlock::freeAllData(){
  if(nullptr != mInterfaceSpec){
    if(nullptr != mEOConns) {
      std::destroy_n(mEOConns, mInterfaceSpec->m_nNumEOs);
    }

    if(nullptr != mDOConns) {
      for (TPortId i = 0; i < mInterfaceSpec->m_nNumDOs; ++i) {
        if(CIEC_ANY* value = mDOConns[i].getValue(); nullptr != value) {
          std::destroy_at(value);
        }
      }
      std::destroy_n(mDOConns, mInterfaceSpec->m_nNumDOs);
    }

    if(nullptr != mDIs) {
      for (TPortId i = 0; i < mInterfaceSpec->m_nNumDIs; ++i) {
        if(CIEC_ANY* value = mDIs[i]; nullptr != value) {
          std::destroy_at(value);
        }
      }
    }

    if(nullptr != mDOs) {
      for (TPortId i = 0; i < mInterfaceSpec->m_nNumDOs; ++i) {
        if(CIEC_ANY* value = mDOs[i]; nullptr != value) {
          std::destroy_at(value);
        }
      }
    }

    if(nullptr != m_apoAdapters) {
      for (TPortId i = 0; i < mInterfaceSpec->m_nNumAdapters; ++i) {
        delete m_apoAdapters[i];
      }
    }
  }

  operator delete(mFBConnData);
  mFBConnData = nullptr;
  operator delete(mFBVarsData);
  mFBVarsData = nullptr;

#ifdef  FORTE_SUPPORT_MONITORING
  delete[] mEOMonitorCount;
  mEOMonitorCount = nullptr;
  delete[] mEIMonitorCount;
  mEIMonitorCount = nullptr;
#endif //FORTE_SUPPORT_MONITORING
}

void CFunctionBlock::setupAdapters(const SFBInterfaceSpec *pa_pstInterfaceSpec, TForteByte *pa_acFBData){
  if((nullptr != pa_pstInterfaceSpec) && (nullptr != pa_acFBData) && (pa_pstInterfaceSpec->m_nNumAdapters)) {
    m_apoAdapters = reinterpret_cast<TAdapterPtr *>(pa_acFBData);
    for(TPortId i = 0; i < pa_pstInterfaceSpec->m_nNumAdapters; ++i) {
      //set pointer to right place in pa_acFBData
      m_apoAdapters[i] = CTypeLib::createAdapter(pa_pstInterfaceSpec->m_pstAdapterInstanceDefinition[i].m_nAdapterNameID,
        pa_pstInterfaceSpec->m_pstAdapterInstanceDefinition[i].m_nAdapterTypeNameID, getResourcePtr(),
        pa_pstInterfaceSpec->m_pstAdapterInstanceDefinition[i].m_bIsPlug);
      if(nullptr != m_apoAdapters[i]) {
        m_apoAdapters[i]->setParentFB(this, static_cast<TForteUInt8>(i));
      }
    }
  }
}

CTimerHandler& CFunctionBlock::getTimer(){
  return mResource->getDevice().getTimer();
}

CEventConnection *CFunctionBlock::getEOConnection(CStringDictionary::TStringId paEONameId) {
  CEventConnection *retVal = nullptr;
  TPortId portId = getPortId(paEONameId, mInterfaceSpec->m_nNumEOs, mInterfaceSpec->m_aunEONames);
  if(cgInvalidPortId != portId){
    retVal = getEOConUnchecked(portId);
  }
  return retVal;
}

const CEventConnection *CFunctionBlock::getEOConnection(CStringDictionary::TStringId paEONameId) const {
  const CEventConnection *retVal = nullptr;
  TPortId portId = getPortId(paEONameId, mInterfaceSpec->m_nNumEOs, mInterfaceSpec->m_aunEONames);
  if(cgInvalidPortId != portId){
    retVal = const_cast<CFunctionBlock*>(this)->getEOConUnchecked(portId);
  }
  return retVal;
}

bool CFunctionBlock::connectDI(TPortId paDIPortId, CDataConnection *paDataCon){
  bool bRetVal = false;

  if(mInterfaceSpec->m_nNumDIs > paDIPortId) { //catch invalid ID
    if(nullptr == paDataCon){
      *getDIConUnchecked(paDIPortId) = nullptr;
      bRetVal = true;
    } else {
      //only perform connection checks if it is not a disconnection request.
      CDataConnection *conn = *getDIConUnchecked(paDIPortId);
      if(nullptr != conn) {
        if(conn == paDataCon) {
          //we have a reconfiguration attempt
          configureGenericDI(paDIPortId, paDataCon->getValue());
          bRetVal = true;
        } else {
          DEVLOG_ERROR("%s cannot connect input data %s to more sources, using the latest connection attempt\n", getInstanceName(), CStringDictionary::getInstance().get(mInterfaceSpec->m_aunDINames[paDIPortId]));
        }
      } else {
        *getDIConUnchecked(paDIPortId) = paDataCon;
        configureGenericDI(paDIPortId, paDataCon->getValue());
        bRetVal = true;
      }
    }
  }
  return bRetVal;
}

void CFunctionBlock::configureGenericDI(TPortId paDIPortId, const CIEC_ANY* paRefValue) {
  CIEC_ANY *di = getDI(paDIPortId);
  if(di->getDataTypeID() == CIEC_ANY::e_ANY && (nullptr != paRefValue)) {
    di->setValue(paRefValue->unwrap());
  }
}

bool CFunctionBlock::connectDIO(TPortId paDIOPortId, CInOutDataConnection *paDataCon){
  if(mInterfaceSpec->mNumDIOs > paDIOPortId) { //catch invalid ID
    if(nullptr == paDataCon){
      *getDIOInConUnchecked(paDIOPortId) = nullptr;
      return true;
    }
    else {
      //only perform connection checks if it is not a disconnection request.
      CDataConnection *conn = *getDIOInConUnchecked(paDIOPortId);
      if(nullptr != conn) {
        if(conn == paDataCon) {
          //we have a reconfiguration attempt
          configureGenericDIO(paDIOPortId, paDataCon->getValue());
          getDIOOutConUnchecked(paDIOPortId)->setValue(paDataCon->getValue());
          return true;
        } else {
          DEVLOG_ERROR("%s cannot connect InOut data %s to more sources, using the latest connection attempt\n", getInstanceName(), CStringDictionary::getInstance().get(mInterfaceSpec->mDIONames[paDIOPortId]));
        }
      } else {
        *getDIOInConUnchecked(paDIOPortId) = paDataCon;
        configureGenericDIO(paDIOPortId, paDataCon->getValue());
        getDIOOutConUnchecked(paDIOPortId)->setValue(paDataCon->getValue());
        return true;
      }
    }
  }
  return false;
}

void CFunctionBlock::configureGenericDIO(TPortId paDIOPortId, const CIEC_ANY* paRefValue) {
  CIEC_ANY *dio = getDIO(paDIOPortId);
  if(dio->getDataTypeID() == CIEC_ANY::e_ANY && (nullptr != paRefValue)) {
    dio->setValue(paRefValue->unwrap());
  }
}

CDataConnection *CFunctionBlock::getDIConnection(CStringDictionary::TStringId paDINameId) {
  CDataConnection *retVal = nullptr;
  TPortId diPortID = getDIID(paDINameId);
  if(cgInvalidPortId != diPortID) {
    retVal = *getDIConUnchecked(diPortID);
  }
  return retVal;
}

const CDataConnection *CFunctionBlock::getDIConnection(CStringDictionary::TStringId paDINameId) const {
  const CDataConnection *retVal = nullptr;
  TPortId diPortID = getDIID(paDINameId);
  if(cgInvalidPortId != diPortID) {
    retVal = *const_cast<CFunctionBlock*>(this)->getDIConUnchecked(diPortID);
  }
  return retVal;
}

CDataConnection *CFunctionBlock::getDOConnection(CStringDictionary::TStringId paDONameId) {
  CDataConnection *retVal = nullptr;
  TPortId doPortID = getDOID(paDONameId);
  if(cgInvalidPortId != doPortID) {
    retVal = getDOConUnchecked(doPortID);
  }
  return retVal;
}

const CDataConnection *CFunctionBlock::getDOConnection(CStringDictionary::TStringId paDONameId) const {
  const CDataConnection *retVal = nullptr;
  TPortId doPortID = getDOID(paDONameId);
  if(cgInvalidPortId != doPortID) {
    retVal = const_cast<CFunctionBlock*>(this)->getDOConUnchecked(doPortID);
  }
  return retVal;
}

CInOutDataConnection *CFunctionBlock::getDIOInConnection(CStringDictionary::TStringId paDIONameId) {
  CInOutDataConnection *retVal = nullptr;
  TPortId doPortID = getDIOID(paDIONameId);
  if(cgInvalidPortId != doPortID) {
    retVal = *getDIOInConUnchecked(doPortID);
  }
  return retVal;
}

const CInOutDataConnection *CFunctionBlock::getDIOInConnection(CStringDictionary::TStringId paDIONameId) const {
  const CInOutDataConnection *retVal = nullptr;
  TPortId doPortID = getDIOID(paDIONameId);
  if(cgInvalidPortId != doPortID) {
    retVal = *const_cast<CFunctionBlock*>(this)->getDIOInConUnchecked(doPortID);
  }
  return retVal;
}

CInOutDataConnection *CFunctionBlock::getDIOOutConnection(CStringDictionary::TStringId paDIONameId) {
  CInOutDataConnection *retVal = nullptr;
  TPortId doPortID = getDIOID(paDIONameId);
  if(cgInvalidPortId != doPortID) {
    retVal = getDIOOutConUnchecked(doPortID);
  }
  return retVal;
}

const CInOutDataConnection *CFunctionBlock::getDIOOutConnection(CStringDictionary::TStringId paDIONameId) const {
  const CInOutDataConnection *retVal = nullptr;
  TPortId doPortID = getDIOID(paDIONameId);
  if(cgInvalidPortId != doPortID) {
    retVal = const_cast<CFunctionBlock*>(this)->getDIOOutConUnchecked(doPortID);
  }
  return retVal;
}

bool CFunctionBlock::configureGenericDO(TPortId paDOPortId, const CIEC_ANY &paRefValue){
  bool retVal = false;

  if(mInterfaceSpec->m_nNumDOs > paDOPortId){
    CIEC_ANY *dataOutput = getDO(paDOPortId);
    if(dataOutput->getDataTypeID() == CIEC_ANY::e_ANY){
      dataOutput->setValue(paRefValue);
      retVal = true;
    }
  }
  return retVal;
}

CIEC_ANY *CFunctionBlock::getDataOutput(CStringDictionary::TStringId pa_unDONameId) {
  CIEC_ANY *poRetVal = nullptr;
  TPortId unDID = getDOID(pa_unDONameId);

  if(cgInvalidPortId != unDID){
    poRetVal = getDO(unDID);
  }
  return poRetVal;
}

CIEC_ANY *CFunctionBlock::getDataInput(CStringDictionary::TStringId pa_unDINameId) {
  CIEC_ANY *poRetVal = nullptr;
  TPortId unDID = getDIID(pa_unDINameId);

  if(cgInvalidPortId != unDID){
    poRetVal = getDI(unDID);
  }
  return poRetVal;
}

CIEC_ANY* CFunctionBlock::getDIFromPortId(TPortId paDIPortId) {
  CIEC_ANY *retVal = nullptr;
  if(paDIPortId < mInterfaceSpec->m_nNumDIs){
    retVal = getDI(paDIPortId);
  }
  return retVal;
}

CIEC_ANY* CFunctionBlock::getDOFromPortId(TPortId paDOPortId) {
  CIEC_ANY *retVal = nullptr;
  if(paDOPortId < mInterfaceSpec->m_nNumDOs){
    retVal = getDO(paDOPortId);
  }
  return retVal;
}

CIEC_ANY* CFunctionBlock::getDIOFromPortId(TPortId paDIPortId) {
  if(paDIPortId < mInterfaceSpec->mNumDIOs){
    return getDIO(paDIPortId);
  }
  return nullptr;
}

CIEC_ANY *CFunctionBlock::getVar(CStringDictionary::TStringId *paNameList,
    unsigned int paNameListSize){

  if(1 == paNameListSize){
    TPortId portId = getDIID(*paNameList);
    if(cgInvalidPortId != portId){
      return getDI(portId);
    }
    portId = getDOID(*paNameList);
    if(cgInvalidPortId != portId){
        return getDO(portId);
    }
    portId = getDIOID(*paNameList);
    if(cgInvalidPortId != portId){
        return getDIO(portId);
    }
  }
  return nullptr;
}

CAdapter *CFunctionBlock::getAdapter(CStringDictionary::TStringId paAdapterNameId) const{
  TPortId adpPortId = getAdapterPortId(paAdapterNameId);

  if(cgInvalidPortId != adpPortId){
    return m_apoAdapters[adpPortId];
  }
  return nullptr;
}

TPortId CFunctionBlock::getAdapterPortId(CStringDictionary::TStringId paAdapterNameId) const{
  for(TPortId i = 0; i < mInterfaceSpec->m_nNumAdapters; ++i){
    if(m_apoAdapters[i]->getInstanceNameId() == paAdapterNameId){
      return i;
    }
  }
  return cgInvalidPortId;
}

void CFunctionBlock::sendAdapterEvent(TPortId paAdapterID, TEventID paEID, CEventChainExecutionThread * const paECET) const{
  if((paAdapterID < mInterfaceSpec->m_nNumAdapters) && (nullptr != m_apoAdapters[paAdapterID])){
    m_apoAdapters[paAdapterID]->receiveInputEvent(paEID, paECET);
  }
}

bool CFunctionBlock::configureFB(const char *){
  return true;
}

void CFunctionBlock::readInputData(TEventID paEIID) {
  if(nullptr != mInterfaceSpec->m_anEIWithIndexes && scmNoDataAssociated != mInterfaceSpec->m_anEIWithIndexes[paEIID]) {
    const TDataIOID *eiWithStart = &(mInterfaceSpec->m_anEIWith[mInterfaceSpec->m_anEIWithIndexes[paEIID]]);

    // TODO think on this lock
    RES_DATA_CON_CRITICAL_REGION();
    for(size_t i = 0; eiWithStart[i] != scmWithListDelimiter; ++i) {
      TDataIOID nDINum = eiWithStart[i];
      CIEC_ANY *di = getDI(nDINum);
      CDataConnection *conn = *getDIConUnchecked(nDINum);
      readData(nDINum, *di, conn);
    }
  }
}

#ifdef FORTE_TRACE_CTF
void CFunctionBlock::readData(size_t paDINum, CIEC_ANY& paValue, const CDataConnection *const paConn) {
  if(!paConn) {
    return;
  }
#ifdef FORTE_SUPPORT_MONITORING
  if(!paValue.isForced()) {
#endif //FORTE_SUPPORT_MONITORING
    paConn->readData(paValue);
#ifdef FORTE_SUPPORT_MONITORING
  }
#endif //FORTE_SUPPORT_MONITORING
  std::string valueString;
  valueString.reserve(paValue.getToStringBufferSize());
  paValue.toString(valueString.data(), valueString.capacity());
  barectf_default_trace_inputData(mResource->getTracePlatformContext().getContext(),
                                  getFBTypeName() ?: "null",
                                  getInstanceName() ?: "null",
                                  static_cast<uint64_t>(paDINum), valueString.c_str());
}
#endif //FORTE_TRACE_CTF

void CFunctionBlock::writeOutputData(TEventID paEO) {
  if (nullptr != mInterfaceSpec->m_anEOWithIndexes && -1 != mInterfaceSpec->m_anEOWithIndexes[paEO]) {
    const TDataIOID *eiWithStart = &(mInterfaceSpec->m_anEOWith[mInterfaceSpec->m_anEOWithIndexes[paEO]]);
    //TODO think on this lock
    RES_DATA_CON_CRITICAL_REGION();
    for (size_t i = 0; eiWithStart[i] != scmWithListDelimiter; ++i) {
      size_t nDONum = eiWithStart[i];
      CDataConnection *con = getDOConUnchecked(nDONum);
      CIEC_ANY *dataOutput = getDO(nDONum);
      writeData(nDONum, *dataOutput, *con);
    }
  }
}

#ifdef FORTE_TRACE_CTF
void CFunctionBlock::writeData(size_t paDONum, CIEC_ANY& paValue, CDataConnection& paConn) {
	if(paConn.isConnected()) {
#ifdef FORTE_SUPPORT_MONITORING
    if(paValue.isForced() != true) {
#endif //FORTE_SUPPORT_MONITORING
      paConn.writeData(paValue);
#ifdef FORTE_SUPPORT_MONITORING
    } else {
      //when forcing we write back the value from the connection to keep the forced value on the output
      paConn.readData(paValue);
    }
#endif //FORTE_SUPPORT_MONITORING
  }
  std::string valueString;
  valueString.reserve(paValue.getToStringBufferSize());
  paValue.toString(valueString.data(), valueString.capacity());
  barectf_default_trace_outputData(mResource->getTracePlatformContext().getContext(),
                                   getFBTypeName() ?: "null",
                                   getInstanceName() ?: "null",
                                   static_cast<uint64_t>(paDONum), valueString.c_str());
}
#endif //FORTE_TRACE_CTF

void CFunctionBlock::setInitialValues() {
  if(mInterfaceSpec) {
    const CStringDictionary::TStringId *pnDataIds;

    pnDataIds = mInterfaceSpec->m_aunDIDataTypeNames;
    for (TPortId i = 0; i < mInterfaceSpec->m_nNumDIs; ++i) {
      TForteByte *varsData = nullptr;
      CIEC_ANY *value = createDataPoint(pnDataIds, varsData);
      if (value) { getDI(i)->setValue(*value); }
      delete value;
    }

    pnDataIds = mInterfaceSpec->m_aunDODataTypeNames;
    for (TPortId i = 0; i < mInterfaceSpec->m_nNumDOs; ++i) {
      TForteByte *varsData = nullptr;
      CIEC_ANY *value = createDataPoint(pnDataIds, varsData);
      if (value) { getDO(i)->setValue(*value); }
      delete value;
    }
  }
}

EMGMResponse CFunctionBlock::changeFBExecutionState(EMGMCommandType pa_unCommand){
  EMGMResponse nRetVal = EMGMResponse::InvalidState;
  switch (pa_unCommand){
    case EMGMCommandType::Start:
      if((E_FBStates::Idle == mFBState) || (E_FBStates::Stopped == mFBState)){
        mFBState = E_FBStates::Running;
        nRetVal = EMGMResponse::Ready;
      }
      break;
    case EMGMCommandType::Stop:
      if(E_FBStates::Running == mFBState){
        mFBState = E_FBStates::Stopped;
        nRetVal = EMGMResponse::Ready;
      }
      break;
    case EMGMCommandType::Kill:
      if(E_FBStates::Running == mFBState){
        mFBState = E_FBStates::Killed;
        nRetVal = EMGMResponse::Ready;
      }
      break;
    case EMGMCommandType::Reset:
      if((E_FBStates::Stopped == mFBState) || (E_FBStates::Killed == mFBState)){
        mFBState = E_FBStates::Idle;
        nRetVal = EMGMResponse::Ready;
        setInitialValues();
      }
      break;
    default:
      nRetVal = EMGMResponse::InvalidOperation;
      break;
  }

  if(EMGMResponse::Ready == nRetVal && nullptr != mInterfaceSpec) {
    for(TPortId i = 0; i < mInterfaceSpec->m_nNumAdapters; ++i) {
      if(nullptr != m_apoAdapters[i]) {
        m_apoAdapters[i]->changeFBExecutionState(pa_unCommand);
      }
    }
  }
  return nRetVal;
}

EMGMResponse CFunctionBlock::changeFBExecutionStateHelper(const EMGMCommandType paCommand, size_t paAmountOfInternalFBs,
    TFunctionBlockPtr *const paInternalFBs){
  EMGMResponse nRetVal = CFunctionBlock::changeFBExecutionState(paCommand);
  if(EMGMResponse::Ready == nRetVal){
    nRetVal = changeInternalFBExecutionState(paCommand, paAmountOfInternalFBs, paInternalFBs);
  }
  return nRetVal;
}

size_t CFunctionBlock::getDataPointSize(const CStringDictionary::TStringId *&paDataTypeIds) {
  CStringDictionary::TStringId dataTypeId = *paDataTypeIds;
  auto *entry = static_cast<CTypeLib::CDataTypeEntry *>(CTypeLib::findType(dataTypeId,
                                                                           CTypeLib::getDTLibStart()));
  nextDataPoint(paDataTypeIds);
  return nullptr != entry ? entry->getSize() : 0;
}

CIEC_ANY *CFunctionBlock::createDataPoint(const CStringDictionary::TStringId *&paDataTypeIds, TForteByte *&paDataBuf) {
  CStringDictionary::TStringId dataTypeId = *paDataTypeIds;
  CIEC_ANY *poRetVal = CTypeLib::createDataTypeInstance(dataTypeId, paDataBuf);
  if (nullptr != poRetVal) {
    if (g_nStringIdARRAY == dataTypeId) {
      static_cast<CIEC_ARRAY_DYNAMIC *>(poRetVal)->setup(paDataTypeIds + 1);
    }
    paDataBuf += poRetVal->getSizeof();
  }
  nextDataPoint(paDataTypeIds);
  return poRetVal;
}

void CFunctionBlock::nextDataPoint(const CStringDictionary::TStringId *&paDataTypeIds) {
  while(*(paDataTypeIds++) == g_nStringIdARRAY) {
    paDataTypeIds += 2;
  }
}

EMGMResponse CFunctionBlock::changeInternalFBExecutionState(const EMGMCommandType paCommand, const size_t paAmountOfInternalFBs, TFunctionBlockPtr *const paInternalFBs) {
  EMGMResponse nRetVal = EMGMResponse::Ready;
  for (size_t i = 0; ((i < paAmountOfInternalFBs) && (EMGMResponse::Ready == nRetVal)); ++i) {
    if(paInternalFBs[i]) {
      nRetVal = paInternalFBs[i]->changeFBExecutionState(paCommand);
    }
  }
  return nRetVal;
}

size_t CFunctionBlock::calculateFBConnDataSize(const SFBInterfaceSpec &paInterfaceSpec) {
  return sizeof(CEventConnection) * paInterfaceSpec.m_nNumEOs +
         sizeof(TDataConnectionPtr) * paInterfaceSpec.m_nNumDIs +
         sizeof(CDataConnection) * paInterfaceSpec.m_nNumDOs;
}

size_t CFunctionBlock::calculateFBVarsDataSize(const SFBInterfaceSpec &paInterfaceSpec) {
  size_t result = 0;
  const CStringDictionary::TStringId *pnDataIds;

  result += paInterfaceSpec.m_nNumDIs * sizeof(CIEC_ANY *);
  pnDataIds = paInterfaceSpec.m_aunDIDataTypeNames;
  for (TPortId i = 0; i < paInterfaceSpec.m_nNumDIs; ++i) {
    result += getDataPointSize(pnDataIds);
  }

  result += paInterfaceSpec.m_nNumDOs * sizeof(CIEC_ANY *);
  pnDataIds = paInterfaceSpec.m_aunDODataTypeNames;
  for (TPortId i = 0; i < paInterfaceSpec.m_nNumDOs; ++i) {
    result += getDataPointSize(pnDataIds) * 2; // * 2 for connection buffer value
  }

  result += paInterfaceSpec.m_nNumAdapters * sizeof(TAdapterPtr);
  return result;
}

void CFunctionBlock::setupFBInterface(const SFBInterfaceSpec *pa_pstInterfaceSpec) {
  freeAllData();

  mInterfaceSpec = const_cast<SFBInterfaceSpec *>(pa_pstInterfaceSpec);

  if (nullptr != pa_pstInterfaceSpec) {
    size_t connDataSize = calculateFBConnDataSize(*pa_pstInterfaceSpec);
    size_t varsDataSize = calculateFBVarsDataSize(*pa_pstInterfaceSpec);
    mFBConnData = connDataSize ? operator new(connDataSize) : nullptr;
    mFBVarsData = varsDataSize ? operator new(varsDataSize) : nullptr;

    auto *connData = reinterpret_cast<TForteByte *>(mFBConnData);
    auto *varsData = reinterpret_cast<TForteByte *>(mFBVarsData);

    TPortId i;
    if (mInterfaceSpec->m_nNumEOs) {
      mEOConns = reinterpret_cast<CEventConnection *>(connData);

      for (i = 0; i < mInterfaceSpec->m_nNumEOs; ++i) {
        //create an event connection for each event output and initialize its source port
        new(connData)CEventConnection(this, i);
        connData += sizeof(CEventConnection);
      }
    } else {
      mEOConns = nullptr;
    }

    const CStringDictionary::TStringId *pnDataIds;
    if (mInterfaceSpec->m_nNumDIs) {
      mDIConns = reinterpret_cast<TDataConnectionPtr *>(connData);
      connData += sizeof(TDataConnectionPtr) * mInterfaceSpec->m_nNumDIs;

      mDIs = reinterpret_cast<CIEC_ANY **>(varsData);
      varsData += mInterfaceSpec->m_nNumDIs * sizeof(CIEC_ANY *);

      pnDataIds = pa_pstInterfaceSpec->m_aunDIDataTypeNames;
      for (i = 0; i < mInterfaceSpec->m_nNumDIs; ++i) {
        mDIs[i] = createDataPoint(pnDataIds, varsData);
        mDIConns[i] = nullptr;
      }
    } else {
      mDIConns = nullptr;
      mDIs = nullptr;
    }

    if (mInterfaceSpec->m_nNumDOs) {
      //let mDOConns point to the first data output connection
      mDOConns = reinterpret_cast<CDataConnection *>(connData);

      mDOs = reinterpret_cast<CIEC_ANY **>(varsData);
      varsData += mInterfaceSpec->m_nNumDOs * sizeof(CIEC_ANY *);

      pnDataIds = pa_pstInterfaceSpec->m_aunDODataTypeNames;
      for (i = 0; i < mInterfaceSpec->m_nNumDOs; ++i) {
        mDOs[i] = createDataPoint(pnDataIds, varsData);
        CIEC_ANY* connVar = mDOs[i]->clone(varsData);
        varsData += connVar->getSizeof();
        new(connData)CDataConnection(this, i, connVar);
        connData += sizeof(CDataConnection);
      }
    } else {
      mDOConns = nullptr;
      mDOs = nullptr;
    }
    if (mInterfaceSpec->m_nNumAdapters) {
      setupAdapters(pa_pstInterfaceSpec, varsData);
    }

#ifdef FORTE_SUPPORT_MONITORING
    setupEventMonitoringData();
#endif
  }
}

TPortId CFunctionBlock::getPortId(CStringDictionary::TStringId pa_unPortNameId, TPortId pa_unMaxPortNames, const CStringDictionary::TStringId* pa_aunPortNames){
  for(TPortId i = 0; i < pa_unMaxPortNames; ++i){
    if(pa_unPortNameId == pa_aunPortNames[i]){
      return i;
    }
  }
  return cgInvalidPortId;
}

//********************************** below here are monitoring specific functions **********************************************************
#ifdef FORTE_SUPPORT_MONITORING
void CFunctionBlock::setupEventMonitoringData(){
  if(0 != mInterfaceSpec->m_nNumEIs){
    mEIMonitorCount = new TForteUInt32[mInterfaceSpec->m_nNumEIs];
    memset(mEIMonitorCount, 0, sizeof(TForteUInt32) * mInterfaceSpec->m_nNumEIs);
  }

  if(0 != mInterfaceSpec->m_nNumEOs){
    mEOMonitorCount = new TForteUInt32[mInterfaceSpec->m_nNumEOs];
    memset(mEOMonitorCount, 0, sizeof(TForteUInt32) * mInterfaceSpec->m_nNumEOs);
  }
}


CFunctionBlock *CFunctionBlock::getFB(forte::core::TNameIdentifier::CIterator &paNameListIt){
  CFunctionBlock *retVal = nullptr;

  if(paNameListIt.isLastEntry()){
    //only check for adpaters if it we have the last entry in the line
    retVal = getAdapter(*paNameListIt);
  }

  return retVal;
}

TForteUInt32 &CFunctionBlock::getEIMonitorData(TEventID paEIID){
  return mEIMonitorCount[paEIID];
}

TForteUInt32 &CFunctionBlock::getEOMonitorData(TEventID paEOID){
  return mEOMonitorCount[paEOID];
}

#endif //FORTE_SUPPORT_MONITORING

//********************************** below here are CTF Tracing specific functions **********************************************************
#ifdef FORTE_TRACE_CTF
void CFunctionBlock::traceInputEvent(TEventID paEIID){
  barectf_default_trace_receiveInputEvent(mResource->getTracePlatformContext().getContext(),
                                          getFBTypeName() ?: "null",
                                          getInstanceName() ?: "null",
                                          static_cast<uint64_t>(paEIID));
  traceInstanceData();
}

void CFunctionBlock::traceOutputEvent(TEventID paEOID){
  barectf_default_trace_sendOutputEvent(mResource->getTracePlatformContext().getContext(),
                                        getFBTypeName() ?: "null",
                                        getInstanceName() ?: "null",
                                        static_cast<uint64_t>(paEOID));
}

#endif

