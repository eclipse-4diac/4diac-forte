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

CXqueryClientLayer::CXqueryClientLayer(CComLayer* pa_poUpperLayer, CBaseCommFB* pa_poComFB) : CComLayer(pa_poUpperLayer, pa_poComFB){
  sfd = -1;
  command = NULL;
  host = NULL;
  port = NULL;
  usr = NULL;
  psw = NULL;
  dbName = NULL;
}

CXqueryClientLayer::~CXqueryClientLayer() {
}

bool CXqueryClientLayer::parseParameters(char *pa_acLayerParameter){
  bool retVal = true;
  host = strtok(pa_acLayerParameter, scmParameterSeperator);
  port = strtok(NULL, scmParameterSeperator);
  dbName = strtok(NULL, scmParameterSeperator);
  usr = strtok(NULL, scmParameterSeperator);
  psw = strtok(NULL, scmParameterSeperator);
  if(NULL == host || NULL == port || NULL == usr || NULL == psw || NULL == dbName){
    retVal = false;
  }
  return retVal;
}

forte::com_infra::EComResponse CXqueryClientLayer::openConnection(char *pa_acLayerParameter){
  EComResponse retVal = e_InitTerminated;
  if(e_Client == m_poFb->getComServiceType() && m_poFb->getNumRD() == 1 && m_poFb->getNumSD() == 1 && parseParameters(pa_acLayerParameter)){
    sfd = basex_connect(host, port);
    if (sfd == -1) {
      DEVLOG_ERROR("Can not connect to BaseX server.\n");
    }else{
      int rc = basex_authenticate(sfd, usr, psw);
      if (rc == -1) {
        DEVLOG_ERROR("Access to DB denied.\n");
      }else{
        m_poFb->QO() = true;
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
  char *r = NULL;
  char *i = NULL;
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

forte::com_infra::EComResponse CXqueryClientLayer::sendData(void *pa_pvData, unsigned int pa_unSize){
  EComResponse retVal = e_ProcessDataOk;
  if(0 == pa_unSize){
    retVal = e_ProcessDataInvalidObject;
  }else{
    CIEC_ANY const *SDs(static_cast<TConstIEC_ANYPtr>(pa_pvData));
    if(SDs[0].getDataTypeID() == CIEC_ANY::e_STRING){
      command = ((CIEC_STRING &)SDs[0]).getValue();
      getExtEvHandler<CXqueryHandler>().registerLayer(this);
    }else{
      retVal = e_ProcessDataInvalidObject;
    }
  }
  return retVal;
}

forte::com_infra::EComResponse CXqueryClientLayer::recvData(const void *pa_pvData, unsigned int pa_unSize){
  EComResponse retVal = e_Nothing;
  if(pa_unSize > 0) {
    m_poFb->getRDs()[0].fromString(static_cast<const char *>(pa_pvData));
    m_poFb->interruptCommFB(this);
    retVal = e_ProcessDataOk;
  }
  return retVal;
}

forte::com_infra::EComResponse CXqueryClientLayer::processInterrupt(){
  return e_ProcessDataOk;
}
