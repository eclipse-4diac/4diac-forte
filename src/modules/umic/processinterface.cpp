/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "processinterface.h"
#include <string>
#include <sstream>
#include <vector>
#include <criticalregion.h>

#include <umic_dio.h>
#include <umic_relay.h>
#include <umic_led.h>

const std::string CUMICProcessInterface::scmDIOID("dio");
#if UMIC_LED_ENABLED
const std::string CUMICProcessInterface::scmLEDID("led");
#endif
#if UMIC_RELAY_ENABLED
const std::string CUMICProcessInterface::scmRELAYID("relay");
#endif
bool CUMICProcessInterface::smDIOInitialized = false;
uint8_t CUMICProcessInterface::smIODirections = 0;
CSyncObject CUMICProcessInterface::directionsMutex = CSyncObject();

const char * const CUMICProcessInterface::scmOK = "OK";
const char * const CUMICProcessInterface::scmPinInUse = "Pin already in use by other FB";
const char * const CUMICProcessInterface::scmNotInitialised = "FB not initialized";
const char * const CUMICProcessInterface::scmError = "Error";
const char * const CUMICProcessInterface::scmCouldNotRead = "Could not read";
const char * const CUMICProcessInterface::scmCouldNotWrite = "Could not write";

CUMICProcessInterface::CUMICProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), mIOType(UNDEFINED), mNumber(0){
}

CUMICProcessInterface::~CUMICProcessInterface(){
    deinitialise();
}

bool CUMICProcessInterface::initialise(bool paIsInput){

  bool retVal = false;
  std::stringstream streamBuf(std::string(PARAMS().getValue()));
  std::string segment;
  std::vector<std::string> paramsList;
  unsigned int number;

  while(std::getline(streamBuf, segment, '.')){ //separate the PARAMS input by '.' for easier processing
    paramsList.pushBack(segment);
  }

  STATUS() = scmNotInitialised;

  if(2 == paramsList.size()){ //TYPE.NUMBER
    std::string ioType = paramsList[0];

    std::stringstream ss(paramsList[1]);
    ss >> number;

    if(scmDIOID == ioType){
      bool statusCorrect = true;
      if(!CUMICProcessInterface::smDIOInitialized){
        DEVLOG_INFO("Global Initializing Digital I/O\n");
        if(0 > umic_dio_init()){
          statusCorrect = false;
          DEVLOG_ERROR("Global Initialization of Digital I/O FAILED!\n");
        }
        else{
          DEVLOG_INFO("Global Initialization of Digital I/O SUCCEED!\n");
        }
      }
      if(statusCorrect && 8 >= number && 0 != number){
        CCriticalRegion(CUMICProcessInterface::directionsMutex);
        uint8_t currentDirection;
        if(paIsInput){ // 0 for input
          currentDirection = static_cast<uint8_t>(CUMICProcessInterface::smIODirections & ~(1 << (number - 1)));
        }
        else{ //1 for output
          currentDirection = static_cast<uint8_t>(CUMICProcessInterface::smIODirections | (1 << (number - 1)));
        }
        if(0 > umic_dio_set_direction(0, currentDirection)){
          DEVLOG_ERROR("Initializing Digital I/O FAILED!");
        }
        else{
          CUMICProcessInterface::smIODirections = currentDirection;
          mIOType = DIO;
          retVal = true;
        }
      }
    }
#if UMIC_LED_ENABLED
    else  if(scmLEDID == ioType && !paIsInput){
      if(7 >= number){
        retVal = true;
        mIOType = LED;
      }
    }
#endif
#if UMIC_RELAY_ENABLED
    else if(scmRELAYID == ioType && !paIsInput){
      if(2 >= number && 0 != number){
        retVal = true;
        mIOType = RELAY;
      }
    }
#endif
  }

  if(true == retVal){
    STATUS() = scmOK;
    mNumber = static_cast<uint8_t>(number);
  }
  else{
    DEVLOG_ERROR("The FB with PARAMS() = '%s' couldn't be initialized. PARAMS is not well defined.\n", PARAMS().getValue());
  }
  return retVal;
}

bool CUMICProcessInterface::deinitialise(){
  //TODO: The global initialization umic_dio_init doesn't have its counterpart umic_dio_release
  mNumber = 0;
  return true;
}

bool CUMICProcessInterface::readPin(){
  bool retVal = false;
  if(DIO == mIOType){
    int32_t currentValue = umic_dio_get_input(0);
    if (0 > currentValue){
      DEVLOG_ERROR("Error reading PIN %d\n", mNumber);
      STATUS() = scmCouldNotRead;
    }else{
      IN_X() = ( (currentValue) & (1 << (mNumber - 1)) ) ? true : false;
      STATUS() = scmOK;
      retVal = true;
    }
  }else{
    STATUS() = scmNotInitialised;
  }
  return retVal;
}

bool CUMICProcessInterface::writePin(){
  bool retVal = false;
  if(UNDEFINED != mIOType){
    if(DIO == mIOType){
      int32_t currentValue = umic_dio_get_input(0);
      if(0 > currentValue){
        DEVLOG_ERROR("Error reading PIN to later write on it%d\n", mNumber);
        STATUS() = scmCouldNotRead;
      }
      else{
        if(false != OUT_X()){
          currentValue |= 1 << (mNumber - 1); //set bit
        }
        else{
          currentValue &= ~(1 << (mNumber - 1)); //clear bit
        }
        if(0 > umic_dio_set_output(0, static_cast<uint8_t>(currentValue))){
          DEVLOG_ERROR("Error writing IO%d\n", mNumber);
          STATUS() = scmCouldNotWrite;
        }
        else{
          STATUS() = scmOK;
          retVal = true;
        }
      }
    }
#if UMIC_LED_ENABLED
    else if(LED == mIOType){
      int32_t res;
      if(false != OUT_X()){
        res = umic_led_set(mNumber);
      }
      else{
        res = umic_led_clr(mNumber);
      }
      if(0 > res){
        DEVLOG_ERROR("Error writing LED%d\n", mNumber);
        STATUS() = scmCouldNotWrite;
      }
      else{
        STATUS() = scmOK;
        retVal = true;
      }
    }
#endif
#if UMIC_LED_ENABLED
    else if(RELAY == mIOType){
      int32_t res;
      if(false != OUT_X()){
        res = umic_relay_on(static_cast<uint8_t>(mNumber - 1));
      }
      else{
        res = umic_relay_off(static_cast<uint8_t>(mNumber - 1));
      }
      if(0 > res){
        DEVLOG_ERROR("Error writing RELAY%d\n", mNumber);
        STATUS() = scmCouldNotWrite;
      }
      else{
        STATUS() = scmOK;
        retVal = true;
      }
    }
#endif
  }
  else{
    STATUS() = scmNotInitialised;
  }
  return retVal;
}
