/*******************************************************************************
 * Copyright (c) 2005 - 2018 Profactor GmbH, ACIN, fortiss GmbH,
 *                           Johannes Kepler University
 *               2022 - 2023 Martin Erich Jobst
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
        m_pstInterfaceSpec(pa_pstInterfaceSpec),
        mEOConns(nullptr), mDIConns(nullptr), mDOConns(nullptr), mDIs(nullptr), mDOs(nullptr),
        m_poInvokingExecEnv(nullptr), m_apoAdapters(nullptr),
        mFBConnData(nullptr), mFBVarsData(nullptr),
        m_poResource(pa_poSrcRes), m_Container(nullptr),
#ifdef FORTE_SUPPORT_MONITORING
        mEOMonitorCount(nullptr), mEIMonitorCount(nullptr),
#endif
        m_nFBInstanceName(pa_nInstanceNameId),
        m_enFBState(
                E_FBStates::Killed),   //put the FB in the killed state so that reseting it after creation will correctly initialize it
        m_bDeletable(true) {
}

bool CFunctionBlock::initialize() {
  setupFBInterface(m_pstInterfaceSpec);
  return true;
}

CFunctionBlock::~CFunctionBlock(){
  freeAllData();
}

void CFunctionBlock::freeAllData(){
  if(nullptr != m_pstInterfaceSpec){
    if(nullptr != mEOConns) {
      std::destroy_n(mEOConns, m_pstInterfaceSpec->m_nNumEOs);
    }

    if(nullptr != mDOConns) {
      for (int i = 0; i < m_pstInterfaceSpec->m_nNumDOs; ++i) {
        if(CIEC_ANY* value = mDOConns[i].getValue(); nullptr != value) {
          std::destroy_at(value);
        }
      }
      std::destroy_n(mDOConns, m_pstInterfaceSpec->m_nNumDOs);
    }

    if(nullptr != mDIs) {
      for (int i = 0; i < m_pstInterfaceSpec->m_nNumDIs; ++i) {
        if(CIEC_ANY* value = mDIs[i]; nullptr != value) {
          std::destroy_at(value);
        }
      }
    }

    if(nullptr != mDOs) {
      for (int i = 0; i < m_pstInterfaceSpec->m_nNumDOs; ++i) {
        if(CIEC_ANY* value = mDOs[i]; nullptr != value) {
          std::destroy_at(value);
        }
      }
    }

    if(nullptr != m_apoAdapters) {
      for (unsigned int i = 0; i < m_pstInterfaceSpec->m_nNumAdapters; ++i) {
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
  return m_poResource->getDevice().getTimer();
}

CEventConnection *CFunctionBlock::getEOConnection(CStringDictionary::TStringId paEONameId) {
  CEventConnection *retVal = nullptr;
  TPortId portId = getPortId(paEONameId, m_pstInterfaceSpec->m_nNumEOs, m_pstInterfaceSpec->m_aunEONames);
  if(cg_unInvalidPortId != portId){
    retVal = getEOConUnchecked(portId);
  }
  return retVal;
}

const CEventConnection *CFunctionBlock::getEOConnection(CStringDictionary::TStringId paEONameId) const {
  const CEventConnection *retVal = nullptr;
  TPortId portId = getPortId(paEONameId, m_pstInterfaceSpec->m_nNumEOs, m_pstInterfaceSpec->m_aunEONames);
  if(cg_unInvalidPortId != portId){
    retVal = const_cast<CFunctionBlock*>(this)->getEOConUnchecked(portId);
  }
  return retVal;
}

bool CFunctionBlock::connectDI(TPortId paDIPortId, CDataConnection *paDataCon){
  bool bRetVal = false;

  if(m_pstInterfaceSpec->m_nNumDIs > paDIPortId){ //catch invalid ID
    if(nullptr == paDataCon){
      *getDIConUnchecked(paDIPortId) = nullptr;
      bRetVal = true;
    }
    else{
      //only perform connection checks if it is not a disconnection request.
      CDataConnection *conn = *getDIConUnchecked(paDIPortId);
      if(nullptr != conn){
        if(conn == paDataCon){
          //we have a reconfiguration attempt
          configureGenericDI(paDIPortId, paDataCon->getValue());
          bRetVal = true;
        }else{
          DEVLOG_ERROR("%s cannot connect input data %s to more sources, using the latest connection attempt\n", getInstanceName(), CStringDictionary::getInstance().get(m_pstInterfaceSpec->m_aunDINames[paDIPortId]));
        }
      }else{
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

CDataConnection *CFunctionBlock::getDIConnection(CStringDictionary::TStringId paDINameId) {
  CDataConnection *retVal = nullptr;
  TPortId doPortID = getDIID(paDINameId);
  if(cg_unInvalidPortId != doPortID){
    retVal = *getDIConUnchecked(doPortID);
  }
  return retVal;
}

const CDataConnection *CFunctionBlock::getDIConnection(CStringDictionary::TStringId paDINameId) const {
  const CDataConnection *retVal = nullptr;
  TPortId doPortID = getDIID(paDINameId);
  if(cg_unInvalidPortId != doPortID){
    retVal = *const_cast<CFunctionBlock*>(this)->getDIConUnchecked(doPortID);
  }
  return retVal;
}

CDataConnection *CFunctionBlock::getDOConnection(CStringDictionary::TStringId paDONameId) {
  CDataConnection *retVal = nullptr;
  TPortId doPortID = getDOID(paDONameId);
  if(cg_unInvalidPortId != doPortID){
    retVal = getDOConUnchecked(doPortID);
  }
  return retVal;
}

const CDataConnection *CFunctionBlock::getDOConnection(CStringDictionary::TStringId paDONameId) const {
  const CDataConnection *retVal = nullptr;
  TPortId doPortID = getDOID(paDONameId);
  if(cg_unInvalidPortId != doPortID){
    retVal = const_cast<CFunctionBlock*>(this)->getDOConUnchecked(doPortID);
  }
  return retVal;
}

bool CFunctionBlock::configureGenericDO(TPortId paDOPortId, const CIEC_ANY &paRefValue){
  bool retVal = false;

  if(m_pstInterfaceSpec->m_nNumDOs > paDOPortId){
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
  unsigned int unDID = getDOID(pa_unDONameId);

  if(cg_unInvalidPortId != unDID){
    poRetVal = getDO(unDID);
  }
  return poRetVal;
}

CIEC_ANY *CFunctionBlock::getDataInput(CStringDictionary::TStringId pa_unDINameId) {
  CIEC_ANY *poRetVal = nullptr;
  unsigned int unDID = getDIID(pa_unDINameId);

  if(cg_unInvalidPortId != unDID){
    poRetVal = getDI(unDID);
  }
  return poRetVal;
}

CIEC_ANY* CFunctionBlock::getDIFromPortId(TPortId paDIPortId) {
  CIEC_ANY *retVal = nullptr;
  if(paDIPortId < m_pstInterfaceSpec->m_nNumDIs){
    retVal = getDI(paDIPortId);
  }
  return retVal;
}

CIEC_ANY* CFunctionBlock::getDOFromPortId(TPortId paDOPortId) {
  CIEC_ANY *retVal = nullptr;
  if(paDOPortId < m_pstInterfaceSpec->m_nNumDOs){
    retVal = getDO(paDOPortId);
  }
  return retVal;
}

CIEC_ANY *CFunctionBlock::getVar(CStringDictionary::TStringId *paNameList,
    unsigned int paNameListSize){

  CIEC_ANY *poRetVal = nullptr;
  if(1 == paNameListSize){
    unsigned int portId = getDIID(*paNameList);
    if(cg_unInvalidPortId != portId){
      poRetVal = getDI(portId);
    }
    else{
      portId = getDOID(*paNameList);
      if(cg_unInvalidPortId != portId){
        poRetVal = getDO(portId);
      }
    }
  }
  return poRetVal;
}

CAdapter *CFunctionBlock::getAdapter(CStringDictionary::TStringId paAdapterNameId) const{
  TPortId adpPortId = getAdapterPortId(paAdapterNameId);

  if(cg_unInvalidPortId != adpPortId){
    return m_apoAdapters[adpPortId];
  }
  return nullptr;
}

TPortId CFunctionBlock::getAdapterPortId(CStringDictionary::TStringId paAdapterNameId) const{
  for(TPortId i = 0; i < m_pstInterfaceSpec->m_nNumAdapters; ++i){
    if(m_apoAdapters[i]->getInstanceNameId() == paAdapterNameId){
      return i;
    }
  }
  return cg_unInvalidPortId;
}

void CFunctionBlock::sendAdapterEvent(size_t paAdapterID, TEventID paEID) const{
  if((paAdapterID < m_pstInterfaceSpec->m_nNumAdapters) && (nullptr != m_apoAdapters[paAdapterID])){
    m_apoAdapters[paAdapterID]->receiveInputEvent(paEID, m_poInvokingExecEnv);
  }
}

bool CFunctionBlock::configureFB(const char *){
  return true;
}

void CFunctionBlock::readInputData(TEventID paEIID) {
  if(nullptr != m_pstInterfaceSpec->m_anEIWithIndexes && -1 != m_pstInterfaceSpec->m_anEIWithIndexes[paEIID]) {
    const TDataIOID *eiWithStart = &(m_pstInterfaceSpec->m_anEIWith[m_pstInterfaceSpec->m_anEIWithIndexes[paEIID]]);

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
  barectf_default_trace_inputData(m_poResource->getTracePlatformContext().getContext(),
          CStringDictionary::getInstance().get(m_nFBInstanceName) ?: "null",
          static_cast<uint64_t>(paDINum), valueString.c_str());
}
#endif //FORTE_TRACE_CTF

void CFunctionBlock::writeOutputData(TEventID paEO) {
  if (nullptr != m_pstInterfaceSpec->m_anEOWithIndexes && -1 != m_pstInterfaceSpec->m_anEOWithIndexes[paEO]) {
    const TDataIOID *eiWithStart = &(m_pstInterfaceSpec->m_anEOWith[m_pstInterfaceSpec->m_anEOWithIndexes[paEO]]);
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
      paConn.writeData(pa.Value);
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
  barectf_default_trace_outputData(m_poResource->getTracePlatformContext().getContext(),
                                   CStringDictionary::getInstance().get(m_nFBInstanceName) ?: "null",
                                   static_cast<uint64_t>(paDONum), valueString.c_str());
}
#endif //FORTE_TRACE_CTF

EMGMResponse CFunctionBlock::changeFBExecutionState(EMGMCommandType pa_unCommand){
  EMGMResponse nRetVal = EMGMResponse::InvalidState;
  switch (pa_unCommand){
    case EMGMCommandType::Start:
      if((E_FBStates::Idle == m_enFBState) || (E_FBStates::Stopped == m_enFBState)){
        m_enFBState = E_FBStates::Running;
        nRetVal = EMGMResponse::Ready;
      }
      break;
    case EMGMCommandType::Stop:
      if(E_FBStates::Running == m_enFBState){
        m_enFBState = E_FBStates::Stopped;
        nRetVal = EMGMResponse::Ready;
      }
      break;
    case EMGMCommandType::Kill:
      if(E_FBStates::Running == m_enFBState){
        m_enFBState = E_FBStates::Killed;
        nRetVal = EMGMResponse::Ready;
      }
      break;
    case EMGMCommandType::Reset:
      if((E_FBStates::Stopped == m_enFBState) || (E_FBStates::Killed == m_enFBState)){
        m_enFBState = E_FBStates::Idle;
        nRetVal = EMGMResponse::Ready;
        setInitialValues();
      }
      break;
    default:
      nRetVal = EMGMResponse::InvalidOperation;
      break;
  }

  if(EMGMResponse::Ready == nRetVal && nullptr != m_pstInterfaceSpec) {
    for(int i = 0; i < m_pstInterfaceSpec->m_nNumAdapters; ++i) {
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

size_t CFunctionBlock::getDataPointSize(const CStringDictionary::TStringId *&pa_panDataTypeIds) {
  CStringDictionary::TStringId dataTypeId = *(pa_panDataTypeIds++);
  auto *entry = static_cast<CTypeLib::CDataTypeEntry *>(CTypeLib::findType(dataTypeId,
                                                                           CTypeLib::getDTLibStart()));
  if (g_nStringIdARRAY == dataTypeId) {
    pa_panDataTypeIds += 2;
  }
  return nullptr != entry ? entry->getSize() : 0;
}

CIEC_ANY *CFunctionBlock::createDataPoint(const CStringDictionary::TStringId *&paDataTypeIds, TForteByte *&paDataBuf){
  CStringDictionary::TStringId dataTypeId = *(paDataTypeIds++);
  CIEC_ANY *poRetVal = CTypeLib::createDataTypeInstance(dataTypeId, paDataBuf);
  if(nullptr != poRetVal) {
    paDataBuf += poRetVal->getSizeof();
  }
  if(g_nStringIdARRAY == dataTypeId){
    if(nullptr != poRetVal){
      //For an array we have to do more
      (static_cast<CIEC_ARRAY_DYNAMIC *>(poRetVal))->setup(static_cast<TForteUInt16>(*paDataTypeIds), paDataTypeIds[1]);
    }
    paDataTypeIds += 2;
  }
  return poRetVal;
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
  for (int i = 0; i < paInterfaceSpec.m_nNumDIs; ++i) {
    result += getDataPointSize(pnDataIds);
  }

  result += paInterfaceSpec.m_nNumDOs * sizeof(CIEC_ANY *);
  pnDataIds = paInterfaceSpec.m_aunDODataTypeNames;
  for (int i = 0; i < paInterfaceSpec.m_nNumDOs; ++i) {
    result += getDataPointSize(pnDataIds) * 2; // * 2 for connection buffer value
  }

  result += paInterfaceSpec.m_nNumAdapters * sizeof(TAdapterPtr);
  return result;
}

void CFunctionBlock::setupFBInterface(const SFBInterfaceSpec *pa_pstInterfaceSpec) {
  freeAllData();

  m_pstInterfaceSpec = const_cast<SFBInterfaceSpec *>(pa_pstInterfaceSpec);

  if (nullptr != pa_pstInterfaceSpec) {
    size_t connDataSize = calculateFBConnDataSize(*pa_pstInterfaceSpec);
    size_t varsDataSize = calculateFBVarsDataSize(*pa_pstInterfaceSpec);
    mFBConnData = connDataSize ? operator new(connDataSize) : nullptr;
    mFBVarsData = varsDataSize ? operator new(varsDataSize) : nullptr;

    auto *connData = reinterpret_cast<TForteByte *>(mFBConnData);
    auto *varsData = reinterpret_cast<TForteByte *>(mFBVarsData);

    TPortId i;
    if (m_pstInterfaceSpec->m_nNumEOs) {
      mEOConns = reinterpret_cast<CEventConnection *>(connData);

      for (i = 0; i < m_pstInterfaceSpec->m_nNumEOs; ++i) {
        //create an event connection for each event output and initialize its source port
        new(connData)CEventConnection(this, i);
        connData += sizeof(CEventConnection);
      }
    } else {
      mEOConns = nullptr;
    }

    const CStringDictionary::TStringId *pnDataIds;
    if (m_pstInterfaceSpec->m_nNumDIs) {
      mDIConns = reinterpret_cast<TDataConnectionPtr *>(connData);
      connData += sizeof(TDataConnectionPtr) * m_pstInterfaceSpec->m_nNumDIs;

      mDIs = reinterpret_cast<CIEC_ANY **>(varsData);
      varsData += m_pstInterfaceSpec->m_nNumDIs * sizeof(CIEC_ANY *);

      pnDataIds = pa_pstInterfaceSpec->m_aunDIDataTypeNames;
      for (i = 0; i < m_pstInterfaceSpec->m_nNumDIs; ++i) {
        mDIs[i] = createDataPoint(pnDataIds, varsData);
        mDIConns[i] = nullptr;
      }
    } else {
      mDIConns = nullptr;
      mDIs = nullptr;
    }

    if (m_pstInterfaceSpec->m_nNumDOs) {
      //let mDOConns point to the first data output connection
      mDOConns = reinterpret_cast<CDataConnection *>(connData);

      mDOs = reinterpret_cast<CIEC_ANY **>(varsData);
      varsData += m_pstInterfaceSpec->m_nNumDOs * sizeof(CIEC_ANY *);

      pnDataIds = pa_pstInterfaceSpec->m_aunDODataTypeNames;
      for (i = 0; i < m_pstInterfaceSpec->m_nNumDOs; ++i) {
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
    if (m_pstInterfaceSpec->m_nNumAdapters) {
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
  return cg_unInvalidPortId;
}

//********************************** below here are monitoring specific functions **********************************************************
#ifdef FORTE_SUPPORT_MONITORING
void CFunctionBlock::setupEventMonitoringData(){
  if(0 != m_pstInterfaceSpec->m_nNumEIs){
    mEIMonitorCount = new TForteUInt32[m_pstInterfaceSpec->m_nNumEIs];
    memset(mEIMonitorCount, 0, sizeof(TForteUInt32) * m_pstInterfaceSpec->m_nNumEIs);
  }

  if(0 != m_pstInterfaceSpec->m_nNumEOs){
    mEOMonitorCount = new TForteUInt32[m_pstInterfaceSpec->m_nNumEOs];
    memset(mEOMonitorCount, 0, sizeof(TForteUInt32) * m_pstInterfaceSpec->m_nNumEOs);
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
  barectf_default_trace_receiveInputEvent(m_poResource->getTracePlatformContext().getContext(),
                                          CStringDictionary::getInstance().get(m_nFBInstanceName) ?: "null",
                                          static_cast<uint64_t>(paEIID));
  traceInstanceData();
}

void CFunctionBlock::traceOutputEvent(TEventID paEOID){
  barectf_default_trace_sendOutputEvent(m_poResource->getTracePlatformContext().getContext(),
                                        CStringDictionary::getInstance().get(m_nFBInstanceName) ?: "null",
                                        static_cast<uint64_t>(paEOID));
}

#endif

