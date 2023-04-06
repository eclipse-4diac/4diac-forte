/*******************************************************************************
 * Copyright (c) 2005 - 2018 Profactor GmbH, ACIN, fortiss GmbH,
 *                           Johannes Kepler University
 *               2022 Martin Erich Jobst
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

#ifdef FORTE_TRACE_CTF
#include "trace/barectf_platform_forte.h"
#endif

CFunctionBlock::CFunctionBlock(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec, const CStringDictionary::TStringId pa_nInstanceNameId, TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    m_pstInterfaceSpec(pa_pstInterfaceSpec), m_acFBConnData(pa_acFBConnData), m_acFBVarsData(pa_acFBVarsData),
   mEOConns(nullptr), m_apoDIConns(nullptr), mDOConns(nullptr),
   m_poInvokingExecEnv(nullptr), m_apoAdapters(nullptr), m_poResource(pa_poSrcRes), m_aoDIs(nullptr), m_aoDOs(nullptr), m_nFBInstanceName(pa_nInstanceNameId),
    m_enFBState(E_FBStates::Killed),   //put the FB in the killed state so that reseting it after creation will correctly initialize it
    m_bDeletable(true){

#ifdef FORTE_SUPPORT_MONITORING
  mEIMonitorCount = nullptr;
  mEOMonitorCount = nullptr;
#endif
}

bool CFunctionBlock::initialize() {
  setupFBInterface(m_pstInterfaceSpec, m_acFBConnData, m_acFBVarsData);
  return true;
}

CFunctionBlock::~CFunctionBlock(){
  freeAllData();
}

void CFunctionBlock::freeAllData(){
  if(nullptr != m_pstInterfaceSpec){
    for(int i = 0; i < m_pstInterfaceSpec->m_nNumEOs; ++i){
      (mEOConns + i)->~CEventConnection();
    }

    for(int i = 0; i < m_pstInterfaceSpec->m_nNumDIs; ++i){
      getDI(i)->~CIEC_ANY();
    }

    for(int i = 0; i < m_pstInterfaceSpec->m_nNumDOs; ++i){
      getDO(i)->~CIEC_ANY();
      (mDOConns + i)->~CDataConnection();
    }

    for(unsigned int i = 0; i < m_pstInterfaceSpec->m_nNumAdapters; ++i){
      delete m_apoAdapters[i];
    }
  }

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
    for(unsigned int i = 0; i < pa_pstInterfaceSpec->m_nNumAdapters; ++i) {
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

CEventConnection *CFunctionBlock::getEOConnection(CStringDictionary::TStringId paEONameId) const{
  CEventConnection *retVal = nullptr;
  TPortId portId = getPortId(paEONameId, m_pstInterfaceSpec->m_nNumEOs, m_pstInterfaceSpec->m_aunEONames);
  if(cg_unInvalidPortId != portId){
    retVal = getEOConUnchecked(portId);
  }
  return retVal;
}

bool CFunctionBlock::connectDI(TPortId paDIPortId, CDataConnection *pa_poDataCon){
  bool bRetVal = false;

  if(m_pstInterfaceSpec->m_nNumDIs > paDIPortId){ //catch invalid ID
    if(nullptr == pa_poDataCon){
      m_apoDIConns[paDIPortId] = nullptr;
      bRetVal = true;
    }
    else{
      //only perform connection checks if it is not a disconnection request.
      if(nullptr != m_apoDIConns[paDIPortId]){
        if(m_apoDIConns[paDIPortId] == pa_poDataCon){
          //we have a reconfiguration attempt
          configureGenericDI(paDIPortId, pa_poDataCon->getValue());
          bRetVal = true;
        }else{
          DEVLOG_ERROR("%s cannot connect input data %s to more sources, using the latest connection attempt\n", CStringDictionary::getInstance().get(getFBTypeId()), CStringDictionary::getInstance().get(m_pstInterfaceSpec->m_aunDINames[paDIPortId]));
        }
      }else{
        m_apoDIConns[paDIPortId] = pa_poDataCon;
        configureGenericDI(paDIPortId, pa_poDataCon->getValue());
        bRetVal = true;
      }
    }
  }
  return bRetVal;
}

void CFunctionBlock::configureGenericDI(TPortId paDIPortId, const CIEC_ANY* paRefValue) {
  if(getDI(paDIPortId)->getDataTypeID() == CIEC_ANY::e_ANY && (nullptr != paRefValue)) {
    paRefValue->clone(reinterpret_cast<TForteByte *>(getDI(paDIPortId)));
  }
}

CDataConnection *CFunctionBlock::getDOConnection(CStringDictionary::TStringId paDONameId) const{
  CDataConnection *retVal = nullptr;
  TPortId doPortID = getDOID(paDONameId);
  if(cg_unInvalidPortId != doPortID){
    retVal = mDOConns + doPortID;
  }
  return retVal;
}

CDataConnection *CFunctionBlock::getDIConnection(CStringDictionary::TStringId paDINameId) const{
  CDataConnection *retVal = nullptr;
  TPortId diPortID = getDIID(paDINameId);
  if(cg_unInvalidPortId != diPortID){
    retVal = m_apoDIConns[diPortID];
  }
  return retVal;
}

bool CFunctionBlock::configureGenericDO(TPortId paDOPortId, const CIEC_ANY &paRefValue){
  bool retVal = false;

  if(m_pstInterfaceSpec->m_nNumDOs > paDOPortId){
    CIEC_ANY *dataOutput = getDO(paDOPortId);
    if(dataOutput->getDataTypeID() == CIEC_ANY::e_ANY){
      paRefValue.clone(reinterpret_cast<TForteByte *>(dataOutput));
      retVal = true;
    }
  }
  return retVal;
}

CIEC_ANY *CFunctionBlock::getDataOutput(CStringDictionary::TStringId pa_unDONameId) const {
  CIEC_ANY *poRetVal = nullptr;
  unsigned int unDID = getDOID(pa_unDONameId);

  if(cg_unInvalidPortId != unDID){
    poRetVal = getDO(unDID);
  }
  return poRetVal;
}

CIEC_ANY *CFunctionBlock::getDataInput(CStringDictionary::TStringId pa_unDINameId) const {
  CIEC_ANY *poRetVal = nullptr;
  unsigned int unDID = getDIID(pa_unDINameId);

  if(cg_unInvalidPortId != unDID){
    poRetVal = getDI(unDID);
  }
  return poRetVal;
}

CIEC_ANY* CFunctionBlock::getDIFromPortId(TPortId paDIPortId) const{
  CIEC_ANY *retVal = nullptr;
  if(paDIPortId < m_pstInterfaceSpec->m_nNumDIs){
    retVal = getDI(paDIPortId);
  }
  return retVal;
}

CIEC_ANY* CFunctionBlock::getDOFromPortId(TPortId paDOPortId) const{
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

void CFunctionBlock::sendOutputEvent(size_t paEO){
  FORTE_TRACE("OutputEvent: Function Block sending event: %d (maxid: %d)\n", paEO, m_pstInterfaceSpec->m_nNumEOs - 1);

#ifdef FORTE_TRACE_CTF
  barectf_default_trace_sendOutputEvent(m_poResource->getTracePlatformContext().getContext(),
                                        CStringDictionary::getInstance().get(m_nFBInstanceName) ?: "null",
                                        static_cast<uint64_t>(paEO));
#endif

  if(paEO < m_pstInterfaceSpec->m_nNumEOs) {
    if(nullptr != m_pstInterfaceSpec->m_anEOWithIndexes && -1 != m_pstInterfaceSpec->m_anEOWithIndexes[paEO]) {
      const TDataIOID *eiWithStart = &(m_pstInterfaceSpec->m_anEOWith[m_pstInterfaceSpec->m_anEOWithIndexes[paEO]]);
      //TODO think on this lock
      CCriticalRegion criticalRegion(m_poResource->m_oResDataConSync);
      for(size_t i = 0; eiWithStart[i] != scmWithListDelimiter; ++i) {
        CDataConnection *con = getDOConUnchecked(eiWithStart[i]);
        CIEC_ANY *dataOutput = getDO(eiWithStart[i]);
        if(con->isConnected()) {
#ifdef FORTE_SUPPORT_MONITORING
          if(dataOutput->isForced() != true) {
#endif //FORTE_SUPPORT_MONITORING
            con->writeData(dataOutput);
#ifdef FORTE_SUPPORT_MONITORING
          } else {
            //when forcing we write back the value from the connection to keep the forced value on the output
            con->readData(dataOutput);
          }
#endif //FORTE_SUPPORT_MONITORING
        }
#ifdef FORTE_TRACE_CTF
        std::string valueString;
        valueString.reserve(dataOutput->getToStringBufferSize());
        dataOutput->toString(valueString.data(), valueString.capacity());
        barectf_default_trace_outputData(m_poResource->getTracePlatformContext().getContext(),
                                         CStringDictionary::getInstance().get(m_nFBInstanceName) ?: "null",
                                         static_cast<uint64_t>(eiWithStart[i]), valueString.c_str());
#endif //FORTE_TRACE_CTF
      }
    }

    getEOConUnchecked(static_cast<TPortId>(paEO))->triggerEvent(m_poInvokingExecEnv);

#ifdef FORTE_SUPPORT_MONITORING
    // Count Event for monitoring
    mEOMonitorCount[paEO]++;
#endif //FORTE_SUPPORT_MONITORING
  }
}

void CFunctionBlock::sendAdapterEvent(size_t paAdapterID, size_t paEID) const{
  if((paAdapterID < m_pstInterfaceSpec->m_nNumAdapters) && (nullptr != m_apoAdapters[paAdapterID])){
    m_apoAdapters[paAdapterID]->receiveInputEvent(paEID, m_poInvokingExecEnv);
  }
}

bool CFunctionBlock::configureFB(const char *){
  return true;
}

void CFunctionBlock::receiveInputEvent(size_t paEIID, CEventChainExecutionThread *paExecEnv){
  FORTE_TRACE("InputEvent: Function Block (%s) got event: %d (maxid: %d)\n", CStringDictionary::getInstance().get(getInstanceNameId()), paEIID, m_pstInterfaceSpec->m_nNumEIs - 1);

#ifdef FORTE_TRACE_CTF
  barectf_default_trace_receiveInputEvent(m_poResource->getTracePlatformContext().getContext(),
                                          CStringDictionary::getInstance().get(m_nFBInstanceName) ?: "null",
                                          static_cast<uint64_t>(paEIID));

  traceInstanceData();
#endif

  if(E_FBStates::Running == getState()){
    if(paEIID < m_pstInterfaceSpec->m_nNumEIs) {
      if(nullptr != m_pstInterfaceSpec->m_anEIWithIndexes && -1 != m_pstInterfaceSpec->m_anEIWithIndexes[paEIID]) {
        const TDataIOID *eiWithStart = &(m_pstInterfaceSpec->m_anEIWith[m_pstInterfaceSpec->m_anEIWithIndexes[paEIID]]);

        // TODO think on this lock
        CCriticalRegion criticalRegion(m_poResource->m_oResDataConSync);
        for(size_t i = 0; eiWithStart[i] != scmWithListDelimiter; ++i) {
          if(nullptr != m_apoDIConns[eiWithStart[i]]) {
            CIEC_ANY *di = getDI(eiWithStart[i]);
#ifdef FORTE_SUPPORT_MONITORING
            if(true != di->isForced()) {
#endif //FORTE_SUPPORT_MONITORING
              m_apoDIConns[eiWithStart[i]]->readData(di);
#ifdef FORTE_SUPPORT_MONITORING
            }
#endif //FORTE_SUPPORT_MONITORING
#ifdef FORTE_TRACE_CTF
            std::string valueString;
            valueString.reserve(di->getToStringBufferSize());
            di->toString(valueString.data(), valueString.capacity());
            barectf_default_trace_inputData(m_poResource->getTracePlatformContext().getContext(),
                                            CStringDictionary::getInstance().get(m_nFBInstanceName) ?: "null",
                                            static_cast<uint64_t>(eiWithStart[i]), valueString.c_str());
#endif //FORTE_TRACE_CTF
          }
        }
      }
#ifdef FORTE_SUPPORT_MONITORING
      // Count Event for monitoring
      mEIMonitorCount[paEIID]++;
#endif //FORTE_SUPPORT_MONITORING
    }
    m_poInvokingExecEnv = paExecEnv;
    executeEvent(paEIID);
  }
}

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

CIEC_ANY *CFunctionBlock::createDataPoint(const CStringDictionary::TStringId **pa_panDataTypeIds, TForteByte *pa_acDataBuf){
  CIEC_ANY *poRetVal = CTypeLib::createDataTypeInstance(**pa_panDataTypeIds, pa_acDataBuf);
  ++(*pa_panDataTypeIds);
#ifdef FORTE_SUPPORT_ARRAYS
  if(g_nStringIdARRAY == (*pa_panDataTypeIds)[-1]){
    if(nullptr != poRetVal){
      //For an array we have to do more
      (static_cast<CIEC_ARRAY_TYPELIB *>(poRetVal))->setup(static_cast<TForteUInt16>(**pa_panDataTypeIds), (*pa_panDataTypeIds)[1]);
    }
    (*pa_panDataTypeIds) += 2;
  }
#endif
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

void CFunctionBlock::setupFBInterface(const SFBInterfaceSpec *pa_pstInterfaceSpec, TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData){
  m_pstInterfaceSpec = const_cast<SFBInterfaceSpec *>(pa_pstInterfaceSpec);

  if(nullptr != pa_pstInterfaceSpec){
    if((nullptr != pa_acFBConnData) && (nullptr != pa_acFBVarsData)){
      TPortId i;
      if(m_pstInterfaceSpec->m_nNumEOs){
        mEOConns = reinterpret_cast<CEventConnection *>(pa_acFBConnData);

        for(i = 0; i < m_pstInterfaceSpec->m_nNumEOs; ++i){
          //create an event connection for each event output and initialize its source port
          new (pa_acFBConnData)CEventConnection(this, i);
          pa_acFBConnData += sizeof(CEventConnection);
        }
      }
      else{
        mEOConns = nullptr;
      }

      const CStringDictionary::TStringId *pnDataIds;
      if(m_pstInterfaceSpec->m_nNumDIs){
        m_apoDIConns = reinterpret_cast<TDataConnectionPtr *>(pa_acFBConnData);
        pa_acFBConnData += sizeof(TDataConnectionPtr) * m_pstInterfaceSpec->m_nNumDIs;

        //let m_aoDIs point to the first data input
        m_aoDIs = reinterpret_cast<CIEC_ANY *>(pa_acFBVarsData);

        pnDataIds = pa_pstInterfaceSpec->m_aunDIDataTypeNames;
        for(i = 0; i < m_pstInterfaceSpec->m_nNumDIs; ++i){
          m_apoDIConns[i] = nullptr;
          createDataPoint(&pnDataIds, pa_acFBVarsData);
          pa_acFBVarsData += sizeof(CIEC_ANY);
        }
      }
      else{
        m_apoDIConns = nullptr;
        m_aoDIs = nullptr;
      }

      if(m_pstInterfaceSpec->m_nNumDOs){
        //let mDOConns point to the first data output connection
        mDOConns = reinterpret_cast<CDataConnection *>(pa_acFBConnData);

        //let m_aoDIs point to the first data output
        m_aoDOs = reinterpret_cast<CIEC_ANY *>(pa_acFBVarsData);

        pnDataIds = pa_pstInterfaceSpec->m_aunDODataTypeNames;
        for(i = 0; i < m_pstInterfaceSpec->m_nNumDOs; ++i){
          createDataPoint(&pnDataIds, pa_acFBVarsData);
          pa_acFBVarsData += sizeof(CIEC_ANY);
          //create an data connection for each data output and initialize its source port
          new (pa_acFBConnData)CDataConnection(this, i, getDO(i));
          pa_acFBConnData += sizeof(CDataConnection);
        }
      }
      else{
        mDOConns = nullptr;
        m_aoDOs = nullptr;
      }
      if(m_pstInterfaceSpec->m_nNumAdapters){
        setupAdapters(pa_pstInterfaceSpec, pa_acFBVarsData);
      }
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
