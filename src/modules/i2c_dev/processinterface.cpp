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

#include "processinterface.h"
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

const char * const CProcessInterface::scmInitDeinitOK = "OK";
const char * const CProcessInterface::scmNotInitialised = "Not initialised";

CProcessInterface::CProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), mFd(-1), mDeviceAddress(-1), mValueAddress(-1){
}

CProcessInterface::~CProcessInterface(){
}

bool CProcessInterface::initialise(bool paInput){
  char path[scmBuffer];
  unsigned long funcs;
  char * element;
  int i2CAdapter;


  strcpy(path, PARAMS().getValue());
  element = strtok(path, ".");
  if(NULL == element){
    STATUS() = scmNotInitialised;
    return false;
  }
  else{
    CIEC_INT param;
    param.fromString(element);
    i2CAdapter = param;
  }
  element = strtok(NULL, ".");
  if(NULL == element){
    STATUS() = scmNotInitialised;
    return false;
  }
  else{
    CIEC_INT param;
    param.fromString(element);
    mDeviceAddress = param;
  }
  element = strtok(NULL, ".");
  if(NULL == element){
    STATUS() = scmNotInitialised;
    return false;
  }
  else{
    CIEC_INT param;
    param.fromString(element);
    mValueAddress = param;
  }

  snprintf(path, scmBuffer, "/dev/i2c-%d", i2CAdapter);
  mFd = open(path, O_RDWR);
  if(mFd < 0){
    STATUS() = scmNotInitialised;
    return false;
  }

  if(ioctl(mFd, I2C_FUNCS, &funcs) < 0){
    STATUS() = scmNotInitialised;
    return false;
  }

  if(ioctl(mFd, I2C_SLAVE, mDeviceAddress) < 0){
    STATUS() = scmNotInitialised;
    return false;
  }

  STATUS() = scmInitDeinitOK;
  return true;
}

bool CProcessInterface::deinitialise(){
  close(mFd);

  STATUS() = scmInitDeinitOK;
  return true;
}

bool CProcessInterface::readPin(){
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

bool CProcessInterface::writePin(){
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

bool CProcessInterface::readWord(){
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

bool CProcessInterface::writeWord(){
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
