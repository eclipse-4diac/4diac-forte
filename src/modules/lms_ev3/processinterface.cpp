/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "processinterface.h"
#include <sstream>

const std::string CLMSEV3ProcessInterface::scmLEDId("led");
const std::string CLMSEV3ProcessInterface::scmSensorID("Sensor");

CLMSEV3ProcessInterface::CLMSEV3ProcessInterface(CResource *paSrcRes,
    const SFBInterfaceSpec *paInterfaceSpec,
    const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData,
    TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData){
  mFile.rdbuf()->pubsetbuf(NULL, 0);    //disable buffer to avoid latency
}

CLMSEV3ProcessInterface::~CLMSEV3ProcessInterface(){
}

bool CLMSEV3ProcessInterface::initialise(bool paInput){
  bool retVal = false;
  std::vector<std::string> paramsList(generateParameterList());

  //TODO check length of vector
  if(paInput){
    if(scmSensorID == paramsList.front().substr(0,scmSensorID.length())){
      retVal = setupSensor(paramsList.front());
    }
  }else{
    if(scmLEDId == paramsList.front()){
      retVal = setupLEDOutput(paramsList);
    }
  }

  return retVal;
}

bool CLMSEV3ProcessInterface::deinitialise(){
  if(mFile.is_open()){
    mFile.close();
  }
  return true;
}

bool CLMSEV3ProcessInterface::readPin(){
  bool retVal = false;
  if(mFile.is_open()){
    char binData = 0;
    mFile.clear();
    mFile.seekg(0, std::ios::beg);
    mFile.read(&binData, 1);
    //TODO check if reading worked
    IN_X() = ('0' != binData) ? true : false;
    retVal = true;
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::writePin(){
  bool retVal = false;
  if(mFile.is_open()){
    int val = (false != OUT_X()) ? 255 : 0;  //if true set the led to full glowing
    mFile << val;
    //TODO check if writing worked
    retVal = true;
  }
  return retVal;
}

std::vector<std::string> CLMSEV3ProcessInterface::generateParameterList(){
  std::stringstream streamBuf(std::string(PARAMS().getValue()));
  std::string segment;
  std::vector<std::string> retVal;

  while(std::getline(streamBuf, segment, '.')){   //seperate the PARAMS input by '.' for easier processing
    retVal.push_back(segment);
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupLEDOutput(const std::vector<std::string> &paParamList){
  bool retVal = false;

  std::string sysFileName("/sys/class/leds/ev3-" + paParamList[2]);

  sysFileName += ("red" == paParamList[1]) ? "0:" : "1:";
  sysFileName += paParamList[1];
  sysFileName += ":ev3dev/brightness";

  mFile.open(sysFileName.c_str(), std::fstream::out);      //TODO change this when fully switching to C++11 for LMS EV3

  if(mFile.is_open()){
    retVal = writePin();  // initialize output with default value (i.e, should be false)
  }

  return retVal;
}

bool CLMSEV3ProcessInterface::setupSensor(const std::string &paParam){
  bool retVal = false;

  std::string sysFileName(getSensorBasePath(*(paParam.rbegin())));
  if(sysFileName.size()){
    sysFileName += "/value0";   //for now use the value file for getting the value. espeically for touch this seams to be the better option. Needs check for other sensors!!
    mFile.open(sysFileName.c_str(), std::fstream::in);      //TODO change this when fully switching to C++11 for LMS EV3
    if(mFile.is_open()){
      retVal = true;
    }
  }
  return retVal;
}

std::string CLMSEV3ProcessInterface::getSensorBasePath(char paSensorPortNum){
  std::string retVal;
  if(('0' <= paSensorPortNum) && (paSensorPortNum < '5')){
    //todo search for the correct sensor port using the port_name file
    retVal = "/sys/class/lego-sensor/sensor";
    retVal += paSensorPortNum;
  }
  return retVal;
}
