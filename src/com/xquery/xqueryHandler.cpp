/*********************************************************************
* Copyright (c) 2017 fortiss GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "xqueryHandler.h"

#include <commfb.h>
#include <criticalregion.h>
#include "forte_string.h"
#include "../../arch/devlog.h"
#include "xqueryClientLayer.h"
extern "C" {
#include <basexdbc.h>
}

DEFINE_HANDLER(CXqueryHandler);

CSyncObject CXqueryHandler::smXqueryMutex = CSyncObject();
forte::arch::CSemaphore CXqueryHandler::mStateSemaphore = forte::arch::CSemaphore();

CXqueryHandler::CXqueryHandler(CDeviceExecution& paDeviceExecution) : CExternalEventHandler(paDeviceExecution){
  result = nullptr;
  info = nullptr;
}

CXqueryHandler::~CXqueryHandler(){
  if(isAlive()){
    {
      CCriticalRegion sectionState(smXqueryMutex);
      setAlive(false);
      resumeSuspend();
    }
    free(result);
    free(info);
    end();
  }
}

void CXqueryHandler::enableHandler(){
  if(!isAlive()){
    start();
  }
}

void CXqueryHandler::disableHandler(){
  if(isAlive()){
    setAlive(false);
    resumeSuspend();
    end();
  }
}

void CXqueryHandler::setPriority(int pa_prio){
  //  FIXME adjust thread priority correctly
}

int CXqueryHandler::getPriority() const{
  return 0;
}

int CXqueryHandler::registerLayer(CXqueryClientLayer* paLayer){
  mXqueryFBList.pushBack(paLayer);
  enableHandler();
  resumeSuspend();
  return 0;
}

void CXqueryHandler::run(){
  while(isAlive()){
    if(mXqueryFBList.isEmpty()){
        selfSuspend();
    }else{
      TXqueryFBContainer::Iterator it = mXqueryFBList.begin();
      CXqueryClientLayer *xc = *it;
      if(xc->getSfd() > -1){
        int rc = basex_execute(xc->getSfd(), xc->getCommand(), &result, &info);  
        if(rc == -1){ // general (i/o or the like) error
          DEVLOG_ERROR("An error occured during execution of '%s'.\n", xc->getCommand());
          free(result);
          free(info);
        }
        if(rc > 0){ // database error while processing command
          DEVLOG_ERROR("Processing of '%s' failed.\n", xc->getCommand());
        }else{
          if(e_Nothing != xc->recvData(result, strlen(result))){
             startNewEventChain(xc->getCommFB());
          }
        }
      }else{
        DEVLOG_ERROR("Connection seems to be lost, query not sent.\n");
      }
      mXqueryFBList.popFront();
    }
  }
}

void CXqueryHandler::resumeSuspend(){
    mStateSemaphore.inc();
}

void CXqueryHandler::selfSuspend(){
  mStateSemaphore.waitIndefinitely();
}
