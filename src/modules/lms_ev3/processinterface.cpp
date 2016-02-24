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
#include <iostream>
#include <linux/input.h>
#include <fcntl.h>

const std::string CLMSEV3ProcessInterface::scmLEDId("led");
const std::string CLMSEV3ProcessInterface::scmSensorID("Sensor");
const std::string CLMSEV3ProcessInterface::scmButtonID("Button");

CLMSEV3ProcessInterface::CLMSEV3ProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
                    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData){
  mFile.rdbuf()->pubsetbuf(NULL, 0); //disable buffer to avoid latency
  typeOfInput = 0;
  button = 0;
  fileDescriptor = -1;
}

CLMSEV3ProcessInterface::~CLMSEV3ProcessInterface(){
}

bool CLMSEV3ProcessInterface::initialise(bool paInput){
  bool retVal = false;
  std::vector<std::string> paramsList(generateParameterList());

  //TODO check length of vector
  if(paInput){
    if(scmSensorID == paramsList.front()){ // led parameter format = Sensor.in[X]
      retVal = setupSensor(paramsList[1]);
      typeOfInput = 1; //SENSOR
    }
    else if(scmButtonID == paramsList.front()){ //button parameter format = Button.[UP | DOWN | LEFT | RIGHT | ENTER | BACKSPACE]
      retVal = setupButton(paramsList);
      typeOfInput = 2; //BUTTON
    }
  }
  else{
    if(scmLEDId == paramsList.front()){ // led parameter format = led.[red | green].[right | left]
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
  if(mFile.is_open() || -1 != fileDescriptor){

    switch (typeOfInput){
      case 1: { //sensor
        char sensorData[10];
        std::string binDataString;
        mFile.clear();
        mFile.seekg(0, std::ios::beg);
        mFile.read(sensorData, 9);
        sensorData[10] = '\0';
        binDataString.assign(sensorData);
        //TODO check if reading worked
        IN_X() = (binDataString.substr(0, 4) == "2550") ? true : false; //2250 is the max value for ultrasonic sensor. Here just for test
        break;
      }
      case 2: { //button
        unsigned char buf[96];

        for(int i = 0; i < 96; i++){
          buf[i] = 0;
        }

        if(0 < ioctl(fileDescriptor, EVIOCGKEY(96), buf)){
          retVal = true;
          IN_X() = !(buf[button / 8] & (1 << (button % 8))) ? true : false;
        }
        break;
      }
    }

  }
  return retVal;
}

bool CLMSEV3ProcessInterface::writePin(){
  bool retVal = false;
  if(mFile.is_open()){
    int val = (false != OUT_X()) ? 255 : 0; //if true set the led to full glowing
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

  while(std::getline(streamBuf, segment, '.')){ //seperate the PARAMS input by '.' for easier processing
    retVal.push_back(segment);
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupLEDOutput(const std::vector<std::string> &paParamList){
  bool retVal = false;

  std::string sysFileName("/sys/class/leds/ev3:" + paParamList[2] + ":" + paParamList[1] + ":ev3dev"); //format in ev3dev is= ev3:[left | right]:[red | green]:ev3dev

  sysFileName += "/brightness";

  mFile.open(sysFileName.c_str(), std::fstream::out); //TODO change this when fully switching to C++11 for LMS EV3

  if(mFile.is_open()){
    retVal = writePin(); // initialize output with default value (i.e, should be false)
  }

  return retVal;
}

bool CLMSEV3ProcessInterface::setupSensor(const std::string &paParam){
  bool retVal = false;

  std::string sysFileName(getSensorBasePath(paParam));

  if(sysFileName.size()){
    sysFileName += "/value0"; //for now use the value file for getting the value. espeically for touch this seams to be the better option. Needs check for other sensors!!
    mFile.open(sysFileName.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3
    if(mFile.is_open()){
      retVal = true;
    }

  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupButton(const std::vector<std::string> &paParamList){
  bool retVal = false;

  std::string state;
  fileDescriptor = open("/dev/input/event0", O_RDONLY);
  if (-1 != fileDescriptor){
    //Button constant found in linux/input.h
    if (0 == paParamList[1].compare("UP")){
      button = 103;
    }else if (0 == paParamList[1].compare("DOWN")){
      button = 108;
    }else if (0 == paParamList[1].compare("LEFT")){
      button = 105;
    }else if (0 == paParamList[1].compare("RIGHT")){
      button = 106;
    }else if (0 == paParamList[1].compare("ENTER")){
      button = 28;
    }else if (0 == paParamList[1].compare("BACKSPACE")){
      button = 14;
    }

    if (0 != button) {
      retVal = true;
    }
  }
  return retVal;
}

std::string CLMSEV3ProcessInterface::getSensorBasePath(const std::string &paParam){
  std::string retVal;
  std::string basePath = "/sys/class/lego-sensor/sensor";
  int sensorNumber = -1;
  std::stringstream number;

  sensorNumber = findNumberFromPort(basePath, paParam);
  if(sensorNumber != -1){
    retVal = basePath;
    number << sensorNumber;
    retVal += number.str();
  }

  return retVal;
}

int CLMSEV3ProcessInterface::findNumberFromPort(const std::string &paBasePath, const std::string &paEv3Port){
  int retVal = -1;
  std::fstream mAddressFile;
  std::string fullPath;
  char port[5];
  int portLenght = paEv3Port.length();

  for(int i = 0; i < 255; i++){ //TODO: check maximum value of i
    std::stringstream number;
    number << i;
    fullPath = paBasePath + number.str() + "/address";

    mAddressFile.open(fullPath.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3ca

    if(mAddressFile.is_open()){
      mAddressFile.clear();
      mAddressFile.seekg(0, std::ios::beg);
      mAddressFile.read(port, portLenght);
      mAddressFile.close();
      port[portLenght] = '\0';

      if(paEv3Port.compare(port) == 0){
        retVal = i;
        break;
      }

    }
  }
  return retVal;
}
