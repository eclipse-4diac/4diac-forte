/*******************************************************************************
 * Copyright (c) 2016 - 2017 fortiss GmbH
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

#include "ROSManager.h"

#include <ros/ros.h>

//------------------------- CROSLayer::CROSManager -------------------------

DEFINE_HANDLER(CROSManager);

CROSManager::CROSManager(CDeviceExecution& paDeviceExecution) : CExternalEventHandler(paDeviceExecution),
    CThread(/* long stacksize , 3500*/){
  start();
}

CROSManager::~CROSManager(){
  end();
}

void CROSManager::enableHandler(){

}

void CROSManager::disableHandler(){
  ros::shutdown();
}

void CROSManager::setPriority(int){
//FIXME adjust thread priority correctly
}

int CROSManager::getPriority() const{
  return 0;
}

void CROSManager::startChain(CEventSourceFB *pa_poECStartF){
  if(0 != pa_poECStartF){
    startNewEventChain(pa_poECStartF);
  }
}

void CROSManager::run(){
  while(isAlive() && ros::ok()){
    ros::spinOnce();
  }
}

std::string CROSManager::ciecStringToStdString(const CIEC_STRING &pa_DataInput){
  return std::string(pa_DataInput.getValue());
}

CIEC_STRING CROSManager::stdStringToCiecString(const std::string &pa_inputString){
  int stringLength = static_cast<int>(std::strlen(pa_inputString.c_str()));
  CIEC_STRING tmpString;
  tmpString.assign(pa_inputString.c_str(), static_cast<TForteUInt16>(stringLength));
  return tmpString;
}
