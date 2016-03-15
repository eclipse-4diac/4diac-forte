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

const std::string CLMSEV3ProcessInterface::scmModeID("mode");

const std::string CLMSEV3ProcessInterface::scmEnableID("enable");
const std::string CLMSEV3ProcessInterface::scmResetID("reset");
const std::string CLMSEV3ProcessInterface::scmPWMID("pwm");
const std::string CLMSEV3ProcessInterface::scmSPEEDID("speed");
const std::string CLMSEV3ProcessInterface::scmStopID("stop");
const std::string CLMSEV3ProcessInterface::scmPositionID("position");
const std::string CLMSEV3ProcessInterface::scmRotID("rot");

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

bool CLMSEV3ProcessInterface::initialise(bool paIsInput){
  bool retVal = false;
  std::vector<std::string> paramsList(generateParameterList());

  if (0 != paramsList.size()){
    std::string ioType = paramsList[1];

    if(scmLEDID == ioType){ // led parameter format = emb.led.[right | left].[red | green]
      retVal = setupLED(paramsList, paIsInput);
    }else  if(scmSensorID == ioType){ // boolean sensor parameter format = in[X].sensor.[Y]
      retVal = setupSensor(paramsList, paIsInput);
    }
    else if(scmSensorWID == ioType){ //word sensor parameter format = in[X].sensorw.[Y]
      retVal = setupSensorW(paramsList, paIsInput);
    }
    else if(scmButtonID == ioType){ //button parameter format = emb.button.[UP | DOWN | LEFT | RIGHT | ENTER | BACKSPACE]
      retVal = setupButton(paramsList[2], paIsInput);
    }
    else if(scmMotorID == ioType){ //pwm parameter format = out[X].motor.[]
      retVal = setupMotor(paramsList, paIsInput);
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

/* READ AND WRITE FUNCTIONS */

bool CLMSEV3ProcessInterface::readPin(){
  bool retVal = false;
  if(mFile.is_open() || (NULL != mstButtonVariables && -1 != mstButtonVariables->nmFd)){
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
        }else{
          //TODO: react to bad return
        }
        break;
      }
    }

  }
  return retVal;
}

