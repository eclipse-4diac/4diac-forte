/*******************************************************************************
 * Copyright (c) 2015, 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Gerd Kainz, Alois Zoitl - initial API and implementation and/or initial documentation
 *    Alois Zoitl - changed from i2c-dev to simple read writes
 *******************************************************************************/

#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include "i2cprocessinterface.h"

const char * const CI2CProcessInterface::scmOK = "OK";
const char * const CI2CProcessInterface::scmNotInitialised = "Not initialised";
const char * const CI2CProcessInterface::scmInvalidParam = "Invalid parameter";
const char * const CI2CProcessInterface::scmCouldNotRead = "Could not read value";
const char * const CI2CProcessInterface::scmCouldNotWrite = "Could not write value";

CI2CProcessInterface::CI2CProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), mFd(-1), mValueAddress(-1){
}

CI2CProcessInterface::~CI2CProcessInterface(){
}

bool CI2CProcessInterface::initialise(bool ){
  bool retVal = false;
  STATUS() = scmNotInitialised;
  std::vector<std::string> paramsList(generateParameterList());

  if(3 == paramsList.size()){
    CIEC_INT param;
    param.fromString(paramsList[1].c_str()); //TODO check return value
    int deviceAddress = param;
    param.fromString(paramsList[2].c_str()); //TODO check return value
    mValueAddress = param;

    std::string devPath("/dev/i2c-");
    devPath += paramsList[0];

    STATUS() = scmNotInitialised;

    mFd = open(devPath.c_str(), O_RDWR);
    if(0 <= mFd){
      if(0 <= ioctl(mFd, scmSetSlaveId, deviceAddress)){
        STATUS() = scmOK;
        retVal = true;
      }
    }
  }
  return retVal;
}

bool CI2CProcessInterface::deinitialise(){
  close(mFd);
  STATUS() = scmOK;
  return true;
}

bool CI2CProcessInterface::readPin(){
  bool retVal = false;
  TForteByte res;

  if(1 == read(mFd, &res, 1)){
    IN_X() = (res & (1 << mValueAddress)) ? true : false;
    retVal = true;
    STATUS() = scmOK;
  }
  else{
    STATUS() = scmCouldNotRead;
  }

  return retVal;
}

bool CI2CProcessInterface::writePin(){
  bool retVal = false;
  TForteByte byteValue;

  if(1 == read(mFd, &byteValue, 1)){
    if(OUT_X()){
      byteValue = static_cast<TForteByte>(byteValue | static_cast<TForteByte>(1 << mValueAddress));
    }
    else{
      byteValue = byteValue & static_cast<TForteByte>(~(1 << mValueAddress));
    }
    if(1 == write(mFd, &byteValue, 1)){
      retVal = true;
      STATUS() = scmOK;
    }else{
      STATUS() = scmCouldNotWrite;
    }
  }else{
    STATUS() = scmCouldNotWrite;
  }
  return retVal;
}

bool CI2CProcessInterface::readWord(){
  bool retVal = true;
  TForteByte readValue[2];
  STATUS() = scmOK;

  //assume in the simple case that a simple read of 2 bytes is sufficient.
  if(2 == read(mFd, readValue, 2)){
    IN_W() = static_cast<TForteWord>(static_cast<TForteWord>(readValue[0]) + (static_cast<TForteWord>(readValue[1]) << 8));
  }else{
    STATUS() = scmCouldNotRead;
    retVal = false;
  }

  return retVal;
}

bool CI2CProcessInterface::writeWord(){
  bool retVal = true;
  TForteByte writeValue[3] = { mValueAddress };
  STATUS() = scmOK;

  writeValue[1] = static_cast<TForteByte>(OUT_W().operator unsigned short int());
  writeValue[2] = static_cast<TForteByte>(OUT_W().operator unsigned short int() >> 8);

  if(3 != write(mFd, writeValue, 3)){
    STATUS() = scmCouldNotWrite;
    retVal = false;
  }

  return retVal;
}

//TODO this is duplicated code from LMS EV3 process interface
std::vector<std::string> CI2CProcessInterface::generateParameterList(){
  std::stringstream streamBuf(std::string(PARAMS().getValue()));
  std::string segment;
  std::vector<std::string> retVal;

  while(std::getline(streamBuf, segment, '.')){ //Separate the PARAMS input by '.' for easier processing
    retVal.push_back(segment);
  }
  return retVal;
}
