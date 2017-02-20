/*******************************************************************************
 * Copyright (c) 2016 - 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Ben Schneider
 *      - initial implementation and documentation
 *******************************************************************************/

#include "ROSActionManager.h"

#include <ros/ros.h>

//	 ------------------------- CROSLayer::CROSManager -------------------------

DEFINE_SINGLETON(CROSActionManager);

CROSActionManager::CROSActionManager() :
    CThread(/* long stacksize , 3500*/){
  start();
}

CROSActionManager::~CROSActionManager(){
  end();
}

void CROSActionManager::enableHandler(){

}

void CROSActionManager::disableHandler(){
  ros::shutdown();
}

void CROSActionManager::setPriority(int pa_prio){
  //	FIXME adjust thread priority correctly
}

int CROSActionManager::getPriority() const{
  return 0;
}

void CROSActionManager::startChain(CEventSourceFB *pa_poECStartF){
  if(0 != pa_poECStartF){
    startNewEventChain(pa_poECStartF);
  }
}

void CROSActionManager::run(){
  while(isAlive() && ros::ok()){
    ros::spinOnce();
  }
}

std::string CROSActionManager::ciecStringToStdString(const CIEC_STRING &pa_DataInput){
  return std::string(pa_DataInput.getValue());
}

CIEC_STRING CROSActionManager::stdStringToCiecString(const std::string pa_inputString){
  int stringLength = std::strlen(pa_inputString.c_str());
  CIEC_STRING tmpString;
  tmpString.assign(pa_inputString.c_str(), static_cast<TForteUInt16>(stringLength));
  return tmpString;
}
