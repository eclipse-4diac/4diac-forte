/*******************************************************************************
 * Copyright (c) 2015, 2016 fortiss GmbH
 *               2019 Robert Promok
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl - initial API and implementation and/or initial documentation
 *    Jose Cabral - expansion of capabilities
 *    Robert Promok - added functions
 *******************************************************************************/

#include "processinterface.h"
#include <sstream>
#include <linux/input.h>
#include <fcntl.h>

const std::string CLMSEV3ProcessInterface::scmLEDID("led");
const std::string CLMSEV3ProcessInterface::scmSensorID("sensor");
const std::string CLMSEV3ProcessInterface::scmSensorWID("sensorw");
const std::string CLMSEV3ProcessInterface::scmButtonID("button");
const std::string CLMSEV3ProcessInterface::scmMotorID("motor");

const std::string CLMSEV3ProcessInterface::scmModeID("mode");

const std::string CLMSEV3ProcessInterface::scmEnableID("enable");
const std::string CLMSEV3ProcessInterface::scmEnableSpeedID("enableSpeed");
const std::string CLMSEV3ProcessInterface::scmRelPositionID("relPos");
const std::string CLMSEV3ProcessInterface::scmAbsPositionID("absPos");

const std::string CLMSEV3ProcessInterface::scmResetID("reset");
const std::string CLMSEV3ProcessInterface::scmPWMID("pwm");
const std::string CLMSEV3ProcessInterface::scmSPEEDID("speed");
const std::string CLMSEV3ProcessInterface::scmSPEEDSETPOINTID("speedsp");
const std::string CLMSEV3ProcessInterface::scmPOSITIONSETPOINTID("positionsp");
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
  mFile.rdbuf()->pubsetbuf(0, 0); //disable buffer to avoid latency
  mnTypeOfIO = UNDEFINED;
  mstButtonVariables = 0;
  mnNoOfBits = 0;
}

