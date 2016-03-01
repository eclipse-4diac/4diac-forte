/*******************************************************************************
 * Copyright (c) 2015, 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Alois Zoitl - initial API and implementation and/or initial documentation
 *    Jose Cabral - expansion of capabilities
 *******************************************************************************/

#include "processinterface.h"
#include <sstream>
#include <iostream>
#include <linux/input.h>
#include <fcntl.h>

const std::string CLMSEV3ProcessInterface::scmLEDID("led");
const std::string CLMSEV3ProcessInterface::scmSensorID("sensor");
const std::string CLMSEV3ProcessInterface::scmSensorWID("sensorw");
const std::string CLMSEV3ProcessInterface::scmButtonID("button");
const std::string CLMSEV3ProcessInterface::scmMotorID("motor");
const std::string CLMSEV3ProcessInterface::scmPWMID("pwm");
const std::string CLMSEV3ProcessInterface::scmSPEEDID("speed");

CLMSEV3ProcessInterface::CLMSEV3ProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
                                                                    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData){
  mFile.rdbuf()->pubsetbuf(NULL, 0); //disable buffer to avoid latency
  mnTypeOfIO = UNDEFINED;
  mstButtonVariables = NULL;
  mnNoOfBits = 0;
}

CLMSEV3ProcessInterface::~CLMSEV3ProcessInterface(){
  if (NULL != mstButtonVariables){
    delete mstButtonVariables;
  }
}

