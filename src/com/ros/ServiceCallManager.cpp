/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ben Schneider
 *      - initial implementation and documentation
 *******************************************************************************/

#include "ServiceCallManager.h"
#include "TRIGGER_SERVICE_CLIENT.h"
#include <ros/ros.h>
#include <std_srvs/Trigger.h>

//TODO add locking for the vector..

DEFINE_HANDLER(CServiceCallManager);

CServiceCallManager::CServiceCallManager(CDeviceExecution& paDeviceExecution) : CExternalEventHandler(paDeviceExecution),
    CThread(/* long stacksize , 3500*/){
  start();
}

CServiceCallManager::~CServiceCallManager(){
  end();
}

void CServiceCallManager::enableHandler(){
}

void CServiceCallManager::disableHandler(){
  ros::shutdown();
}

void CServiceCallManager::setPriority(int){
}

int CServiceCallManager::getPriority() const{
  return 0;
}

void CServiceCallManager::startChain(CEventSourceFB *pa_poECStartF){
  if(0 != pa_poECStartF){
    startNewEventChain(pa_poECStartF);
  }
}

void CServiceCallManager::queueServiceCall(FORTE_TRIGGER_SERVICE_CLIENT* pa_serviceClientPtr){
  m_callerVector.push_back(pa_serviceClientPtr);
}

void CServiceCallManager::queueConnectWait(FORTE_TRIGGER_SERVICE_CLIENT* pa_serviceClientPtr){
  m_connectVector.push_back(pa_serviceClientPtr);
}

void CServiceCallManager::run(){
  while(isAlive() && ros::ok()){

    if(!m_connectVector.empty()){
      m_connectVector.front()->waitForServer();
      m_connectVector.erase(m_connectVector.begin());
    }

    if(!m_callerVector.empty()){
      m_callerVector.front()->callService();
      m_callerVector.erase(m_callerVector.begin());
    }
    //TODO maybe sleep for a small amount of time?
    //this.waitfor(std::chrono::milliseconds(1));
    //this.sleepfor();
  }
}