CLMSEV3ProcessInterface::~CLMSEV3ProcessInterface(){
  if (0 != mstButtonVariables){
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
    DEVLOG_ERROR("The FB with PARAMS() = '%s' couldn't be initialized. PARAMS is not well defined.", PARAMS().getValue());
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::deinitialise(){
  if(mFile.is_open()){
    mFile.close();
  }
  if (0 != mstButtonVariables){
    delete mstButtonVariables;
  }
  STATUS() = scmOK;
  return true;
}

/* READ AND WRITE FUNCTIONS */

bool CLMSEV3ProcessInterface::readPin(){
  bool retVal = false;
  if(mFile.is_open() || (0 != mstButtonVariables && -1 != mstButtonVariables->nmFd)){
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
    STATUS() = scmCouldNotRead;
  }

  if (!retVal){
    DEVLOG_ERROR("Reading pin failed. The FB with PARAMS() = '%s' couldn't be read.", PARAMS().getValue());
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
        mFile << val;
        break;
      }
      case MOTOR_ENABLE:{
        std::string val = (false != OUT_X()) ? "run-direct" : "stop";
        mFile << val;
        break;
      }
      case MOTOR_ENABLE_SPEED:{
        std::string val = (false != OUT_X()) ? "run-forever" : "stop";
        mFile << val;
        break;
      }
      case MOTOR_RELATIVE_POSITION:{
        std::string val = (false != OUT_X()) ? "run-to-rel-pos" : "stop";
        mFile << val;
        break;
      }
      case MOTOR_ABSOLUTE_POSITION:{
        std::string val = (false != OUT_X()) ? "run-to-abs-pos" : "stop";
        mFile << val;
        break;
      }
      case MOTOR_RESET:{
        if (true == OUT_X()){
          mFile << "reset";
        }
        break;
      }
      default:{
        STATUS() = scmCouldNotWrite;
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
    STATUS() = scmCouldNotWrite;
  }

  if (!retVal){
    DEVLOG_ERROR("Writing pin failed. The FB with PARAMS() = '%s' couldn't be written.", PARAMS().getValue());
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::readWord(){
  bool retVal = false;

  if (SENSORW_VALUE == mnTypeOfIO || MOTOR_PWM == mnTypeOfIO || MOTOR_SPEED == mnTypeOfIO || MOTOR_ROT == mnTypeOfIO || MOTOR_SPEEDSETPOINT == mnTypeOfIO ) {
    TForteInt32 val;
    int internalChecker;
    internalChecker = readNumberFromFile(&val);
    if (0 == internalChecker){
      IN_W() = static_cast<TForteWord>(val);
      STATUS() = scmOK;
      retVal = true;
    }else if (1 == internalChecker){
      STATUS() = scmCouldNotRead;
      DEVLOG_ERROR("Reading word failed. The FB with PARAMS() = '%s' is not initialized.", PARAMS().getValue());
    }else{
      STATUS() = scmCouldNotRead;
    }
  }else if (SENSORW_MODE == mnTypeOfIO){
    if (mFile.is_open()){
      std::string mode;
      mFile.clear();
      mFile.seekg(0, std::ios::beg);
      std::getline(mFile, mode);
      if(mFile.fail()){
        STATUS() = scmCouldNotRead;
      }else{
        TForteWord counter = 0;
        std::vector<std::string>::iterator it;
        for (it = mModes.begin(); it < mModes.end(); ++it, counter++){
          if (mode == *it){
            IN_W() = counter;
            STATUS() = scmOK;
            retVal = true;
            break;
          }
        }
        if (it == mModes.end()){
          STATUS() = scmCouldNotRead;
        }
      }
    }else{
      STATUS() = scmCouldNotRead;
    }
  }else{
    STATUS() = scmCouldNotRead;
  }
  if (!retVal){
    DEVLOG_ERROR("Reading word failed. The FB with PARAMS() = '%s' couldn't be read.", PARAMS().getValue());
  }

  return retVal;
}

bool CLMSEV3ProcessInterface::writeWord(){
  bool retVal = false;

  if (mFile.is_open()){
    bool writeAttempted = false;
    mFile.clear();
    mFile.seekp(0, std::ios::beg);
    TForteWord val = OUT_W();
    switch (mnTypeOfIO){
      case MOTOR_PWM:{
        TForteInt16 finalVal = static_cast<TForteInt16>(val);
        if (-100 <= finalVal && 100 >= finalVal){
          mFile << finalVal;
          writeAttempted = true;
        }else{
          STATUS() = scmCouldNotWrite;
        }
        break;
      }
      case MOTOR_SPEEDSETPOINT:{
        TForteInt16 finalVal = static_cast<TForteInt16>(val);
        if (-30000 <= finalVal && 30000 >= finalVal){
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
    }
  }else{
    STATUS() = scmNotInitialised;
  }

  if (!retVal){
    DEVLOG_ERROR("Writing word failed. The FB with PARAMS() = '%s' couldn't be written.", PARAMS().getValue());
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::readDWord(){
  bool retVal = false;

  if (MOTOR_POSITION == mnTypeOfIO){
    int internalChecker;
    TForteInt32 val;
    internalChecker = readNumberFromFile(&val);
    if (0 == internalChecker){
      IN_D() = static_cast<TForteDWord>(val);
      STATUS() = scmOK;
      retVal = true;
    }else if (1 == internalChecker){
      STATUS() = scmNotInitialised;
      DEVLOG_ERROR("Reading double word failed. The FB with PARAMS() = '%s' is not initialized.\n", PARAMS().getValue());
    }else{
      STATUS() = scmCouldNotRead;
    }
  }
  else if (MOTOR_POSITIONSETPOINT == mnTypeOfIO){
    int internalChecker;
    TForteInt32 val;
    internalChecker = readNumberFromFile(&val);
    if (0 == internalChecker){
      IN_D() = static_cast<TForteDWord>(val);
      STATUS() = scmOK;
      retVal = true;
    }else if (1 == internalChecker){
      STATUS() = scmNotInitialised;
      DEVLOG_ERROR("Reading double word failed. The FB with PARAMS() = '%s' is not initialized.\n", PARAMS().getValue());
    }else{
      STATUS() = scmCouldNotRead;
    }
  }
  if (!retVal){
    DEVLOG_ERROR("Reading double word failed. The FB with PARAMS() = '%s' couldn't be read.\n", PARAMS().getValue());
  }

  return retVal;
}

bool CLMSEV3ProcessInterface::writeDWord(){
  bool retVal = false;

  if (mFile.is_open()){
    bool writeAttempted = false;
    mFile.clear();
    mFile.seekp(0, std::ios::beg);
    TForteDWord val = OUT_D();
    switch (mnTypeOfIO){
      case MOTOR_POSITION:{
        TForteInt32 finalVal = static_cast<TForteInt32>(val);
        if (CIEC_INT::scm_nMinVal <= finalVal && CIEC_INT::scm_nMaxVal >= finalVal){
          mFile << finalVal;
          writeAttempted = true;
        }else{
          STATUS() = scmCouldNotWrite;
        }
        break;
      }
      case MOTOR_POSITIONSETPOINT:{
        TForteInt32 finalVal = static_cast<TForteInt32>(val);
        if (CIEC_INT::scm_nMinVal <= finalVal && CIEC_INT::scm_nMaxVal >= finalVal){
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
    }
  }else{
    STATUS() = scmNotInitialised;
  }

  if (!retVal){
    DEVLOG_ERROR("Writing double word failed. The FB with PARAMS() = '%s' couldn't be written.", PARAMS().getValue());
  }
  return retVal;
}

/* INTIALIZATION FUNCTIONS **/

bool CLMSEV3ProcessInterface::setupLED(const std::vector<std::string> &paParamList, bool paIsInput){
  bool retVal = false;
  if (false == paIsInput){
    std::string sysFileName("/sys/class/leds/ev3:" + paParamList[2] + ":" + paParamList[3] + ":ev3dev/brightness"); //format in ev3dev is= ev3:[left | right]:[red | green]:ev3dev
    mFile.open(sysFileName.c_str(), std::fstream::out); //TODO change this when fully switching to C++11 for LMS EV3
    if(mFile.is_open()){
      if (true == writePin()){ // initialize output with default value (i.e, should be false)
        mnTypeOfIO = LED;
        retVal = true;
      }
    }else{
      DEVLOG_ERROR("Opening file %s failed.", sysFileName.c_str());
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
  bool retVal = false;
  std::string sysFileName;
  std::string basePath = "/sys/class/lego-sensor/sensor";
  std::stringstream helperStringStream;
  int helperInteger = findNumberFromPort(basePath, paParamList[0]);

  if(-1 != helperInteger){
    helperStringStream << helperInteger;
    if (!helperStringStream.fail()){
      basePath += helperStringStream.str();
      sysFileName = basePath + "/num_values";

      mFile.open(sysFileName.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3
      if(mFile.is_open()){
        bool internalCheck;
        TForteInt32 aux;
        internalCheck = readNumberFromFile(&aux);
        mFile.close();
        if (0 == internalCheck){
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
                  retVal = true;
                }else{
                  DEVLOG_ERROR("Opening file %s failed.", sysFileName.c_str());
                }
              }
            }else{
              DEVLOG_ERROR("Internal error trying to store a stringstream in an int.");
            }
          }
        }else{
          DEVLOG_ERROR("File %s was opened, but couln't be read.", sysFileName.c_str());
        }
      }else{
        DEVLOG_ERROR("Opening file %s failed.", sysFileName.c_str());
      }
    }else{
      DEVLOG_ERROR("Internal error trying to store an int in a stringstream");
    }
  }else{
    DEVLOG_ERROR("Port %s wasn't found.", paParamList[0].c_str());
  }
  return retVal;
}

bool CLMSEV3ProcessInterface::setupSensorMode(const std::vector<std::string> &paParamList, bool paIsInput){
  bool retVal = false;
  std::string helperString;
  std::string basePath = "/sys/class/lego-sensor/sensor";
  std::stringstream helperStringStream;
  int helperInteger = findNumberFromPort(basePath, paParamList[0]);

  if(-1 != helperInteger){
    helperStringStream << helperInteger;
    if (!helperStringStream.fail()){
      basePath += helperStringStream.str();
      helperString = basePath + "/modes";

      mFile.open(helperString.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3
      if(mFile.is_open()){
        while(std::getline(mFile, helperString, ' ')){
          mModes.push_back(helperString);
        }
        mFile.close();
        helperString = basePath + "/mode";
        mFile.open(helperString.c_str(), paIsInput ? std::fstream::in : std::fstream::out); //TODO change this when fully switching to C++11 for LMS EV3
        if(mFile.is_open()){
          retVal = true;
        }else{
          DEVLOG_ERROR("Opening file %s failed.", helperString.c_str());
        }
      }else{
        DEVLOG_ERROR("Opening file %s failed.", helperString.c_str());
      }
    }else{
      DEVLOG_ERROR("Internal error trying to store an int in a stringstream");
    }
  }else{
    DEVLOG_ERROR("Port %s wasn't found.", paParamList[0].c_str());
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
  std::string sysFileName = "/sys/class/tacho-motor/motor";
  std::stringstream number;

  if (3 == paParamList.size()){
    int sensorNumber = findNumberFromPort(sysFileName, paParamList[0]);
    if(sensorNumber != -1){
      number << sensorNumber;
      if (!number.fail()){
        bool defaultType = false;
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
          }else if (scmSPEEDSETPOINTID == paParamList[2]){
            sysFileName += number.str() + "/speed_sp";
            mnTypeOfIO = MOTOR_SPEEDSETPOINT;
          }else if (scmPOSITIONSETPOINTID == paParamList[2]){
            sysFileName += number.str() + "/position_sp";
            mnTypeOfIO = MOTOR_POSITIONSETPOINT;
          }else{
            defaultType = true;
          }
        }else{
          if (scmEnableID == paParamList[2]){
            sysFileName += number.str() + "/command";
            mnTypeOfIO = MOTOR_ENABLE;
          }else if (scmEnableSpeedID == paParamList[2]){
            sysFileName += number.str() + "/command";
            mnTypeOfIO = MOTOR_ENABLE_SPEED;
          }else if (scmRelPositionID == paParamList[2]){
            sysFileName += number.str() + "/command";
            mnTypeOfIO = MOTOR_RELATIVE_POSITION;
          }else if (scmAbsPositionID == paParamList[2]){
            sysFileName += number.str() + "/command";
            mnTypeOfIO = MOTOR_ABSOLUTE_POSITION;
          }else if (scmResetID == paParamList[2]){
            sysFileName += number.str() + "/command";
            mnTypeOfIO = MOTOR_RESET;
          }else if (scmPWMID == paParamList[2]){
            sysFileName += number.str() + "/duty_cycle_sp";
            mnTypeOfIO = MOTOR_PWM;
          }else if (scmStopID == paParamList[2]){
            sysFileName += number.str() + "/stop_action";
            mnTypeOfIO = MOTOR_STOP;
          }else if (scmPositionID == paParamList[2]){
            sysFileName += number.str() + "/position";
            mnTypeOfIO = MOTOR_POSITION;
          }else if (scmSPEEDSETPOINTID == paParamList[2]){
            sysFileName += number.str() + "/speed_sp";
            mnTypeOfIO = MOTOR_SPEEDSETPOINT;
          }else if (scmPOSITIONSETPOINTID == paParamList[2]){
            sysFileName += number.str() + "/position_sp";
            mnTypeOfIO = MOTOR_POSITIONSETPOINT;
          }else{
            defaultType = true;
          }
        }
        if (!defaultType){
          mFile.open(sysFileName.c_str(), paIsInput ? std::fstream::in : std::fstream::out); //TODO change this when fully switching to C++11 for LMS EV3
          if(mFile.is_open()){
            retVal = true;
          }else{
            mnTypeOfIO = UNDEFINED;
            DEVLOG_ERROR("Opening file %s failed.", sysFileName.c_str());
          }
        }
      }else{
        DEVLOG_ERROR("Internal error trying to store an int in a stringstream");
      }
    }else{
      DEVLOG_ERROR("Port %s wasn't found.", paParamList[0].c_str());
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
  int portLength = static_cast<int>(paEv3Port.length());

  if (portLength < 5){ //Only possible values are inX and outX according to input and output ports of the EV3
    for(int i = 0; i < 255; i++){ //TODO: check maximum value of i
      std::stringstream number;
      number << i;
      if (!number.fail()){
        fullPath = paBasePath + number.str() + "/address";
        mAddressFile.open(fullPath.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3ca
        if(mAddressFile.is_open()){
          char port[16];
          mAddressFile.clear();
          mAddressFile.seekg(0, std::ios::beg);
          mAddressFile.read(port, sizeof(port));
          auto len = mAddressFile.gcount();
          if (len > 1) {
            if (paEv3Port.at(portLength-1) == port[len-2]) {
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
      number >> *paResult;
      if (number.fail()){
        retVal = 2; //Could not read
      }
    }else{
      retVal = 2; //Could not read
    }
  }else{
    retVal = 1; //Not initialized
  }
  return retVal;
}

/* END OF HELPER FUNCTIONS */