bool CLMSEV3ProcessInterface::initialise(bool paInput){
  bool retVal = false;
  std::vector<std::string> paramsList(generateParameterList());

  if (0 != paramsList.size()){
    std::string ioType = paramsList.front();

    if(paInput){
      if(scmSensorID == ioType){ // boolean sensor parameter format = sensor.in[X]
        retVal = setupSensor(paramsList[1]);
        mnTypeOfIO = SENSOR;
      }
      else if(scmButtonID == ioType){ //button parameter format = button.[UP | DOWN | LEFT | RIGHT | ENTER | BACKSPACE]
        retVal = setupButton(paramsList[1]);
        mnTypeOfIO = BUTTON;
      }
      else if(scmSensorWID == ioType){ //word sensor parameter format = sensorw.in[X]
        retVal = setupSensorW(paramsList[1]);
        mnTypeOfIO = SENSORW;
      }
      else if(scmPWMID == ioType){ //pwm parameter format = pwm.out[X]
        retVal = setupPWM(paramsList[1], true);
        mnTypeOfIO = PWM;
      }else if(scmSPEEDID == ioType){ //speed parameter format = speed.out[X]
        retVal = setupSpeed(paramsList[1]);
        mnTypeOfIO = SPEED;
      }
    }
    else{
      if(scmLEDID == paramsList.front()){ // led parameter format = led.[right | left].[red | green]
        retVal = setupLED(paramsList);
        mnTypeOfIO = LED;
      }
      else if(scmMotorID == ioType){ //motor parameter format = motor.out[X].enable
        retVal = setupMotor(paramsList);
        mnTypeOfIO = MOTOR;
      }
      else if(scmPWMID == ioType){ //pwm parameter format = pwm.out[X]
        retVal = setupPWM(paramsList[1], false);
        mnTypeOfIO = PWM;
      }
    }
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::deinitialise(){
  if(mFile.is_open()){
    mFile.close();
  }
  if (NULL != mstButtonVariables){
    delete mstButtonVariables;
  }
  return true;
}

bool CLMSEV3ProcessInterface::readPin(){
  bool retVal = false;
  if(mFile.is_open() || -1 != mstButtonVariables->nmFd){

    switch (mnTypeOfIO){
      case SENSOR: {
        char binData = 0;
        mFile.clear();
        mFile.seekg(0, std::ios::beg);
        mFile.read(&binData, 1);
        //TODO check if reading worked
        IN_X() = ('0' != binData) ? true : false;
        retVal = true;
        break;
      }
      case BUTTON: {
        unsigned char buf[96];
        for(int i = 0; i < 96; i++){
          buf[i] = 0;
        }
        if(0 < ioctl(mstButtonVariables->nmFd, EVIOCGKEY(96), buf)){
          retVal = true;
          IN_X() = !(buf[mstButtonVariables->nmbutton / 8] & (1 << (mstButtonVariables->nmbutton % 8))) ? true : false;
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
    switch (mnTypeOfIO){
      case LED:{
        int val = (false != OUT_X()) ? 255 : 0; //if true set the led to full glowing
        mFile << val;
        //TODO check if writing worked
        retVal = true;
        break;
      }
      case MOTOR:{
        std::string val = (false != OUT_X()) ? "run-direct" : "stop"; //if true set the led to full glowing
        mFile << val;
        //TODO check if writing worked
        retVal = true;
        break;
      }
    }
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::readWord(){
  bool retVal = false;
  TForteWord val = -1;
  val = readWordCore();
  if (-1 != val){
    if (SPEED == mnTypeOfIO){
      IN_W() = (val * 360) / mCountPerRot; //degree per seconds
      std::cout << "Writing word to output: " <<  (val * 360) / mCountPerRot <<  "\n";
    }else{
      IN_W() = val;
    }
    retVal = true;
  }
  return retVal;
}

TForteWord CLMSEV3ProcessInterface::readWordCore(){
  TForteWord val = -1;
  if (mFile.is_open()){
    std::cout << "Reading word!\n";
    char sensorData[7];
    for (int i = 0; i < 7; i++){
      sensorData[i] = 0;
    }
    std::string binDataString;
    mFile.clear();
    mFile.seekg(0, std::ios::beg);
    mFile.read(sensorData, 6);
    //TODO check if reading worked
    sensorData[7] = '\0';
    binDataString.assign(sensorData);
    std::stringstream number(binDataString);
    number >> val;  //TODO: check value of val
    for (int i = 0; i < 7; i++)
    {
      std::cout << std::dec << sensorData[i];
    }
    std::cout << "\nBinDataSting: " << binDataString << "\nnumber: " <<  number << "\nval:" << val << "\n";
  }

  return val;

}

bool CLMSEV3ProcessInterface::writeWord(){
  bool retVal = false;
  if (mFile.is_open()){
    int val = OUT_W();
    mFile << val;
    //TODO: Check if write worked
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

bool CLMSEV3ProcessInterface::setupLED(const std::vector<std::string> &paParamList){
  bool retVal = false;

  std::string sysFileName("/sys/class/leds/ev3:" + paParamList[1] + ":" + paParamList[2] + ":ev3dev"); //format in ev3dev is= ev3:[left | right]:[red | green]:ev3dev

  sysFileName += "/brightness";

  mFile.open(sysFileName.c_str(), std::fstream::out); //TODO change this when fully switching to C++11 for LMS EV3
  if(mFile.is_open()){
    retVal = writePin(); // initialize output with default value (i.e, should be false)
  }

  return retVal;
}

bool CLMSEV3ProcessInterface::setupSensor(const std::string &paParam){
  bool retVal = false;
  std::string sysFileName;
  std::string basePath = "/sys/class/lego-sensor/sensor";
  std::stringstream number;
  int sensorNumber = -1;

  sensorNumber = findNumberFromPort(basePath, paParam);
  if(sensorNumber != -1){
    number << sensorNumber;
    sysFileName += number.str() + "/value0"; //for now use the value file for getting the value. espeically for touch this seams to be the better option. Needs check for other sensors!!;
    mFile.open(sysFileName.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3
    if(mFile.is_open()){
      retVal = true;
    }
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupSensorW(const std::string &paParam){
  return setupSensor(paParam);
}

bool CLMSEV3ProcessInterface::setupPWM(const std::string &paParam, bool paInput){
  bool retVal = false;
  std::string sysFileName = "/sys/class/tacho-motor/motor";
  std::stringstream number;
  int sensorNumber = -1;

  sensorNumber = findNumberFromPort(sysFileName, paParam);
  if(sensorNumber != -1){
    number << sensorNumber;

    if (paInput){
      sysFileName += number.str() + "/duty_cycle";
      mFile.open(sysFileName.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3
    }else{
      sysFileName += number.str() + "/duty_cycle_sp";
      mFile.open(sysFileName.c_str(), std::fstream::out); //TODO change this when fully switching to C++11 for LMS EV3
    }

    if(mFile.is_open()){
      std::cout << "File " << sysFileName << " opened\n";
      retVal = true;
    }else{
      std::cout << "File " << sysFileName << " not opened\n";
    }

  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupButton(const std::string &paParam){
  bool retVal = false;

  mstButtonVariables = new st_ButtonVariables;

  mstButtonVariables->nmFd = open("/dev/input/event0", O_RDONLY);
  if (-1 !=   mstButtonVariables->nmFd){
    //Button constant found in linux/input.h
    if (0 == paParam.compare("up")){
      mstButtonVariables->nmbutton = 103;
    }else if (0 == paParam.compare("down")){
      mstButtonVariables->nmbutton = 108;
    }else if (0 == paParam.compare("left")){
      mstButtonVariables->nmbutton = 105;
    }else if (0 == paParam.compare("right")){
      mstButtonVariables->nmbutton = 106;
    }else if (0 == paParam.compare("enter")){
      mstButtonVariables->nmbutton = 28;
    }else if (0 == paParam.compare("backspace")){
      mstButtonVariables->nmbutton = 14;
    }

    if (0 != mstButtonVariables->nmbutton) {
      retVal = true;
    }
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupMotor(const std::vector<std::string>& paParamList){
  bool retVal = false;
  std::string sysFileName = "/sys/class/tacho-motor/motor";
  std::stringstream number;
  int sensorNumber = -1;

  sensorNumber = findNumberFromPort(sysFileName, paParamList[1]);
  if(sensorNumber != -1){
    number << sensorNumber;
    sysFileName += number.str() + "/command"; //TODO: Check for the thirs parameter of motor. Now, the only possible third parameter from motor is "enable", but many more can later be added
    mFile.open(sysFileName.c_str(), std::fstream::out); //TODO change this when fully switching to C++11 for LMS EV3
    if(mFile.is_open()){
      std::cout << "File " << sysFileName << " opened\n";
      retVal = true;
    }else{
      std::cout << "File " << sysFileName << " not opened\n";
    }
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupSpeed(const std::string& paParam){
  bool retVal = false;
  std::string sysFileName;
  std::string basePath = "/sys/class/tacho-motor/motor";
  std::stringstream number;
  int sensorNumber = -1;

  sensorNumber = findNumberFromPort(basePath, paParam);
  if(sensorNumber != -1){
    number << sensorNumber;
    sysFileName = basePath + number.str() + "/count_per_rot";
    mFile.open(sysFileName.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3
    if(mFile.is_open()){
      std::cout << "File " << sysFileName << " opened\n";
      TForteWord auxWord = readWordCore();
      if (-1 != auxWord){
        mFile.close();
        std::cout << "File " << sysFileName << " closed\n";
        mCountPerRot = auxWord;
        sysFileName = basePath + number.str() + "/speed";
        mFile.open(sysFileName.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3
        if(mFile.is_open()){
          std::cout << "File " << sysFileName << " opened\n";
          retVal = true;
        }else{
          std::cout << "File " << sysFileName << " not opened\n";
        }
      }
    }
  }else{
    std::cout << "File " << sysFileName << " not opened\n";
  }
  return retVal;
}

int CLMSEV3ProcessInterface::findNumberFromPort(const std::string &paBasePath, const std::string &paEv3Port){
  int retVal = -1;
  std::fstream mAddressFile;
  std::string fullPath;
  char port[5];
  int portLenght = paEv3Port.length();

  if (portLenght < 5){ //Only possible values are inX and outX according to input and output ports of the EV3
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
  }
  return retVal;
}
