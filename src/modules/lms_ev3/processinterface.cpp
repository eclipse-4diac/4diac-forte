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

const char * const CLMSEV3ProcessInterface::scmOK = "OK";
const char * const CLMSEV3ProcessInterface::scmNotInitialised = "Not initialized";
const char * const CLMSEV3ProcessInterface::scmCouldNotRead = "Could not read";
const char * const CLMSEV3ProcessInterface::scmCouldNotWrite = "Could not write";

//TODO: QO is not changed in this file, should be changed according to the execution?

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
  STATUS() = scmNotInitialised;

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
  if (true == retVal){
    STATUS() = scmOK;
  }else{
    std::cout << "----------ERROR: Not initialized\n";
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
  STATUS() = scmOK;
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
        if (mFile.fail()){
          STATUS() = scmCouldNotRead;
        }else{
          IN_X() = ('0' != binData) ? true : false;
          STATUS() = scmOK;
          retVal = true;
        }
        break;
      }
      case BUTTON: {
        unsigned char buf[96];
        for(int i = 0; i < 96; i++){
          buf[i] = 0;
        }
        if(0 < ioctl(mstButtonVariables->nmFd, EVIOCGKEY(96), buf)){
          IN_X() = !(buf[mstButtonVariables->nmbutton / 8] & (1 << (mstButtonVariables->nmbutton % 8))) ? true : false;
          STATUS() = scmOK;
          retVal = true;
        }else{
          STATUS() = scmCouldNotRead;
        }
        break;
      }
      default:{
        STATUS() = scmCouldNotRead;
        break;
      }
    }
  }else{
    STATUS() = scmNotInitialised;
  }

  if (!retVal){
    std::cout << "----------ERROR: reading pin\n";
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::writePin(){
  bool retVal = false;
  if(mFile.is_open()){
    mFile.clear();
    mFile.seekp(0, std::ios::beg);
    switch (mnTypeOfIO){
      case LED:{
        unsigned int val = (false != OUT_X()) ? 255 : 0; //if true set the led to full glowing
        std::cout << "Writing led " << val << "\n";
        mFile << val;
        break;
      }
      case MOTOR_ENABLE:{
        std::string val = (false != OUT_X()) ? "run-direct" : "stop";
        std::cout << "Writing enable " << val << "\n";
        mFile << val;
        break;
      }
      case MOTOR_RESET:{
        if (true == OUT_X()){
          mFile << "reset";
          std::cout << "Writing " << "reset" << "\n";
          break;
        }
      }
      default:{
        STATUS() = scmCouldNotRead;
        break;
      }
    }
    if (mFile.fail()){
      STATUS() = scmCouldNotWrite;
    }else{
      STATUS() = scmOK;
      retVal = true;
    }
  }else{
    STATUS() = scmNotInitialised;
  }

  if (!retVal){
    std::cout << "----------ERROR: writing pin\n";
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::readWord(){
  bool retVal = false;
  int internalChecker;

  if (SENSORW_VALUE == mnTypeOfIO || MOTOR_PWM == mnTypeOfIO || MOTOR_SPEED == mnTypeOfIO || MOTOR_ROT == mnTypeOfIO ||
      MOTOR_POSITION == mnTypeOfIO){ //this should be a double, but for testing is here
    TForteInt32 val;
    internalChecker = readNumberFromFile(&val);
    if (0 == internalChecker){
      IN_W() = (TForteWord) (val);
      STATUS() = scmOK;
      retVal = true;
    }else if (1 == internalChecker){
      STATUS() = scmNotInitialised;
      std::cout << "----------ERROR: Not initialized\n";
    }else{
      STATUS() = scmCouldNotRead;
      std::cout << "----------ERROR: Could not read\n";
    }
  }else if (SENSORW_MODE == mnTypeOfIO){
    if (mFile.is_open()){
      std::string mode;
      mFile.clear();
      mFile.seekg(0, std::ios::beg);
      std::getline(mFile, mode);
      if(mFile.fail()){
        STATUS() == scmCouldNotRead;
      }else{
        TForteWord counter = 0;
        std::vector<std::string>::iterator it;
        for (it = mModes.begin(); it < mModes.end(); it++, counter++){
          if (mode == *it){
            IN_W() = counter;
            STATUS() = scmOK;
            retVal = true;
            break;
          }
        }
        if (it == mModes.end()){
          STATUS() == scmCouldNotRead;
        }
      }
    }else{
      STATUS() = scmNotInitialised;
    }
  }else{
    STATUS() = scmCouldNotRead;
  }
  if (!retVal){
    std::cout << "----------ERROR: reading word\n";
  }

  return retVal;
}

bool CLMSEV3ProcessInterface::writeWord(){
  bool retVal = false;
  bool writeAttempted = false;
  if (mFile.is_open()){
    mFile.clear();
    mFile.seekp(0, std::ios::beg);
    TForteWord val = OUT_W();
    switch (mnTypeOfIO){
      case MOTOR_PWM:{
        TForteInt16 finalVal = (TForteInt16) val;
        if (finalVal >= -100 && finalVal <= 100){
          mFile << finalVal;
          writeAttempted = true;
        }else{
          STATUS() = scmCouldNotWrite;
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
          default:{
            STATUS() = scmCouldNotWrite;
            break;
          }
        }
        if ("" != finalVal){
          mFile << finalVal;
          writeAttempted = true;
        }
        break;
      }
      case SENSORW_MODE:{
        std::string finalVal = "";
        if (val < mModes.size()){
          finalVal =  mModes[val];
          mFile.clear();
          mFile.seekp(0, std::ios::beg);
          mFile << finalVal;
          writeAttempted = true;
        }else{
          STATUS() = scmCouldNotWrite;
        }
        break;
      }
      default:{
        STATUS() = scmCouldNotWrite;
        break;
      }
    }
    if (!mFile.fail() &&  writeAttempted) {
      STATUS() = scmOK;
      retVal = true;
    }else{
      STATUS() = scmCouldNotWrite;
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
    }
  }else{
    STATUS() = scmNotInitialised;
  }

  if (!retVal){
    std::cout << "----------ERROR writing word\n";
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
    if (true == setupSensorMode(paParamList, paIsInput)){
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
    helperStringStream.clear();
    helperStringStream.seekg(0, std::ios::beg);
    helperStringStream << helperInteger;
    if (!helperStringStream.fail()){
      basePath += helperStringStream.str();
      sysFileName = basePath + "/num_values";

      mFile.open(sysFileName.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3
      if(mFile.is_open()){
        std::cout << "File " << sysFileName << " opened\n";
        TForteInt32 aux;
        internalCheck = readNumberFromFile(&aux);
        mFile.close();
        if (0 == internalCheck){
          std::cout << "File " << sysFileName << " closed\n";
          if (0 <= aux){ //a negative value is not possible for num_values file
            helperStringStream.str(paParamList[2]);
            helperStringStream.clear();
            helperStringStream.seekp(0, std::ios::beg);
            helperStringStream >> helperInteger;
            if (!helperStringStream.fail()){
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
      }
    }
  }else{
    std::cout << "File " << sysFileName << " not opened\n";
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupSensorMode(const std::vector<std::string> &paParamList, bool paIsInput){
  bool retVal;
  std::string helperString;
  std::string basePath = "/sys/class/lego-sensor/sensor";
  std::stringstream helperStringStream;
  int helperInteger = -1;

  helperInteger = findNumberFromPort(basePath, paParamList[0]);
  if(-1 != helperInteger){
    helperStringStream.clear();
    helperStringStream.seekg(0, std::ios::beg);
    helperStringStream << helperInteger;
    if (!helperStringStream.fail()){
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
        mFile.open(helperString.c_str(), paIsInput ? std::fstream::in : std::fstream::out); //TODO change this when fully switching to C++11 for LMS EV3
        if(mFile.is_open()){
          std::cout << "File " << helperString << " opened\n";
          retVal = true;
        }else{
          std::cout << "File " << helperString << " not opened\n";
        }
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
    if (!retVal){
      delete mstButtonVariables;
    }
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupMotor(const std::vector<std::string>& paParamList, bool paIsInput){
  bool retVal = false;
  bool defaultType = false;
  std::string sysFileName = "/sys/class/tacho-motor/motor";
  std::stringstream number;
  int sensorNumber = -1;

  if (3 == paParamList.size()){
    sensorNumber = findNumberFromPort(sysFileName, paParamList[0]);
    if(sensorNumber != -1){
      number.clear();
      number.seekp(0, std::ios::beg);
      number << sensorNumber;
      if (!number.fail()){
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
          }else{
            defaultType = true;
          }
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
            sysFileName += number.str() + "/stop_command";
            mnTypeOfIO = MOTOR_STOP;
          } else{
            defaultType = true;
          }
        }
        if (!defaultType){
          mFile.open(sysFileName.c_str(), paIsInput ? std::fstream::in : std::fstream::out); //TODO change this when fully switching to C++11 for LMS EV3
          if(mFile.is_open()){
            std::cout << "File " << sysFileName << " opened with parameter " << PARAMS().getValue() << "\n";
            retVal = true;
          }else{
            mnTypeOfIO = UNDEFINED;
            std::cout << "File " << sysFileName << " not opened with parameter " << PARAMS().getValue()<< "\n";
          }
        }
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
      number.clear();
      number.seekp(0, std::ios::beg);
      number << i;
      if (!number.fail()){
        fullPath = paBasePath + number.str() + "/address";
        mAddressFile.open(fullPath.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3ca
        if(mAddressFile.is_open()){
          mAddressFile.clear();
          mAddressFile.seekg(0, std::ios::beg);
          mAddressFile.read(port, portLenght);
          if (!mAddressFile.fail()){
            port[portLenght] = '\0';
            if(paEv3Port.compare(port) == 0){
              retVal = i;
              break;
            }
          }
          mAddressFile.close();
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

int CLMSEV3ProcessInterface::readNumberFromFile(TForteInt32* paResult){
  int retVal = 0; //No error
  if (mFile.is_open()){
    std::string read;
    mFile.clear();
    mFile.seekg(0, std::ios::beg);
    std::getline(mFile, read); //TODO: This return the same stream, should it be check? What behavior should it have if file is empty?
    if (!mFile.fail()){
      std::stringstream number(read);
      number.clear();
      number.seekp(0, std::ios::beg);
      number >> *paResult;
      if (number.fail()){
        retVal = 2; //Could not read
      }
    }else{
      retVal = 2; //Could not read
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
    }
  }else{
    retVal = 1; //Not initialized
  }
  return retVal;
}

/* END OF HELPER FUNCTIONS */