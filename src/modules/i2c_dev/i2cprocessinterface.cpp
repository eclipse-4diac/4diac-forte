/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Gerd Kainz, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <stdint.h>
#include <sstream>
#include "i2cprocessinterface.h"

const char * const CI2CProcessInterface::scmInitDeinitOK = "OK";
const char * const CI2CProcessInterface::scmNotInitialised = "Not initialised";
const char * const CI2CProcessInterface::scmInvalidParam = "Invalid parameter";

CI2CProcessInterface::CI2CProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), mFd(-1), mDeviceAddress(-1), mValueAddress(-1){
}

CI2CProcessInterface::~CI2CProcessInterface(){
}

bool CI2CProcessInterface::initialise(bool paInput){
  unsigned long funcs;

  bool retVal = false;
  STATUS() = scmNotInitialised;
  std::vector<std::string> paramsList(generateParameterList());

  if(3 == paramsList.size()){
    CIEC_INT param;
    param.fromString(paramsList[1].c_str());  //TODO check return value
    mDeviceAddress = param;
    param.fromString(paramsList[2].c_str());  //TODO check return value
    mValueAddress = param;

    std::string devPath("/dev/i2c-");
    devPath += paramsList[0];

    STATUS() = scmNotInitialised;

    mFd = open(devPath.c_str(), O_RDWR);
    if(0 <= mFd){
      if(0 <= ioctl(mFd, I2C_FUNCS, &funcs)){  //TODO check if really needed
        if(0 <= ioctl(mFd, I2C_SLAVE, mDeviceAddress)){
          STATUS() = scmInitDeinitOK;
          retVal = true;
        }
      }
    }
  }
  return retVal;
}

bool CI2CProcessInterface::deinitialise(){
  close(mFd);
  STATUS() = scmInitDeinitOK;
  return true;
}

bool CI2CProcessInterface::readPin(){
  int res;

  if(ioctl(mFd, I2C_SLAVE, mDeviceAddress) < 0){
    STATUS() = "Kann net lesen";
    return false;
  }

  res = i2c_smbus_read_byte(mFd);
  if(res < 0){
    STATUS() = "Kann net lesen";
    return false;
  }

  IN_X() = (res & (1 << mValueAddress)) ? true : false;
  STATUS() = "Konnte lesen";
  return true;
}

bool CI2CProcessInterface::writePin(){
  int res;
  uint8_t byteValue;

  if(ioctl(mFd, I2C_SLAVE, mDeviceAddress) < 0){
    STATUS() = "Kann net schreiben";
    return false;
  }

  byteValue = i2c_smbus_read_byte(mFd);
  if(OUT_X()){
    byteValue = byteValue | (uint8_t) (1 << mValueAddress);
  }
  else{
    byteValue = byteValue & (uint8_t) (~(1 << mValueAddress));
  }
  res = i2c_smbus_write_byte(mFd, byteValue);
  if(res < 0){
    STATUS() = "Kann net schreiben";
    return false;
  }
  STATUS() = "Konnte lesen";
  return true;
}

bool CI2CProcessInterface::readWord(){
  int res;

  if(ioctl(mFd, I2C_SLAVE, mDeviceAddress) < 0){
    STATUS() = "Kann net lesen";
    return false;
  }

  res = i2c_smbus_read_word_data(mFd, (uint8_t) mValueAddress);
  if(res < 0){
    STATUS() = "Kann net lesen";
    return false;
  }
  IN_W() = res;
  STATUS() = "Konnte lesen";
  return true;
}

bool CI2CProcessInterface::writeWord(){
  int res;

  if(ioctl(mFd, I2C_SLAVE, mDeviceAddress) < 0){
    STATUS() = "Kann net schreiben";
    return false;
  }

  res = i2c_smbus_write_word_data(mFd, mValueAddress, (uint16_t) OUT_W());
  if(res < 0){
    STATUS() = "Kann net schreiben";
    return false;
  }
  STATUS() = "Konnte schreiben";
  return true;
}

//TODO this is duplicated code from LMS EV3 process interface
std::vector<std::string> CI2CProcessInterface::generateParameterList(){
  std::stringstream streamBuf(std::string(PARAMS().getValue()));
  std::string segment;
  std::vector<std::string> retVal;

  while(std::getline(streamBuf, segment, '.')){   //Separate the PARAMS input by '.' for easier processing
    retVal.push_back(segment);
  }
  return retVal;
}
