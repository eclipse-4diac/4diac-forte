/*********************************************************************
* Copyright (c) 2017 fortiss GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include <extevhan.h>
#include <commfb.h>
#include <string.h>
#include "../../arch/devlog.h"
#include <xqueryClientLayer.h>
#include "xqueryHandler.h"
extern "C" {
#include <basexdbc.h>
}

using namespace forte::com_infra;
const char *CXqueryClientLayer::scmParameterSeperator = " ;:";

CXqueryClientLayer::CXqueryClientLayer(CComLayer* paUpperLayer, CBaseCommFB* paComFB) : CComLayer(paUpperLayer, paComFB){
  sfd = -1;
  command = nullptr;
  host = nullptr;
  port = nullptr;
  usr = nullptr;
  psw = nullptr;
  dbName = nullptr;
}

CXqueryClientLayer::~CXqueryClientLayer() = default;

bool CXqueryClientLayer::parseParameters(char *paLayerParameter){
  bool retVal = true;
  host = strtok(paLayerParameter, scmParameterSeperator);
  port = strtok(nullptr, scmParameterSeperator);
  dbName = strtok(nullptr, scmParameterSeperator);
  usr = strtok(nullptr, scmParameterSeperator);
  psw = strtok(nullptr, scmParameterSeperator);
  if(nullptr == host || nullptr == port || nullptr == usr || nullptr == psw || nullptr == dbName){
    retVal = false;
  }
  return retVal;
}

forte::com_infra::EComResponse CXqueryClientLayer::openConnection(char *paLayerParameter){
  EComResponse retVal = e_InitTerminated;
  if(e_Client == mFb->getComServiceType() && mFb->getNumRD() == 1 && mFb->getNumSD() == 1 && parseParameters(paLayerParameter)){
    sfd = basex_connect(host, port);
    if (sfd == -1) {
      DEVLOG_ERROR("Can not connect to BaseX server.\n");
    }else{
      int rc = basex_authenticate(sfd, usr, psw);
      if (rc == -1) {
        DEVLOG_ERROR("Access to DB denied.\n");
      }else{
        mFb->QO() = CIEC_BOOL(true);
        DEVLOG_INFO("Connected to DB.\n");
        openDB();
        retVal = e_InitOk;
      }
    }
  }else{
     DEVLOG_ERROR("[xquery] supports only CLIENT_1 with ID = xquery[IP:port; dbName; usr; psw]\n");
  }
  return retVal;
}

void CXqueryClientLayer::openDB(){
  char *r = nullptr;
  char *i = nullptr;
  command = new char[strlen(dbName) + 6];
  strcpy(command, "open ");
  strcat(command, dbName);
  int ret = basex_execute(sfd, command, &r, &i);
  if(ret == -1){ // general (i/o or the like) error
    DEVLOG_ERROR("An error occured during opening of DB'.\n");
    free(r);
    free(i);
  }
  if(ret == 1){ // database error while processing command
    DEVLOG_ERROR("Processing of '%s' failed.\n", command);
  }else{
    DEVLOG_INFO("DB %s opend.\n", dbName);
  }
}

void CXqueryClientLayer::closeConnection(){
  if (sfd > -1) {
    basex_close(sfd);
    DEVLOG_INFO("Disonnected from DB.\n");
  }
}

forte::com_infra::EComResponse CXqueryClientLayer::sendData(void *paData, unsigned int paSize){
  EComResponse retVal = e_ProcessDataOk;
  if(0 == paSize){
    retVal = e_ProcessDataInvalidObject;
  }else{
    CIEC_ANY const *SDs(static_cast<TConstIEC_ANYPtr>(paData));
    if(SDs[0].getDataTypeID() == CIEC_ANY::e_STRING){
      command = ((CIEC_STRING &)SDs[0]).getValue();
      getExtEvHandler<CXqueryHandler>().registerLayer(this);
    }else{
      retVal = e_ProcessDataInvalidObject;
    }
  }
  return retVal;
}

forte::com_infra::EComResponse CXqueryClientLayer::recvData(const void *paData, unsigned int paSize){
  EComResponse retVal = e_Nothing;
  if(paSize > 0) {
    mFb->getRDs()[0]->fromString(static_cast<const char *>(paData));
    mFb->interruptCommFB(this);
    retVal = e_ProcessDataOk;
  }
  return retVal;
}

forte::com_infra::EComResponse CXqueryClientLayer::processInterrupt(){
  return e_ProcessDataOk;
}