bool CLMSEV3ProcessInterface::writePin(){
  bool retVal = false;
  std::cout << "Writing PIN\n";
  if(mFile.is_open()){
    std::cout << "File is open\n";
    switch (mnTypeOfIO){
      case LED:{
        int val = (false != OUT_X()) ? 255 : 0; //if true set the led to full glowing
        std::cout << "Writing led " << val << "\n";
        mFile << val;
        //TODO check if writing worked
        retVal = true;
        break;
      }
      case MOTOR_ENABLE:{
        std::string val = (false != OUT_X()) ? "run-direct" : "stop";
        std::cout << "Writing enable " << val << "\n";
        mFile << val;
        //TODO check if writing worked
        retVal = true;
        break;
      }
      case MOTOR_RESET:{
        if (true == OUT_X()){
          mFile << "reset";
          std::cout << "Writing " << "reset" << "\n";
          //TODO check if writing worked
          retVal = true;
          break;
        }
      }
    }
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::readWord(){
  bool retVal = false;
  bool internalChecker;

  if (SENSORW_VALUE == mnTypeOfIO || MOTOR_PWM == mnTypeOfIO || MOTOR_SPEED == mnTypeOfIO || MOTOR_ROT == mnTypeOfIO ||
      MOTOR_POSITION == mnTypeOfIO){ //this should be a double, but for testing is here
    TForteInt32 val;
    internalChecker = readNumberFromFile(&val);
    if (true == internalChecker){
      IN_W() = (TForteWord) (val);
      retVal = true;
    }else{
      std::cout << "Could not read word\n";
    }
  }else if (SENSORW_MODE == mnTypeOfIO){
    std::string mode;
    std::getline(mFile, mode);
    TForteWord counter = 0;
    for (std::vector<std::string>::iterator it = mModes.begin(); it < mModes.end(); it++, counter++){
      if (mode == *it){
        IN_W() = counter;
        retVal = true;
      }
    }
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::writeWord(){
  bool retVal = false;
  if (mFile.is_open()){
    TForteWord val = OUT_W();
    switch (mnTypeOfIO){
      case MOTOR_PWM:{
        TForteInt16 finalVal = (TForteInt16) val;
        if (finalVal >= -100 && finalVal <= 100){
          mFile << finalVal;
          //TODO: Check if write worked
          retVal = true;
        }
        break;
      }
      case MOTOR_STOP:{
        std::string finalVal = "";
        switch(val){
          case 0:{
            finalVal = "coast";
            break;
          }
          case 1:{
            finalVal = "brake";
            break;
          }
          case 2:{
            finalVal = "hold";
            break;
          }
        }
        if ("" != finalVal){
          mFile << finalVal;
        }
        break;
      }
      case SENSORW_MODE:{
        if (val < mModes.size()){
          mFile << mModes.at(val);
          retVal = true;
        }
      }
    }
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::readDouble(){
  bool retVal = false;
  /*bool internalChecker;
  TForteInt32 val;
  internalChecker = readNumberFromFile(&val);
  if (true == internalChecker){
    IN_D() = (TForteDWord) (val);
    std::cout << "Reading integer: " <<  IN_D() <<  "\n";
    retVal = true;
  }*/
  return retVal;
}

/* INTIALIZATION FUNCTIONS **/

bool CLMSEV3ProcessInterface::setupLED(const std::vector<std::string> &paParamList, bool paIsInput){
  bool retVal = false;
  if (false == paIsInput){
    std::string sysFileName("/sys/class/leds/ev3:" + paParamList[2] + ":" + paParamList[3] + ":ev3dev/brightness"); //format in ev3dev is= ev3:[left | right]:[red | green]:ev3dev
    mFile.open(sysFileName.c_str(), std::fstream::out); //TODO change this when fully switching to C++11 for LMS EV3
    if(mFile.is_open()){
      std::cout << "File " << sysFileName << " opened\n";
      if (true == writePin()){ // initialize output with default value (i.e, should be false)
        mnTypeOfIO = LED;
        retVal = true;
      }
    }else{
      //TODO:
    }
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupSensor(const std::vector<std::string> &paParamList, bool paIsInput){
  bool retVal = false;

  if (true == paIsInput){ //Output for sensor is not allowed
    if (true == setupSensorValue(paParamList)){
      mnTypeOfIO = SENSOR;
      retVal = true;
    }
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupSensorW(const std::vector<std::string> &paParamList, bool paIsInput){
  bool retVal = false;

  if (scmModeID == paParamList[2]){ //mode of sensor can be input or output
    if (true == setupSensorMode(paParamList)){
      mnTypeOfIO = SENSORW_MODE;
      retVal = true;
    }
  }else{ //value of sensor
    if (true == paIsInput){
      if (true == setupSensorValue(paParamList)){
        mnTypeOfIO = SENSORW_VALUE;
        retVal = true;
      }
    }
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupSensorValue(const std::vector<std::string> &paParamList){
  bool internalCheck;
  bool retVal = false;
  std::string sysFileName;
  std::string basePath = "/sys/class/lego-sensor/sensor";
  std::stringstream helperStringStream;
  int helperInteger = -1;

  helperInteger = findNumberFromPort(basePath, paParamList[0]);
  if(-1 != helperInteger){
    helperStringStream << helperInteger;
    basePath += helperStringStream.str();
    sysFileName = basePath + "/num_values";

    mFile.open(sysFileName.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3
    if(mFile.is_open()){
      std::cout << "File " << sysFileName << " opened\n";
      TForteInt32 aux;
      internalCheck = readNumberFromFile(&aux);
      mFile.close();
      if (true == internalCheck){
        std::cout << "File " << sysFileName << " closed\n";
        if (0 <= aux){ //a negative value is not possible for num_values file
          helperStringStream.str(paParamList[2]);
          helperStringStream >> helperInteger; //TODO: check value

          if (helperInteger <= (aux - 1)){
            sysFileName = basePath + "/value" + helperStringStream.str();
            mFile.open(sysFileName.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3
            if(mFile.is_open()){
              std::cout << "File " << sysFileName << " opened\n";
              retVal = true;
            }else{
              std::cout << "File " << sysFileName << " not opened\n";
            }
          }
        }
      }
    }
  }else{
    std::cout << "File " << sysFileName << " not opened\n";
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupSensorMode(const std::vector<std::string> &paParamList){
  bool retVal;
  std::string helperString;
  std::string basePath = "/sys/class/lego-sensor/sensor";
  std::stringstream helperStringStream;
  int helperInteger = -1;

  helperInteger = findNumberFromPort(basePath, paParamList[0]);
  if(-1 != helperInteger){
    helperStringStream << helperInteger;
    basePath += helperStringStream.str();
    helperString = basePath + "/modes";

    mFile.open(helperString.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3
    if(mFile.is_open()){
      std::cout << "File " << helperString << " opened\n";
      while(std::getline(mFile, helperString, ' ')){
        mModes.push_back(helperString);
      }
      mFile.close();
      std::cout << "File closed\n";
      helperString = basePath + "/mode";
      mFile.open(helperString.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3
      if(mFile.is_open()){
        std::cout << "File " << helperString << " opened\n";
        retVal = true;
      }else{
        std::cout << "File " << helperString << " not opened\n";
      }
    }
  }else{
    std::cout << "File " << helperString << " not opened\n";
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupButton(const std::string &paParam, bool paIsInput){
  bool retVal = false;

  if (true == paIsInput){
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
        mnTypeOfIO = BUTTON;
        retVal = true;
      }
    }
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupMotor(const std::vector<std::string>& paParamList, bool paIsInput){
  bool retVal = false;
  std::string sysFileName = "/sys/class/tacho-motor/motor";
  std::stringstream number;
  int sensorNumber = -1;

  if (3 == paParamList.size()){
    sensorNumber = findNumberFromPort(sysFileName, paParamList[0]);
    if(sensorNumber != -1){
      number << sensorNumber;
      if (paIsInput){
        if (scmPWMID == paParamList[2]){
          sysFileName += number.str() + "/duty_cycle";
          mnTypeOfIO = MOTOR_PWM;
        }else if (scmSPEEDID == paParamList[2]){
          sysFileName += number.str() + "/speed";
          mnTypeOfIO = MOTOR_SPEED;
        }else if (scmRotID == paParamList[2]){
          sysFileName += number.str() + "/count_per_rot";
          mnTypeOfIO = MOTOR_ROT;
        }else if (scmPositionID == paParamList[2]){
          sysFileName += number.str() + "/position";
          mnTypeOfIO = MOTOR_POSITION;
        }
        mFile.open(sysFileName.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3

      }else{
        if (scmEnableID == paParamList[2]){
          sysFileName += number.str() + "/command";
          mnTypeOfIO = MOTOR_ENABLE;
        }else if (scmResetID == paParamList[2]){
          sysFileName += number.str() + "/command";
          mnTypeOfIO = MOTOR_RESET;
        }else if (scmPWMID == paParamList[2]){
          sysFileName += number.str() + "/duty_cycle_sp";
          mnTypeOfIO = MOTOR_PWM;
        }else if (scmStopID == paParamList[2]){
          sysFileName += number.str() + "/stop";
          mnTypeOfIO = MOTOR_STOP;
        } //TODO: if the type if not valid, it would try to open the folder "/sys/class/tacho-motor/motor"
        mFile.open(sysFileName.c_str(), std::fstream::out); //TODO change this when fully switching to C++11 for LMS EV3
      }

      if(mFile.is_open()){
        std::cout << "File " << sysFileName << " opened with parameter" << PARAMS().getValue() << "\n";
        retVal = true;
      }else{
        mnTypeOfIO = UNDEFINED;
        std::cout << "File " << sysFileName << " not opened with parameter" << PARAMS().getValue()<< "\n";
      }
    }
  }
  return retVal;
}

/* END OF INTIALIZATION FUNCTIONS **/

/* HELPER FUNCTIONS */

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

std::vector<std::string> CLMSEV3ProcessInterface::generateParameterList(){
  std::stringstream streamBuf(std::string(PARAMS().getValue()));
  std::string segment;
  std::vector<std::string> retVal;

  while(std::getline(streamBuf, segment, '.')){ //seperate the PARAMS input by '.' for easier processing
    retVal.push_back(segment);
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::readNumberFromFile(TForteInt32* paResult){
  bool retVal = false;
  if (mFile.is_open()){
    std::string read;
    mFile.clear();
    mFile.seekg(0, std::ios::beg);
    if (std::getline(mFile, read)){
      if (!mFile.fail()){
        std::stringstream number(read);
        number >> *paResult;  //TODO: check value of paResult
        retVal = true;
      }else{
        std::cout << "readNumberFromFile: mFile.fail()\n";
        std::string result;
        result = ((mFile.rdstate() & std::ifstream::failbit ) != 0 ) ? "true" : "false";
        std::cout << "readNumberFromFile: fail bit = " << result << "\n";
        result = ((mFile.rdstate() & std::ifstream::badbit ) != 0 ) ? "true" : "false";
        std::cout << "readNumberFromFile: bad bit = " << result << "\n";
        result = ((mFile.rdstate() & std::ifstream::eofbit ) != 0 ) ? "true" : "false";
        std::cout << "readNumberFromFile: eof bit = " << result << "\n";
        result = ((mFile.rdstate() & std::ifstream::goodbit ) != 0 ) ? "true" : "false";
        std::cout << "readNumberFromFile: good bit = " << result << "\n";
        //TODO: Check if error
      }
    }
  }
  return retVal;
}

/* END OF HELPER FUNCTIONS */