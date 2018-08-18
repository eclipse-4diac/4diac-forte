/*******************************************************************************
 * Copyright (c) 2005 - 2018 ACIN, Profactor GmbH, fortiss GmbH,
 *                           Johannes Kepler University
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Alois Zoitl, Rene Smodic, Gerhard Ebenhofer, Thomas Strasser,
 *    Martin Melik Merkumians,
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include <fortenew.h>
#include "resource.h"
#include "device.h"
#include "adapter.h"
#include "adapterconn.h"
#include "if2indco.h"
#include "utils/criticalregion.h"
#include "utils/fixedcapvector.h"
#include "ecet.h"

#ifdef FORTE_DYNAMIC_TYPE_LOAD
#include "lua/luaengine.h"
#include "lua/luacfbtypeentry.h"
#include "lua/luabfbtypeentry.h"
#include "lua/luaadaptertypeentry.h"
#endif

CResource::CResource(CResource* pa_poDevice, const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId, TForteByte *pa_acFBConnData,
    TForteByte *pa_acFBVarsData) :
        CFunctionBlock(pa_poDevice, pa_pstInterfaceSpec, pa_nInstanceNameId, pa_acFBConnData, pa_acFBVarsData),
        forte::core::CFBContainer(CStringDictionary::scm_nInvalidStringId, 0), // the fbcontainer of resources does not have a seperate name as it is stored in the resource
        mResourceEventExecution(CEventChainExecutionThread::createEcet()),
        mResIf2InConnections(0)
            #ifdef FORTE_SUPPORT_MONITORING
            , mMonitoringHandler(*this)
#endif
{
#ifdef FORTE_DYNAMIC_TYPE_LOAD
        luaEngine = new CLuaEngine();
#endif
  initializeResIf2InConnections();
}

CResource::CResource(const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId, TForteByte *pa_acFBConnData,
    TForteByte *pa_acFBVarsData) :
    CFunctionBlock(0, pa_pstInterfaceSpec, pa_nInstanceNameId, pa_acFBConnData, pa_acFBVarsData),
        forte::core::CFBContainer(CStringDictionary::scm_nInvalidStringId, 0), // the fbcontainer of resources does not have a seperate name as it is stored in the resource
        mResourceEventExecution(0),
        mResIf2InConnections(0)
            #ifdef FORTE_SUPPORT_MONITORING
            , mMonitoringHandler(*this)
#endif
{
#ifdef FORTE_DYNAMIC_TYPE_LOAD
    luaEngine = new CLuaEngine();
#endif
  initializeResIf2InConnections();
}

CResource::~CResource(){
#ifdef FORTE_DYNAMIC_TYPE_LOAD
  delete luaEngine;
#endif
  delete mResourceEventExecution;
  delete[] mResIf2InConnections;
}


EMGMResponse CResource::executeMGMCommand(forte::core::SManagementCMD &paCommand){
  EMGMResponse retVal = e_INVALID_DST;

  if(CStringDictionary::scm_nInvalidStringId == paCommand.mDestination){
    switch (paCommand.mCMD){
      case cg_nMGM_CMD_Create_FBInstance: {
        forte::core::TNameIdentifier::CIterator itRunner(paCommand.mFirstParam.begin());
        retVal = createFB(itRunner, paCommand.mSecondParam.front(), this);
      }
        break;
      case cg_nMGM_CMD_Create_FBType: {
#ifdef FORTE_DYNAMIC_TYPE_LOAD
        retVal = createFBTypeFromLua(paCommand.mFirstParam.front(), paCommand.mAdditionalParams);
#else
        retVal = e_UNSUPPORTED_CMD;
#endif
      }
        break;
      case cg_nMGM_CMD_Create_AdapterType: {
#ifdef FORTE_DYNAMIC_TYPE_LOAD
        retVal = createAdapterTypeFromLua(paCommand.mFirstParam.front(), paCommand.mAdditionalParams);
#else
        retVal = e_UNSUPPORTED_CMD;
#endif
      }
        break;
      case cg_nMGM_CMD_Delete_FBInstance: {
        forte::core::TNameIdentifier::CIterator itRunner(paCommand.mFirstParam.begin());
        retVal = deleteFB(itRunner);
      }
        break;
      case cg_nMGM_CMD_Create_Connection:
        retVal = createConnection(paCommand.mFirstParam, paCommand.mSecondParam);
        break;
      case cg_nMGM_CMD_Delete_Connection:
        retVal = deleteConnection(paCommand.mFirstParam, paCommand.mSecondParam);
        break;
      case cg_nMGM_CMD_Read:
        retVal = readValue(paCommand.mFirstParam, paCommand.mAdditionalParams);
        break;
      case cg_nMGM_CMD_Write:
        retVal = writeValue(paCommand.mFirstParam, paCommand.mAdditionalParams);
        break;
      case cg_nMGM_CMD_Start:
        case cg_nMGM_CMD_Stop:
        case cg_nMGM_CMD_Kill:
        case cg_nMGM_CMD_Reset:
        retVal = handleExecutionStateCmd(paCommand.mCMD, paCommand.mFirstParam);
        break;
      case cg_nMGM_CMD_QUERY_FBTypes:
        retVal = queryAllFBTypes(paCommand.mAdditionalParams);
        break;
      case cg_nMGM_CMD_QUERY_AdapterTypes:
        retVal = queryAllAdapterTypes(paCommand.mAdditionalParams);
        break;
      case cg_nMGM_CMD_QUERY_FB:{
#ifdef FORTE_DYNAMIC_TYPE_LOAD
        retVal = queryFBs(paCommand.mAdditionalParams);
#else
        retVal = e_UNSUPPORTED_CMD;
#endif
      }
        break;
      case cg_nMGM_CMD_QUERY_Connection:{
#ifdef FORTE_DYNAMIC_TYPE_LOAD
        retVal = queryConnections(paCommand.mAdditionalParams);
#else
        retVal = e_UNSUPPORTED_CMD;
#endif
      }
        break;
      default:
        #ifdef FORTE_SUPPORT_MONITORING
        retVal = mMonitoringHandler.executeMonitoringCommand(paCommand);
#else
        retVal = e_UNSUPPORTED_CMD;
#endif
        break;
    }
  }
  return retVal;
}

EMGMResponse CResource::changeFBExecutionState(EMGMCommandType pa_unCommand){
  EMGMResponse retVal = CFunctionBlock::changeFBExecutionState(pa_unCommand);
  if(e_RDY == retVal){
    retVal = changeContainedFBsExecutionState(pa_unCommand);
    if(e_RDY == retVal){
      if(cg_nMGM_CMD_Start == pa_unCommand){
        //on start sample inputs
        if(0 != m_pstInterfaceSpec){
          for(int i = 0; i < m_pstInterfaceSpec->m_nNumDIs; ++i){
            if(0 != m_apoDIConns[i]){
              m_apoDIConns[i]->readData(getDI(i));
            }
          }
        }
      }
      if(0 != mResourceEventExecution){
        // if we have a m_poResourceEventExecution handle it
        mResourceEventExecution->changeExecutionState(pa_unCommand);
      }
    }
  }
  return retVal;
}

EMGMResponse CResource::handleExecutionStateCmd(EMGMCommandType paCMD,
    forte::core::TNameIdentifier &paTarget){
  EMGMResponse retVal = e_NO_SUCH_OBJECT;
  CFunctionBlock *fb = this;

  if(!paTarget.isEmpty()){
    forte::core::TNameIdentifier::CIterator itRunner(paTarget.begin());
    fb = getContainedFB(itRunner);
  }

  if(0 != fb){
    retVal = fb->changeFBExecutionState(paCMD);
  }
  return retVal;
}

EMGMResponse CResource::createConnection(forte::core::SManagementCMD &paCommand){
  return createConnection(paCommand.mFirstParam, paCommand.mSecondParam);
}


EMGMResponse CResource::createConnection(forte::core::TNameIdentifier &paSrcNameList,
    forte::core::TNameIdentifier &paDstNameList){
  EMGMResponse retVal = e_NO_SUCH_OBJECT;

  CConnection *con = getConnection(paSrcNameList);
  if(0 != con){
    CStringDictionary::TStringId portName = paDstNameList.back();
    paDstNameList.popBack();
    forte::core::TNameIdentifier::CIterator runner(paDstNameList.begin());
    CFunctionBlock *dstFB = getContainedFB(runner);
    if((0 != dstFB) && (runner.isLastEntry())){
      retVal = con->connect(dstFB, portName);
    }
  }

  return retVal;
}

EMGMResponse CResource::deleteConnection(forte::core::TNameIdentifier &paSrcNameList,
    forte::core::TNameIdentifier &paDstNameList){
  EMGMResponse retVal = e_NO_SUCH_OBJECT;

  CConnection *con = getConnection(paSrcNameList);
  if(0 != con){
    CStringDictionary::TStringId portName = paDstNameList.back();
    paDstNameList.popBack();
    forte::core::TNameIdentifier::CIterator runner(paDstNameList.begin());
    CFunctionBlock *dstFB = getContainedFB(runner);
    if((0 != dstFB) && (runner.isLastEntry())){
      retVal = con->disconnect(dstFB, portName);
    }
  }

  return retVal;
}

EMGMResponse CResource::writeValue(forte::core::TNameIdentifier &paNameList,
    const CIEC_STRING & paValue, bool paForce){
  EMGMResponse retVal = e_NO_SUCH_OBJECT;

  CStringDictionary::TStringId portName = paNameList.back();
  paNameList.popBack();
  forte::core::TNameIdentifier::CIterator runner(paNameList.begin());

  CFunctionBlock *fb = this;
  if(paNameList.size() >= 1){
   //this is not an identifier for the resource interface
   fb = getContainedFB(runner); // the last entry is the input name therefore reduce list here by one
  }

  if((0 != fb) && (runner.isLastEntry())){
    CIEC_ANY *var = fb->getVar(&portName, 1);
    if(0 != var){
      // 0 is not supported in the fromString method
      if((paValue.length() > 0) && (paValue.length() == var->fromString(paValue.getValue()))){
        //if we cannot parse the full value the value is not valid
        if(paForce){
          var->setForced(true);
          CDataConnection *con = fb->getDOConnection(portName);
          if(0 != con){
            //if we have got a connection it was a DO mirror the forced value there
            CCriticalRegion criticalRegion(m_oResDataConSync);
            con->writeData(var);
          }
        }
        retVal = e_RDY;
      } else {
        retVal = e_BAD_PARAMS;
      }
    }
  }
  return retVal;
}

EMGMResponse CResource::readValue(forte::core::TNameIdentifier &paNameList, CIEC_STRING & paValue){
  EMGMResponse retVal = e_NO_SUCH_OBJECT;

  CIEC_ANY *var = getVariable(paNameList);
  if(0 != var){
    int nUsedChars = var->toString(paValue.getValue(), paValue.getCapacity());

    if(-1 != nUsedChars){
      paValue.assign(paValue.getValue(), static_cast<TForteUInt16>(nUsedChars));
      retVal = e_RDY;
    }
    else{
      retVal = e_INVALID_OBJECT;
    }
  }
  return retVal;
}

EMGMResponse CResource::queryAllFBTypes(CIEC_STRING & paValue){
  EMGMResponse retVal = e_UNSUPPORTED_TYPE;

  CTypeLib::CTypeEntry *fbTypeRunner = CTypeLib::getFBLibStart();
  if(fbTypeRunner != 0){
    retVal = e_RDY;
    for(; fbTypeRunner != 0; fbTypeRunner = fbTypeRunner->m_poNext){
      paValue.append(CStringDictionary::getInstance().get(fbTypeRunner->getTypeNameId()));
      if(fbTypeRunner->m_poNext != 0){
        paValue.append(", ");
      }
    }
  }
  return retVal;
}

#ifdef FORTE_DYNAMIC_TYPE_LOAD
void CResource::createEOConnectionResponse(const CFunctionBlock& paFb, CIEC_STRING& paReqResult){
  const SFBInterfaceSpec *const spec = paFb.getFBInterfaceSpec();
  if(spec->m_nNumEOs > 0){
    for(size_t i = 0; spec->m_aunEONames[i] != spec->m_aunEONames[spec->m_nNumEOs]; i++){
      const CEventConnection* eConn = paFb.getEOConnection(spec->m_aunEONames[i]);
      for(CSinglyLinkedList<SConnectionPoint>::Iterator itRunnerDst(eConn->getDestinationList().begin()); itRunnerDst != eConn->getDestinationList().end(); ++itRunnerDst){
        if(itRunnerDst != eConn->getDestinationList().begin()){
          paReqResult.append("\n");
        }
        createConnectionResponseMessage(spec->m_aunEONames[i],
          itRunnerDst->mFB->getFBInterfaceSpec()->m_aunEINames[itRunnerDst->mPortId],
          *itRunnerDst->mFB, paFb, paReqResult);
      }
    }
  }
}

void CResource::createDOConnectionResponse(const CFunctionBlock& paFb, CIEC_STRING& paReqResult){
  const SFBInterfaceSpec *const spec = paFb.getFBInterfaceSpec();
  if(spec->m_nNumDOs > 0){
    for(size_t i = 0; spec->m_aunDONames[i] != spec->m_aunDONames[spec->m_nNumDOs]; i++){
      const CDataConnection *const dConn = paFb.getDOConnection(spec->m_aunDONames[i]);
      for(CSinglyLinkedList<SConnectionPoint>::Iterator itRunnerDst(dConn->getDestinationList().begin()); itRunnerDst != dConn->getDestinationList().end(); ++itRunnerDst){
        if(itRunnerDst != dConn->getDestinationList().begin()){
          paReqResult.append("\n");
        }
        createConnectionResponseMessage(spec->m_aunDONames[i],
          itRunnerDst->mFB->getFBInterfaceSpec()->m_aunDINames[itRunnerDst->mPortId],
          *itRunnerDst->mFB, paFb, paReqResult);
      }
    }
  }
}

void CResource::createAOConnectionResponse(const CFunctionBlock& paFb, CIEC_STRING& paReqResult){
  const SFBInterfaceSpec *const spec = paFb.getFBInterfaceSpec();
  if(spec->m_nNumAdapters > 0){
    for(size_t i = 0; i < spec->m_nNumAdapters; i++){
      const CAdapter *const adapter = paFb.getAdapter(spec->m_pstAdapterInstanceDefinition[i].m_nAdapterNameID);
      const CAdapterConnection* aConn = adapter->getAdapterConnection();
      if(spec->m_pstAdapterInstanceDefinition[i].m_bIsPlug && 0 != aConn){
        if(i != 0){
          paReqResult.append("\n");
        }
        CSinglyLinkedList<SConnectionPoint>::Iterator itRunnerDst(aConn->getDestinationList().begin());
        createConnectionResponseMessage(spec->m_pstAdapterInstanceDefinition[i].m_nAdapterNameID,
          itRunnerDst->mFB->getFBInterfaceSpec()->m_pstAdapterInstanceDefinition[itRunnerDst->mPortId].m_nAdapterNameID,
          *itRunnerDst->mFB, paFb, paReqResult);
      }
    }
  }
}

void CResource::createConnectionResponseMessage(const CStringDictionary::TStringId srcId, const CStringDictionary::TStringId dstId,
    const CFunctionBlock& paDstFb, const CFunctionBlock& paSrcFb, CIEC_STRING& paReqResult){
  paReqResult.append("<Connection source=\"");
  paReqResult.append(paSrcFb.getInstanceName());
  paReqResult.append(".");
  paReqResult.append(CStringDictionary::getInstance().get(srcId));
  paReqResult.append("\" destination=\"");
  paReqResult.append(paDstFb.getInstanceName());
  paReqResult.append(".");
  paReqResult.append(CStringDictionary::getInstance().get(dstId));
  paReqResult.append("\"/>");
}

EMGMResponse CResource::queryConnections(CIEC_STRING & paReqResult){
   EMGMResponse retVal = e_UNSUPPORTED_TYPE;
   //TODO check container list to support subapps issue[538333]
    for (TFunctionBlockList::Iterator itRunner(getFBList().begin()); itRunner != getFBList().end(); ++itRunner) {
      createEOConnectionResponse(**itRunner, paReqResult);
      createDOConnectionResponse(**itRunner, paReqResult);
      createAOConnectionResponse(**itRunner, paReqResult);
    }
    retVal = e_RDY;
    return retVal;
}

EMGMResponse CResource::queryFBs(CIEC_STRING & paValue){
  EMGMResponse retVal = e_UNSUPPORTED_TYPE;
  for (TFunctionBlockList::Iterator itRunner(getFBList().begin()); itRunner != getFBList().end(); ++itRunner) {
    if(itRunner != getFBList().begin()){
      paValue.append("\n");
    }
    paValue.append("<FB name=\"");
    paValue.append((static_cast<CFunctionBlock *>(*itRunner))->getInstanceName());
    paValue.append("\" type=\"");
    paValue.append(CStringDictionary::getInstance().get((static_cast<CFunctionBlock *>(*itRunner))->getFBTypeId()));
    paValue.append("\"/>");
  }
  retVal = e_RDY;
  return retVal;
}

EMGMResponse CResource::createFBTypeFromLua(CStringDictionary::TStringId typeNameId,
    CIEC_STRING& paLuaScriptAsString){
  EMGMResponse retVal = e_UNSUPPORTED_TYPE;
  if(NULL != strstr(paLuaScriptAsString.getValue(), "internalFBs")){// CFBType
    if(CLuaCFBTypeEntry::createLuaFBTypeEntry(typeNameId, paLuaScriptAsString) != NULL){
         retVal = e_RDY;
       }else{
         retVal = e_INVALID_OPERATION;
       }
  }else{// BFBType
    if(CLuaBFBTypeEntry::createLuaFBTypeEntry(typeNameId, paLuaScriptAsString) != NULL){
      retVal = e_RDY;
    }else{
      retVal = e_INVALID_OPERATION;
    }
  }
  return retVal;
}

EMGMResponse CResource::createAdapterTypeFromLua(CStringDictionary::TStringId typeNameId,
    CIEC_STRING& paLuaScriptAsString){
  EMGMResponse retVal = e_UNSUPPORTED_TYPE;
   if(CLuaAdapterTypeEntry::createLuaAdapterTypeEntry(typeNameId, paLuaScriptAsString) != NULL){
     retVal = e_RDY;
   }else{
     retVal = e_INVALID_OPERATION;
   }
  return retVal;
}
#endif

EMGMResponse CResource::queryAllAdapterTypes(CIEC_STRING & paValue){
  EMGMResponse retVal = e_UNSUPPORTED_TYPE;

  CTypeLib::CTypeEntry *adapterTypeRunner = CTypeLib::getAdapterLibStart();
  if(adapterTypeRunner != 0){
    retVal = e_RDY;
    for(; adapterTypeRunner != 0; adapterTypeRunner = adapterTypeRunner->m_poNext){
      paValue.append(CStringDictionary::getInstance().get(adapterTypeRunner->getTypeNameId()));
      if(adapterTypeRunner->m_poNext != 0){
        paValue.append(", ");
      }
    }
  }
  return retVal;
}

CIEC_ANY *CResource::getVariable(forte::core::TNameIdentifier &paNameList){
  CStringDictionary::TStringId portName = paNameList.back();
  paNameList.popBack();
  forte::core::TNameIdentifier::CIterator runner(paNameList.begin());

  CFunctionBlock *fb = this;
  if(paNameList.size() >= 1){
    //this is not an identifier for the resource interface
    fb = getContainedFB(runner); // the last entry is the input name therefore reduce list here by one
  }

  CIEC_ANY *var = 0;
  if((0 != fb) && (runner.isLastEntry())){
    var = fb->getVar(&portName, 1);
  }
  return var;
}

CConnection *CResource::getConnection(forte::core::TNameIdentifier &paSrcNameList){
  CConnection *con = 0;
  if(1 == paSrcNameList.size()){
    con = getResIf2InConnection(paSrcNameList[0]);
  }
  else if(paSrcNameList.size() > 1){
    CStringDictionary::TStringId portName = paSrcNameList.back();
    paSrcNameList.popBack();
    forte::core::TNameIdentifier::CIterator runner(paSrcNameList.begin());

    CFunctionBlock *srcFB = getContainedFB(runner);
    if((0 != srcFB) && (runner.isLastEntry())){
      //only use the found result if we have really the last result in the list
      con = srcFB->getEOConnection(portName);
      if(0 == con){
        //it is not an event connection try data connection next
        con = srcFB->getDOConnection(portName);
        if(0 == con){
          //it is not an data connection try data connection next
          //TODO think if it would be better to move this to CFunctionBlock
          CAdapter *adp = srcFB->getAdapter(portName);
          if((0 != adp) && (adp->isPlug())){
            //only plugs hold the connection object
            con = adp->getAdapterConnection();
          }
        }
      }
    }
  }
  return con;
}

CConnection *CResource::getResIf2InConnection(CStringDictionary::TStringId paResInput) const{
  CConnection *con = 0;
  if(0 != m_pstInterfaceSpec){
    TPortId inPortId = getDIID(paResInput);
    if(cg_unInvalidPortId != inPortId){
      con = mResIf2InConnections + inPortId;
    }
  }
  return con;
}

void CResource::initializeResIf2InConnections(){
  if(0 != m_pstInterfaceSpec){
    mResIf2InConnections = new CInterface2InternalDataConnection[m_pstInterfaceSpec->m_nNumDIs];
    for(TPortId i = 0; i < m_pstInterfaceSpec->m_nNumDIs; i++){
      (mResIf2InConnections + i)->setSource(this, i);
    }
  }
}
