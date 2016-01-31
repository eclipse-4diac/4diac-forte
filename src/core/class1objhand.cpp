/*******************************************************************************
  * Copyright (c) 2006 - 2015 ACIN, Profactor GmbH, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl, Thomas Strasser, Gerhard Ebenhofer, Rene Smodic, Ingo Hegny,
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include "class1objhand.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "class1objhand_gen.cpp"
#endif
#include "if2indco.h"
#include "typelib.h"
#include "resource.h"
#include "adapterconn.h"
#include "utils/criticalregion.h"

C61499Class1ObjectHandler::C61499Class1ObjectHandler(CResource& pa_roHandlerOwner) :
    m_roHandlerOwner(pa_roHandlerOwner)
{
}

C61499Class1ObjectHandler::~C61499Class1ObjectHandler(){
  deleteWholeFBNetwork();
}

EMGMResponse C61499Class1ObjectHandler::executeMGMCommand(SManagementCMD &pa_oCommand){
  EMGMResponse eRetVal = e_INVALID_DST;

  if(CStringDictionary::scm_nInvalidStringId == pa_oCommand.m_nDestination){
    switch (pa_oCommand.m_nCMD){
      case cg_nMGM_CMD_Create_Connection:
        eRetVal = createConnection(pa_oCommand);
        break;
      case cg_nMGM_CMD_Delete_Connection:
        eRetVal = deleteConnection(pa_oCommand);
        break;
      case cg_nMGM_CMD_Delete_AllFBInstances:
        eRetVal = deleteWholeFBNetwork();
        break;
#ifdef FORTE_SUPPORT_QUERY_CMD
      case cg_nMGM_CMD_QUERY_FB:
        eRetVal = executeQueryReq(pa_oCommand);
        break;
      case cg_nMGM_CMD_QUERY_Connection:
        eRetVal = executeQueryCon(pa_oCommand);
        break;
      case cg_nMGM_CMD_QUERY_FBTypes:
        pa_oCommand.m_oAdditionalParams.reserve(static_cast<TForteUInt16>(g_nStringIdNextFreeId / 2));
        eRetVal = queryTypeList(pa_oCommand, CTypeLib::getFBLibStart());
        break;
      case cg_nMGM_CMD_QUERY_DTTypes:
        eRetVal = queryTypeList(pa_oCommand, CTypeLib::getDTLibStart());
        break;
      case cg_nMGM_CMD_QUERY_AdapterTypes:
        eRetVal = queryTypeList(pa_oCommand, CTypeLib::getAdapterLibStart());
        break;
#endif


      default:
        eRetVal = e_UNSUPPORTED_CMD;
        break;
    }
  }
  return eRetVal;
}


EMGMResponse C61499Class1ObjectHandler::deleteConnection(SManagementCMD &paCommand){
  EMGMResponse retval = e_NO_SUCH_OBJECT;

  CFunctionBlock *srcFB = static_cast<CFunctionBlock*>(&m_roHandlerOwner);
  if(1 < paCommand.mFirstParamArraySize){
    //this is not a connection from the resource interface
    srcFB = m_roHandlerOwner.getFB(paCommand.mFirstParam, paCommand.mFirstParamArraySize);
  }
  CFunctionBlock *dstFB = m_roHandlerOwner.getFB(paCommand.mSecondParam, paCommand.mSecondParamArraySize);

  if((0 != srcFB) && (0 != dstFB)){ // check if the named fbs are existing in this resource
    TConnectionList::Iterator itRunner = m_lstpoConnectionList.begin();
    TConnectionList::Iterator itRefNode = m_lstpoConnectionList.end();

    //TODO workaround till connection is updated to new mgm structure
    TConnectionID nSrcId;
    if(srcFB == &m_roHandlerOwner){
      nSrcId = paCommand.mFirstParam[1];
    }else{
      nSrcId = CConnection::genConPortId(paCommand.mFirstParam[0], paCommand.mFirstParam[1]);
    }
    TConnectionID nDestId = CConnection::genConPortId(paCommand.mSecondParam[0], paCommand.mSecondParam[1]);

    while(itRunner != m_lstpoConnectionList.end()){
      if((*itRunner)->getSourceId() == nSrcId){
        CConnection *poConToDel = *itRunner;
        retval = poConToDel->disconnect(nDestId, *srcFB, *dstFB);

        if(poConToDel->isEmpty()){
          //this was the last fan of the connection remove it from the list
          CTypeLib::deleteConnection(poConToDel); // if yes delete the connection as it point nowhere
          if(itRefNode == m_lstpoConnectionList.end()){
            m_lstpoConnectionList.pop_front();
          }
          else{
            m_lstpoConnectionList.eraseAfter(itRefNode);
          }
        }

        break;
      }

      itRefNode = itRunner;
      ++itRunner;
    }
  }
  return retval;
}

EMGMResponse C61499Class1ObjectHandler::createConnection(SManagementCMD &paCommand){
  EMGMResponse retval = e_NO_SUCH_OBJECT;

  CFunctionBlock *srcFB = static_cast<CFunctionBlock*>(&m_roHandlerOwner);
  if(1 < paCommand.mFirstParamArraySize){
    //this is not a connection from the resource interface
    srcFB = m_roHandlerOwner.getFB(paCommand.mFirstParam, paCommand.mFirstParamArraySize);
  }
  CFunctionBlock *dstFB = m_roHandlerOwner.getFB(paCommand.mSecondParam, paCommand.mSecondParamArraySize);

  CConnection *newConnection = 0;
  if((0 != srcFB) && (0 != dstFB)){ // check if the named fbs are existing in this resource
    //TODO workaround till connection is updated to new mgm structure
    TConnectionID nSrcId;
    if(srcFB == &m_roHandlerOwner){
      nSrcId = paCommand.mFirstParam[0];
    }else{
      nSrcId = CConnection::genConPortId(paCommand.mFirstParam[0], paCommand.mFirstParam[1]);
    }
    TConnectionID nDestId = CConnection::genConPortId(paCommand.mSecondParam[0], paCommand.mSecondParam[1]);

    newConnection = getConnection(nSrcId); //try to find if there is already a connection with given source

    if(0 == newConnection){
      // there is no connection with given source try to create a new one
      CStringDictionary::TStringId unOutNameId = CConnection::extractPortNameId(nSrcId);
      if(srcFB == static_cast<CFunctionBlock*>(&m_roHandlerOwner)){
        if(0 != m_roHandlerOwner.getDataInput(unOutNameId)){
          newConnection = new CInterface2InternalDataConnection(nSrcId, *srcFB, nDestId, *dstFB);
        }
      }
      else{
        if(cg_nInvalidEventID != srcFB->getEOID(unOutNameId)){
          newConnection = new CEventConnection(nSrcId, *srcFB, nDestId, *dstFB); // it was an event connection to create
        }
        else{
          if(0 != srcFB->getDataOutput(unOutNameId)){
            newConnection = new CDataConnection(nSrcId, *srcFB, nDestId, *dstFB); // it was an data connection to create
          }
          else{
            if(0 != srcFB->getAdapter(unOutNameId)){
              newConnection = new CAdapterConnection(nSrcId, *srcFB, nDestId, *dstFB);
            }
          }
        }
      }
      if(0 != newConnection){
        if(newConnection->isEmpty()){ //Check if it was the only destination this connection had
          delete newConnection; // if yes delete the connection as it point nowhere
        }
        else{
          retval = e_RDY;
          m_lstpoConnectionList.push_back(newConnection);
        }
      }
    }
    else{ // Connection exists, Dest. added to the Dest.-List
      retval = newConnection->connectFannedOut(nDestId, *srcFB, *dstFB); //establish the connection
    }
  }
  return retval;
}

CConnection* C61499Class1ObjectHandler::getConnection(TConnectionID pa_nSrcId){
  CConnection* pRetVal = 0;
  for(TConnectionList::Iterator it = m_lstpoConnectionList.begin(); it != m_lstpoConnectionList.end(); ++it){
    if(pa_nSrcId == (*(*it)).getSourceId()){
      pRetVal = (*it);
      break;
    }
  }
  return pRetVal;
}

EMGMResponse C61499Class1ObjectHandler::deleteWholeFBNetwork(){
  //changeFBExecutionState(cg_nMGM_CMD_Kill);

  for(TConnectionList::Iterator itRunner(m_lstpoConnectionList.begin()); itRunner != m_lstpoConnectionList.end(); ++itRunner){
    CTypeLib::deleteConnection(*itRunner);
  }
  m_lstpoConnectionList.clearAll();

  for(TObjInstanceList::Iterator itRunner(m_lstpoObjectInstanceList.begin()); itRunner != m_lstpoObjectInstanceList.end(); ++itRunner){
    CTypeLib::deleteFB(*itRunner);
  }
  m_lstpoObjectInstanceList.clearAll();
  return e_RDY;
}



#ifdef FORTE_SUPPORT_QUERY_CMD
EMGMResponse C61499Class1ObjectHandler::executeQueryReq(SManagementCMD &paCommand){ //TConnectionID pa_nSrcId, TConnectionID &pa_nDestId, char *pa_pacResponse) {
  EMGMResponse eRetVal = e_NO_SUCH_OBJECT;
  TObjInstanceList::Iterator itRunner = m_lstpoObjectInstanceList.begin();

  paCommand.m_oAdditionalParams.clear();

  if(CStringDictionary::scm_nInvalidStringId == paCommand.mFirstParam[0]){
    if(CStringDictionary::scm_nInvalidStringId == paCommand.mSecondParam[0]){
      //fetch all fb instancenames of all types and return them
      for(; itRunner != m_lstpoObjectInstanceList.end(); ++itRunner){
        paCommand.m_oAdditionalParams.append((*itRunner)->getInstanceName());
        paCommand.m_oAdditionalParams.append(",", 1);
      }
      eRetVal = e_RDY;
    }
    else{
      //fetch all fb instancenames of the defined type and return them
      CTypeLib::CTypeEntry* FBTypes = CTypeLib::getFBLibStart();
      bool bFoundType = false;
      do{
        if(FBTypes->getTypeNameId() == paCommand.mSecondParam[0]){
          bFoundType = true;
          break;
        }
        FBTypes = FBTypes->m_poNext;
      } while(0 != FBTypes);

      if(!bFoundType){
        eRetVal = e_UNSUPPORTED_TYPE;
      }
      else{
        for(; itRunner != m_lstpoObjectInstanceList.end(); ++itRunner){
          if((*itRunner)->getFBTypeId() == paCommand.mSecondParam[0]){
            paCommand.m_oAdditionalParams.append((*itRunner)->getInstanceName());
            paCommand.m_oAdditionalParams.append(",", 1);
          }
        }
        eRetVal = e_RDY;
      }
    }
    if(!paCommand.m_oAdditionalParams.empty()){
      paCommand.m_oAdditionalParams.assign(paCommand.m_oAdditionalParams.getValue(), static_cast<TForteUInt16>(paCommand.m_oAdditionalParams.length() - 1));
    }
  }
  else{ // Query whether FB instancename exists + FB Type
    if(CStringDictionary::scm_nInvalidStringId == paCommand.mSecondParam[0]){
      const char *acSourceName = CStringDictionary::getInstance().get(paCommand.mFirstParam[0]);
      for(; itRunner != m_lstpoObjectInstanceList.end(); ++itRunner){
        paCommand.m_oAdditionalParams.assign((*itRunner)->getInstanceName(), static_cast<TForteUInt16>(strlen((*itRunner)->getInstanceName())));
        if(!strcmp(paCommand.m_oAdditionalParams.getValue(), acSourceName)){
          //pa_oCommand.m_nSecondParam = (*itRunner)->getFBTypeId();
          paCommand.m_oAdditionalParams.clear(); //clear string (held FB-Name)
          paCommand.m_oAdditionalParams.append(CStringDictionary::getInstance().get(static_cast<CStringDictionary::TStringId>((*itRunner)->getFBTypeId())));
          eRetVal = e_RDY;
          break;
        }
      }

      if(eRetVal != e_RDY){
        paCommand.m_oAdditionalParams.clear(); //clear string (held FB-Name)
        eRetVal = e_NO_SUCH_OBJECT;
      }
    }
    else{ //type and instance-name given -> return status
      const char *acSourceName = CStringDictionary::getInstance().get(static_cast<CStringDictionary::TStringId>(paCommand.mFirstParam[0]));
      for(; itRunner != m_lstpoObjectInstanceList.end(); ++itRunner){
        paCommand.m_oAdditionalParams.assign((*itRunner)->getInstanceName(), static_cast<TForteUInt16>(strlen((*itRunner)->getInstanceName())));
        if(!strcmp(paCommand.m_oAdditionalParams.getValue(), acSourceName)){
          paCommand.m_oAdditionalParams.clear(); //clear string (held FB-Name)
          if(paCommand.mSecondParam[0] == (*itRunner)->getFBTypeId()){
            switch ((*itRunner)->getState()){
              case CFunctionBlock::e_IDLE:
                paCommand.m_oAdditionalParams.append("IDLE", 4);
                break;
              case CFunctionBlock::e_RUNNING:
                paCommand.m_oAdditionalParams.append("RUNNING", 7);
                break;
              case CFunctionBlock::e_STOPPED:
                paCommand.m_oAdditionalParams.append("STOPPED", 7);
                break;
              case CFunctionBlock::e_KILLED:
                paCommand.m_oAdditionalParams.append("KILLED", 6);
                break;
              default:
                break;
            }
            eRetVal = e_RDY;
          }
          else{
            eRetVal = e_NO_SUCH_OBJECT;
          }
          break;
        }
      }

      if(eRetVal != e_RDY){
        eRetVal = e_NO_SUCH_OBJECT;
      }
    }
  }
  return eRetVal;
}

EMGMResponse C61499Class1ObjectHandler::executeQueryCon(SManagementCMD &paCommand){
  EMGMResponse eRetVal = e_NO_SUCH_OBJECT;
  TConnectionList::Iterator itRunner(m_lstpoConnectionList.begin());

  paCommand.m_oAdditionalParams.reserve(90);
  paCommand.m_oAdditionalParams.clear();

  CIEC_STRING oTempString;
  oTempString.reserve(50);
  oTempString.clear();

  if(CStringDictionary::scm_nInvalidStringId == paCommand.mFirstParam[0]){
    if(CStringDictionary::scm_nInvalidStringId == paCommand.mSecondParam[0]){ // both parameters are *

      for(; itRunner != m_lstpoConnectionList.end(); ++itRunner){
        oTempString.clear();
        oTempString.append(CStringDictionary::getInstance().get(CConnection::extractFBNameId((*itRunner)->getSourceId())));
        oTempString.append(".", 1);
        oTempString.append(CStringDictionary::getInstance().get(CConnection::extractPortNameId((*itRunner)->getSourceId())));

        CSinglyLinkedList<TConnectionID>::Iterator end = ((*itRunner)->getDestinationIds().end());
        for(CSinglyLinkedList<TConnectionID>::Iterator itDestRunner = ((*itRunner)->getDestinationIds().begin()); itDestRunner != end; ++itDestRunner){
          paCommand.m_oAdditionalParams.append("<Connection Source=\"", 20);
          paCommand.m_oAdditionalParams.append(oTempString.getValue(), oTempString.length());
          paCommand.m_oAdditionalParams.append("\" Destination=\"", 15);
          paCommand.m_oAdditionalParams.append(CStringDictionary::getInstance().get(CConnection::extractFBNameId(*itDestRunner)));
          paCommand.m_oAdditionalParams.append(".", 1);
          paCommand.m_oAdditionalParams.append(CStringDictionary::getInstance().get(CConnection::extractPortNameId(*itDestRunner)));
          paCommand.m_oAdditionalParams.append("\" />\n", 5);
        }
      }
      eRetVal = e_RDY;
    }
    else{ //only first parameter is *

      for(; itRunner != m_lstpoConnectionList.end(); ++itRunner){
        oTempString.clear();
        oTempString.append(CStringDictionary::getInstance().get(CConnection::extractFBNameId((*itRunner)->getSourceId())));
        oTempString.append(".", 1);
        oTempString.append(CStringDictionary::getInstance().get(CConnection::extractPortNameId((*itRunner)->getSourceId())));

        CSinglyLinkedList<TConnectionID>::Iterator end = (*itRunner)->getDestinationIds().end();
        for(CSinglyLinkedList<TConnectionID>::Iterator itDestRunner = ((*itRunner)->getDestinationIds().begin()); itDestRunner != end; ++itDestRunner){
          if(*itDestRunner == paCommand.mSecondParam[0]){
            paCommand.m_oAdditionalParams.append(oTempString.getValue(), oTempString.length());
            paCommand.m_oAdditionalParams.append(",", 1);
            break;
          }
        }
      }
      eRetVal = e_RDY;
    }
  }
  else{ //first parameter is != *

    if(CStringDictionary::scm_nInvalidStringId == paCommand.mSecondParam[0]){ // only second parameter is *
      for(; itRunner != m_lstpoConnectionList.end(); ++itRunner){
        if((*itRunner)->getSourceId() == paCommand.mFirstParam[0]){
          CSinglyLinkedList<TConnectionID>::Iterator end = (*itRunner)->getDestinationIds().end();
          for(CSinglyLinkedList<TConnectionID>::Iterator itDestRunner = (*itRunner)->getDestinationIds().begin(); itDestRunner != end; ++itDestRunner){
            paCommand.m_oAdditionalParams.append(CStringDictionary::getInstance().get(CConnection::extractFBNameId(*itDestRunner)));
            paCommand.m_oAdditionalParams.append(".", 1);
            paCommand.m_oAdditionalParams.append(CStringDictionary::getInstance().get(CConnection::extractPortNameId(*itDestRunner)));
            paCommand.m_oAdditionalParams.append(",", 1);
          }
          break; // all destinations of this connection are included
        }
      }
      eRetVal = e_RDY;
    }
    else{ //both parameters are != 0
      for(; itRunner != m_lstpoConnectionList.end(); ++itRunner){
        if((*itRunner)->getSourceId() == paCommand.mFirstParam[0]){
          CSinglyLinkedList<TConnectionID>::Iterator end = (*itRunner)->getDestinationIds().end();
          for(CSinglyLinkedList<TConnectionID>::Iterator itDestRunner = (*itRunner)->getDestinationIds().begin(); itDestRunner != end; ++itDestRunner){
            if((*itDestRunner) == paCommand.mSecondParam[0]){
              eRetVal = e_RDY;
              break; // all destinations of this connection are included
            }
          }
          if(e_RDY == eRetVal){
            break;
          }
        }
      }
    }
  }

  if(!paCommand.m_oAdditionalParams.empty()){
    paCommand.m_oAdditionalParams.assign(paCommand.m_oAdditionalParams.getValue(), static_cast<TForteUInt16>(paCommand.m_oAdditionalParams.length() - 1));
  }

  return eRetVal;
}

EMGMResponse C61499Class1ObjectHandler::queryTypeList(SManagementCMD &pa_oCommand, CTypeLib::CTypeEntry * pa_LibTypeListStart){
  EMGMResponse eRetVal = e_NO_SUCH_OBJECT;

  pa_oCommand.m_oAdditionalParams.clear();

  CTypeLib::CTypeEntry *poToFirst = pa_LibTypeListStart;
  if(poToFirst != 0){
    pa_oCommand.m_oAdditionalParams.append(CStringDictionary::getInstance().get(((CTypeLib::CTypeEntry *) poToFirst)->getTypeNameId()));
    while(poToFirst->m_poNext != 0){
      poToFirst = poToFirst->m_poNext;
      pa_oCommand.m_oAdditionalParams.append(",", 1);
      pa_oCommand.m_oAdditionalParams.append(CStringDictionary::getInstance().get(((CTypeLib::CTypeEntry *) poToFirst)->getTypeNameId()));
    }
    eRetVal = e_RDY;
  }
  return eRetVal;
}

EMGMResponse C61499Class1ObjectHandler::queryTypeVersion(TForteUInt32, char*){
  EMGMResponse eRetVal = e_NO_SUCH_OBJECT;
  return eRetVal;
}

#endif

