/*******************************************************************************
  * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Gunnar Grabmaier, Alois Zoitl, Smodic Rene, Ingo Hegny,
  *    Gerhard Ebenhofer, Michael Hofmann, Martin Melik Merkumians, Monika Wenger, 
  *    Matthias Plasch
  *      - initial implementation and rework communication infrastructure
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

CFunctionBlock::CFunctionBlock(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec, const CStringDictionary::TStringId pa_nInstanceNameId, TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
   mEOConns(0), m_apoDIConns(0), mDOConns(0),
    m_apoAdapters(0), m_poResource(pa_poSrcRes), m_aoDIs(0), m_aoDOs(0), m_nFBInstanceName(pa_nInstanceNameId), 
    m_enFBState(e_KILLED),   //put the FB in the killed state so that reseting it after creation will correctly initialize it
    m_bDeletable(true){

  #ifdef FORTE_SUPPORT_MONITORING
  m_nEIMonitorCount = 0;
  m_nEOMonitorCount = 0;
  mContainer = 0;
#endif

  setupFBInterface(pa_pstInterfaceSpec, pa_acFBConnData, pa_acFBVarsData, false);
}

CFunctionBlock::~CFunctionBlock(){
  freeAllData();
}

void CFunctionBlock::freeAllData(){
  if(0 != m_pstInterfaceSpec){
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

  if(m_bManagesFBData){
    TForteByte *acFBConnData = 0;
    TForteByte *acFBVarsData = 0;

    if(0 != mEOConns){
      //if not 0 m_apoEOConns points to the start of the connection data array
      acFBConnData = reinterpret_cast<TForteByte *>(mEOConns);
    }
    else{
      if(0 != m_apoDIConns){
        //if not 0 m_apoDIConns points to the start of the connection data array
        acFBConnData = reinterpret_cast<TForteByte *>(m_apoDIConns);
      }
      else{
        if(0 != mDOConns){
          //if not 0 m_apoDOConns points to the start of the connection data array
          acFBConnData = reinterpret_cast<TForteByte *>(mDOConns);
        }
      }
    }

    if(0 != m_aoDIs){
      //if not 0 m_aoDIs points to the start of the vars data array
      acFBVarsData = reinterpret_cast<TForteByte *>(m_aoDIs);
    }
    else{
      if(0 != m_aoDOs){
        //if not 0 m_aoDOs points to the start of the vars data array
        acFBVarsData = reinterpret_cast<TForteByte *>(m_aoDOs);
      }
      else{
        if(0 != m_apoAdapters){
          //if not 0 m_apoAdapters points to the start of the vars data array
          acFBVarsData = reinterpret_cast<TForteByte *>(m_apoAdapters);
        }
      }
    }

    delete[] acFBConnData;
    delete[] acFBVarsData;
    delete m_pstInterfaceSpec;
  }
#ifdef  FORTE_SUPPORT_MONITORING
  delete[] m_nEOMonitorCount;
  delete[] m_nEIMonitorCount;
#endif //FORTE_SUPPORT_MONITORING
}

void CFunctionBlock::setupAdapters(const SFBInterfaceSpec *pa_pstInterfaceSpec, TForteByte *pa_acFBData){
  if((0 != pa_pstInterfaceSpec) && (0 != pa_acFBData)){
    if(pa_pstInterfaceSpec->m_nNumAdapters){
      m_apoAdapters = reinterpret_cast<TAdapterPtr *>(pa_acFBData);
      for(unsigned int i = 0; i < pa_pstInterfaceSpec->m_nNumAdapters; ++i){
        //set pointer to right place in pa_acFBData
        m_apoAdapters[i] = CTypeLib::createAdapter(pa_pstInterfaceSpec->m_pstAdapterInstanceDefinition[i].m_nAdapterNameID, pa_pstInterfaceSpec->m_pstAdapterInstanceDefinition[i].m_nAdapterTypeNameID, getResourcePtr(), pa_pstInterfaceSpec->m_pstAdapterInstanceDefinition[i].m_bIsPlug);
        if(0 != m_apoAdapters[i]){
          m_apoAdapters[i]->setParentFB(this, static_cast<TForteUInt8>(i));
#ifdef FORTE_SUPPORT_MONITORING
          m_apoAdapters[i]->setContainer(this);
#endif
        }
      }
    }
  }
}

CTimerHandler& CFunctionBlock::getTimer(void){
  return m_poResource->getDevice().getTimer();
}

CEventConnection *CFunctionBlock::getEOConection(CStringDictionary::TStringId paEONameId) const{
  CEventConnection *retVal = 0;
  TPortId portId = getPortId(paEONameId, m_pstInterfaceSpec->m_nNumEOs, m_pstInterfaceSpec->m_aunEONames);
  if(cg_unInvalidPortId != portId){
    retVal = getEOConUnchecked(portId);
  }
  return retVal;
}

bool CFunctionBlock::connectDI(TPortId paDIPortId, CDataConnection *pa_poDataCon){
  bool bRetVal = false;

  if(m_pstInterfaceSpec->m_nNumDIs > paDIPortId){ //catch invalid ID
    if(0 == pa_poDataCon){
      m_apoDIConns[paDIPortId] = 0;
      bRetVal = true;
    }
    else{
      //only perform connection checks if it is not a disconnection request.
      if(0 != m_apoDIConns[paDIPortId]){
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

void CFunctionBlock::configureGenericDI(TPortId paDIPortId, const CIEC_ANY *paRefValue){
  if(getDI(paDIPortId)->getDataTypeID() == CIEC_ANY::e_ANY){
    if(0 != paRefValue){
      paRefValue->clone(reinterpret_cast<TForteByte *>(getDI(paDIPortId)));
    }
  }
}

CDataConnection *CFunctionBlock::getDOConnection(CStringDictionary::TStringId paDONameId) const{
  CDataConnection *retVal = 0;
  TPortId doPortID = getDOID(paDONameId);
  if(cg_unInvalidPortId != doPortID){
    retVal = mDOConns + doPortID;
  }
  return retVal;
}

CDataConnection *CFunctionBlock::getDIConnection(CStringDictionary::TStringId paDINameId) const{
  CDataConnection *retVal = 0;
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
  CIEC_ANY *poRetVal = 0;
  unsigned int unDID = getDOID(pa_unDONameId);

  if(cg_unInvalidPortId != unDID){
    poRetVal = getDO(unDID);
  }
  return poRetVal;
}

CIEC_ANY *CFunctionBlock::getDataInput(CStringDictionary::TStringId pa_unDINameId) const {
  CIEC_ANY *poRetVal = 0;
  unsigned int unDID = getDIID(pa_unDINameId);

  if(cg_unInvalidPortId != unDID){
    poRetVal = getDI(unDID);
  }
  return poRetVal;
}

CIEC_ANY* CFunctionBlock::getDIFromPortId(TPortId paDIPortId) const{
  CIEC_ANY *retVal = 0;
  if(paDIPortId < m_pstInterfaceSpec->m_nNumDIs){
    retVal = getDI(paDIPortId);
  }
  return retVal;
}

CIEC_ANY* CFunctionBlock::getDOFromPortId(TPortId paDOPortId) const{
  CIEC_ANY *retVal = 0;
  if(paDOPortId < m_pstInterfaceSpec->m_nNumDOs){
    retVal = getDO(paDOPortId);
  }
  return retVal;
}

CIEC_ANY *CFunctionBlock::getVar(CStringDictionary::TStringId *paNameList,
    unsigned int paNameListSize){

  CIEC_ANY *poRetVal = 0;
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

CAdapter *CFunctionBlock::getAdapter(CStringDictionary::TStringId paAdapterNameId){
  TPortId adpPortId = getAddapterPortId(paAdapterNameId);

  if(cg_unInvalidPortId != adpPortId){
    return m_apoAdapters[adpPortId];
  }
  return 0;
}

TPortId CFunctionBlock::getAddapterPortId(CStringDictionary::TStringId paAdapterNameId){
  for(TPortId i = 0; i < m_pstInterfaceSpec->m_nNumAdapters; ++i){
    if(m_apoAdapters[i]->getInstanceNameId() == paAdapterNameId){
      return i;
    }
  }
  return cg_unInvalidPortId;
}

void CFunctionBlock::sendOutputEvent(int pa_nEO){
  FORTE_TRACE("OutputEvent: Function Block sending event: %d (maxid: %d)\n", pa_nEO, m_pstInterfaceSpec->m_nNumEOs - 1);
  if(pa_nEO < m_pstInterfaceSpec->m_nNumEOs){
#ifdef FORTE_SUPPORT_MONITORING
    forte::core::SMonitorEvent &eventMonitoring(m_nEOMonitorCount[pa_nEO]);
    forte::core::SMonitorEvent::SMonitorEventData &monitoringData(eventMonitoring.mMonitorEventData[eventMonitoring.mBufPos]);
    // breakpoint set, ignore all other events
    if(forte::core::eActive != eventMonitoring.mBreakpointSet){
#endif //FORTE_SUPPORT_MONITORING
    if(0 != m_pstInterfaceSpec->m_anEOWithIndexes){
      if(-1 != m_pstInterfaceSpec->m_anEOWithIndexes[pa_nEO]){
        const TDataIOID *poEOWithStart = &(m_pstInterfaceSpec->m_anEOWith[m_pstInterfaceSpec->m_anEOWithIndexes[pa_nEO]]);
        //TODO think on this lock
        CCriticalRegion criticalRegion(m_poResource->m_oResDataConSync);
        for(int i = 0; poEOWithStart[i] != 255; ++i){
          CDataConnection *con = getDOConUnchecked(poEOWithStart[i]);
          if(con->isConnected()){
            CIEC_ANY *poDO = getDO(poEOWithStart[i]);
#ifdef FORTE_SUPPORT_MONITORING
            if(poDO->isForced() != true){
#endif //FORTE_SUPPORT_MONITORING
              con->writeData(poDO);
#ifdef FORTE_SUPPORT_MONITORING
            }
            else{
              //when forcing we write back the value from the connection to keep the forced value on the output
              con->readData(poDO);
            }
#endif //FORTE_SUPPORT_MONITORING
          }
        }
      }
    }

    getEOConUnchecked(static_cast<TPortId>(pa_nEO))->triggerEvent(*m_poInvokingExecEnv);

#ifdef FORTE_SUPPORT_MONITORING
      // stop execution on Breakpoint
      if(eventMonitoring.mBreakpointEnable){
        if(forte::core::eOnce == eventMonitoring.mBreakpointSet){ // if < 0 -> continue called
          eventMonitoring.mBreakpointSet = forte::core::eInactive;
        }
        else{
          eventMonitoring.mBreakpointSet = forte::core::eActive;
         return;
        }
        // Get Resource
        //CResource &res = this->getResource();
        //res.stopManagedObject();    // we cant restart it,
      }
      // Count Event for monitoring
      monitoringData.mEventCount++;
      monitoringData.mTimeStamp = getTimer().getForteTime();
      eventMonitoring.mBufPos = (eventMonitoring.mBufPos + 1) % forte::core::cgMonitorBufferLength;
      m_updated = true;
    }  // if(forte::core::eActive != eventMonitoring.mMonitorEventData[eventMonitoring.mBufPos].mBreakpointSet){
#endif //FORTE_SUPPORT_MONITORING
  }
}

void CFunctionBlock::sendAdapterEvent(int pa_nAdapterID, int pa_nEID) const{
  if(0 != m_apoAdapters[pa_nAdapterID]){
    m_apoAdapters[pa_nAdapterID]->receiveInputEvent(pa_nEID, *m_poInvokingExecEnv);
  }
}

bool CFunctionBlock::configureFB(const char *){
  return true;
}

void CFunctionBlock::receiveInputEvent(int pa_nEIID, CEventChainExecutionThread &pa_poExecEnv){
  FORTE_TRACE("InputEvent: Function Block (%s) got event: %d (maxid: %d)\n", CStringDictionary::getInstance().get(getInstanceNameId()), pa_nEIID, m_pstInterfaceSpec->m_nNumEIs - 1);

  if(e_RUNNING == getState()){
    if(pa_nEIID < m_pstInterfaceSpec->m_nNumEIs){
#ifdef FORTE_SUPPORT_MONITORING
      forte::core::SMonitorEvent &eventMonitoring(m_nEIMonitorCount[pa_nEIID]);
      forte::core::SMonitorEvent::SMonitorEventData &monitoringData(eventMonitoring.mMonitorEventData[eventMonitoring.mBufPos]);
      // breakpoint set, ignore all other events
      if(forte::core::eActive == eventMonitoring.mBreakpointSet){
        return;
      }
#endif //FORTE_SUPPORT_MONITORING
      if(0 != m_pstInterfaceSpec->m_anEIWithIndexes){

        //        printf(   "Event name: %s\n", m_pstInterfaceSpec->m_acEINames[pa_nEIID]);
        if(-1 != m_pstInterfaceSpec->m_anEIWithIndexes[pa_nEIID]){
          const TDataIOID *poEIWithStart = &(m_pstInterfaceSpec->m_anEIWith[m_pstInterfaceSpec->m_anEIWithIndexes[pa_nEIID]]);

          // TODO think on this lock
          CCriticalRegion criticalRegion(m_poResource->m_oResDataConSync);
          for(int i = 0; poEIWithStart[i] != 255; ++i){
            if(0 != m_apoDIConns[poEIWithStart[i]]){
              CIEC_ANY *poDI = getDI(poEIWithStart[i]);
#ifdef FORTE_SUPPORT_MONITORING
              if(true != poDI->isForced()){
#endif //FORTE_SUPPORT_MONITORING
                m_apoDIConns[poEIWithStart[i]]->readData(poDI);
#ifdef FORTE_SUPPORT_MONITORING
              }
#endif //FORTE_SUPPORT_MONITORING
            }
          }
        }
      }
#ifdef FORTE_SUPPORT_MONITORING
      // stop execution on Breakpoint
      if(eventMonitoring.mBreakpointEnable){
        if(forte::core::eOnce == eventMonitoring.mBreakpointSet){ // if < 0 -> continue called
          eventMonitoring.mBreakpointSet = forte::core::eInactive;
        }
        else{
          eventMonitoring.mBreakpointSet = forte::core::eActive;
         return;
        }
        // Get Resource
        //CResource &res = this->getResource();
        //res.stopManagedObject();    // we cant restart it,
      }
      // Count Event for monitoring
      monitoringData.mEventCount++;
      monitoringData.mTimeStamp = getTimer().getForteTime();
      eventMonitoring.mBufPos = (eventMonitoring.mBufPos + 1) % forte::core::cgMonitorBufferLength;
      m_updated = true;
#endif //FORTE_SUPPORT_MONITORING
    }
    m_poInvokingExecEnv = &pa_poExecEnv;
    executeEvent(pa_nEIID);
  }
}

EMGMResponse CFunctionBlock::changeFBExecutionState(EMGMCommandType pa_unCommand){
  EMGMResponse nRetVal = e_INVALID_STATE;
  switch (pa_unCommand){
    case cg_nMGM_CMD_Start:
      if((e_IDLE == m_enFBState) || (e_STOPPED == m_enFBState)){
        m_enFBState = e_RUNNING;
        nRetVal = e_RDY;
      }
      break;
    case cg_nMGM_CMD_Stop:
      if(e_RUNNING == m_enFBState){
        m_enFBState = e_STOPPED;
        nRetVal = e_RDY;
      }
      break;
    case cg_nMGM_CMD_Kill:
      if(e_RUNNING == m_enFBState){
        m_enFBState = e_KILLED;
        nRetVal = e_RDY;
      }
      break;
    case cg_nMGM_CMD_Reset:
      if((e_STOPPED == m_enFBState) || (e_KILLED == m_enFBState)){
        m_enFBState = e_IDLE;
        nRetVal = e_RDY;
        setInitialValues();
      }
      break;
    default:
      nRetVal = e_INVALID_OPERATION;
      break;
  }

  if(e_RDY == nRetVal){
    if(0 != m_pstInterfaceSpec){
      for(int i = 0; i < m_pstInterfaceSpec->m_nNumAdapters; ++i){
        if(0 != m_apoAdapters[i]){
          m_apoAdapters[i]->changeFBExecutionState(pa_unCommand);
        }
      }
    }
  }
  return nRetVal;
}

CIEC_ANY *CFunctionBlock::createDataPoint(const CStringDictionary::TStringId **pa_panDataTypeIds, TForteByte *pa_acDataBuf){
  CIEC_ANY *poRetVal = CTypeLib::createDataTypeInstance(**pa_panDataTypeIds, pa_acDataBuf);
  ++(*pa_panDataTypeIds);
#ifdef FORTE_SUPPORT_ARRAYS
  if(g_nStringIdARRAY == (*pa_panDataTypeIds)[-1]){
    if(0 != poRetVal){
      //For an array we have to do more
      (static_cast<CIEC_ARRAY *>(poRetVal))->setup(static_cast<TForteUInt16>(**pa_panDataTypeIds), (*pa_panDataTypeIds)[1]);
    }
    (*pa_panDataTypeIds) += 2;
  }
#endif
  return poRetVal;
}

void CFunctionBlock::setupFBInterface(const SFBInterfaceSpec *pa_pstInterfaceSpec, TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData, bool pa_bManagesFBData){
  m_pstInterfaceSpec = const_cast<SFBInterfaceSpec *>(pa_pstInterfaceSpec);
  m_bManagesFBData = pa_bManagesFBData;

  if(0 != pa_pstInterfaceSpec){
    if((0 != pa_acFBConnData) && (0 != pa_acFBVarsData)){
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
        mEOConns = 0;
      }

      const CStringDictionary::TStringId *pnDataIds;
      if(m_pstInterfaceSpec->m_nNumDIs){
        m_apoDIConns = reinterpret_cast<TDataConnectionPtr *>(pa_acFBConnData);
        pa_acFBConnData += sizeof(TDataConnectionPtr) * m_pstInterfaceSpec->m_nNumDIs;

        //let m_aoDIs point to the first data input
        m_aoDIs = reinterpret_cast<CIEC_ANY *>(pa_acFBVarsData);

        pnDataIds = pa_pstInterfaceSpec->m_aunDIDataTypeNames;
        for(i = 0; i < m_pstInterfaceSpec->m_nNumDIs; ++i){
          m_apoDIConns[i] = 0;
          createDataPoint(&pnDataIds, pa_acFBVarsData);
          pa_acFBVarsData += sizeof(CIEC_ANY);
        }
      }
      else{
        m_apoDIConns = 0;
        m_aoDIs = 0;
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
        mDOConns = 0;
        m_aoDOs = 0;
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

void CFunctionBlock::generateGenericInterfacePointNameArray(const char * const pa_acPrefix, CStringDictionary::TStringId* pa_anNamesArayStart, unsigned int pa_unNumGenericDataPoints){
  size_t unLen = strlen(pa_acPrefix);

  if(cg_nIdentifierLength > unLen){
    TIdentifier acBuffer;
    memcpy(acBuffer, pa_acPrefix, unLen);
    acBuffer[unLen + 1] = '\0';
    acBuffer[unLen + 2] = '\0';

		for(unsigned int i = 1; i <= pa_unNumGenericDataPoints; i++){
			if(i < 10){ //1 digit
				acBuffer[unLen] = static_cast<char>(0x30 + i);
			}
			else if(i < 100){ //2 digits
				if(0 == i % 10){
					acBuffer[unLen] = static_cast<char>(0x30 + (i % 100 / 10));
				}
				acBuffer[unLen + 1] = static_cast<char>(0x30 + i % 10);
      }
      else{ //3 digits
        if(0 == i % 100){
          acBuffer[unLen] = static_cast<char>(0x30 + (i / 100));
        }
        if(0 == i % 10){
          acBuffer[unLen + 1] = static_cast<char>(0x30 + (i % 100 / 10));
        }
				acBuffer[unLen + 2] = static_cast<char>(0x30 + i % 10);
			}
			pa_anNamesArayStart[i - 1] = CStringDictionary::getInstance().insert(acBuffer);
		}
	}
}

void CFunctionBlock::generateGenericDataPointArrays(const char * const pa_acPrefix, CStringDictionary::TStringId* pa_anDataTypeNamesArrayStart, CStringDictionary::TStringId* pa_anNamesArrayStart, unsigned int pa_unNumGenericDataPoints){
  generateGenericInterfacePointNameArray(pa_acPrefix, pa_anNamesArrayStart, pa_unNumGenericDataPoints);

  for(unsigned int i = 0; i < pa_unNumGenericDataPoints; i++){
    pa_anDataTypeNamesArrayStart[i] = g_nStringIdANY;
  }
}

#ifdef FORTE_SUPPORT_MONITORING
void CFunctionBlock::setupEventMonitoringData(){
  if(0 != m_pstInterfaceSpec->m_nNumEIs){
    m_nEIMonitorCount = new forte::core::SMonitorEvent[m_pstInterfaceSpec->m_nNumEIs];
  }

  if(0 != m_pstInterfaceSpec->m_nNumEOs){
    m_nEOMonitorCount = new forte::core::SMonitorEvent[m_pstInterfaceSpec->m_nNumEOs];
  }
}

//////////////////////////////////////////////////////////////////////////////
// EO-Monitor functions
//////////////////////////////////////////////////////////////////////////////
bool CFunctionBlock::startEIBreakpoint(TEventID p_nEventId){
  CCriticalRegion criticalRegion(m_poResource->m_oResDataConSync);
  if(p_nEventId < cg_unInvalidPortId){
    m_nEIMonitorCount[p_nEventId].mBreakpointEnable = true;
  }
  return true;
}

bool CFunctionBlock::stopEIBreakpoint(TEventID p_nEventId){
  CCriticalRegion criticalRegion(m_poResource->m_oResDataConSync);
  if(p_nEventId < cg_unInvalidPortId){
    m_nEIMonitorCount[p_nEventId].mBreakpointEnable = false;
  }
  return true;
}

bool CFunctionBlock::clearEIBreakpoint(TEventID p_nEventId){
  CCriticalRegion criticalRegion(m_poResource->m_oResDataConSync);
  if(p_nEventId != cg_unInvalidPortId){
    forte::core::SMonitorEvent &monitorEventData(m_nEIMonitorCount[p_nEventId]);
    if(monitorEventData.mBreakpointSet > 0){
      // Get Resource
      //CResource &res = this->getResource();
      //res.startManagedObject();
      if(monitorEventData.mBreakpointEnable == true){
        monitorEventData.mBreakpointSet = forte::core::eOnce; // clear breakpoint, execute once
      }
      else{
        monitorEventData.mBreakpointSet = forte::core::eInactive; // clear breakpoint
      }
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
// EI-Monitor functions
//////////////////////////////////////////////////////////////////////////////
bool CFunctionBlock::startEOBreakpoint(TEventID p_nEventId){
  CCriticalRegion criticalRegion(m_poResource->m_oResDataConSync);
  if(p_nEventId < cg_unInvalidPortId){
    forte::core::SMonitorEvent &monitorEventData(m_nEOMonitorCount[p_nEventId]);
    monitorEventData.mBreakpointEnable = true;
    monitorEventData.mBreakpointSet = forte::core::eInactive;
  }
  return true;
}

bool CFunctionBlock::stopEOBreakpoint(TEventID p_nEventId){
  CCriticalRegion criticalRegion(m_poResource->m_oResDataConSync);
  if(p_nEventId < cg_unInvalidPortId){
    forte::core::SMonitorEvent &monitorEventData(m_nEOMonitorCount[p_nEventId]);
    monitorEventData.mBreakpointEnable = false;
    monitorEventData.mBreakpointSet = forte::core::eInactive;
  }
  return true;
}

bool CFunctionBlock::clearEOBreakpoint(TEventID p_nEventId){
  CCriticalRegion criticalRegion(m_poResource->m_oResDataConSync);
  if(p_nEventId != cg_unInvalidPortId){
    forte::core::SMonitorEvent &monitorEventData(m_nEOMonitorCount[p_nEventId]);
    if(monitorEventData.mBreakpointSet > 0){
      // Get Resource
      //CResource &res = this->getResource();
      //res.startManagedObject();
      if(monitorEventData.mBreakpointEnable == true){
        monitorEventData.mBreakpointSet = forte::core::eOnce; // clear breakpoint, execute once
      }
      else{
        monitorEventData.mBreakpointSet = forte::core::eInactive; // clear breakpoint
      }
    }
  }
  return true;
}

bool CFunctionBlock::getEIBreakpoint(TEventID p_nEventId, bool & enable, int & set){
  bool ret = false;
  CCriticalRegion criticalRegion(m_poResource->m_oResDataConSync);
  if(p_nEventId < cg_unInvalidPortId){
    forte::core::SMonitorEvent &monitorEventData(m_nEIMonitorCount[p_nEventId]);
    enable = monitorEventData.mBreakpointEnable;
    set = monitorEventData.mBreakpointSet; // > 0;
    ret = true;
  }
  return ret;
}

bool CFunctionBlock::getEOBreakpoint(TEventID p_nEventId, bool & enable, int & set){
  bool ret = false;
  CCriticalRegion criticalRegion(m_poResource->m_oResDataConSync);
  if(p_nEventId < cg_unInvalidPortId){
    forte::core::SMonitorEvent &monitorEventData(m_nEOMonitorCount[p_nEventId]);
    enable = monitorEventData.mBreakpointEnable;
    set = monitorEventData.mBreakpointSet; // > 0;
    ret = true;
  }
  return ret;
}

CFunctionBlock *CFunctionBlock::getFB(forte::core::TNameIdentifier::CIterator &paNameListIt){
  CFunctionBlock *retVal = 0;

  if(paNameListIt.isLastEntry()){
    //only check for adpaters if it we have the last entry in the line
    retVal = getAdapter(*paNameListIt);
  }

  return retVal;
}

//////////////////////////////////////////////////////////////////////////////
// Data-Monitor functions
//////////////////////////////////////////////////////////////////////////////
bool CFunctionBlock::forceData(CStringDictionary::TStringId pa_acName, const char * forceValue){

  TPortId portID = getDOID(pa_acName);
  if(portID != cg_unInvalidPortId){
    CIEC_ANY *poDO = getDO(portID);
    poDO->setForced(true);
    poDO->fromString(forceValue);


    CDataConnection *con = getDOConUnchecked(portID);
    if(con->isConnected()){
      CCriticalRegion criticalRegion(m_poResource->m_oResDataConSync);
      con->writeData(poDO);
    }
    return true;
  }

  portID = getDIID(pa_acName);

  if(portID != cg_unInvalidPortId){
    CIEC_ANY *poDI = getDI(portID);
    poDI->setForced(true);
    poDI->fromString(forceValue);
    return true;
  }

  return false;
}

forte::core::SMonitorEvent &CFunctionBlock::getEIMontiorData(TEventID pa_unEIID){
  return m_nEIMonitorCount[pa_unEIID];
}

forte::core::SMonitorEvent &CFunctionBlock::getEOMontiorData(TEventID pa_unEOID){
  return m_nEOMonitorCount[pa_unEOID];
}

#endif //FORTE_SUPPORT_MONITORING
