/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Waldemar Eisenmenger, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "processinterface.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

const char * const CSysFsProcessInterface::scmPinInUse = "Pin already in use by other FB";
const char * const CSysFsProcessInterface::scmInitDeinitOK = "OK";
const char * const CSysFsProcessInterface::scmNotInitialised = "Not initialised";

CSysFsProcessInterface::CSysFsProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), mPinNumber(-1){
}

CSysFsProcessInterface::~CSysFsProcessInterface(){
}

bool CSysFsProcessInterface::initialise(bool m_bInputOrOutput){
  char buffer[scmBuffer];
  ssize_t bytes;
  int fd;
  int res;
  char path[scmBuffer];

  fd = open("/sys/class/gpio/export", O_WRONLY);
  if(fd < 0){
    STATUS() = scmNotInitialised;
    return false;
  }

  CIEC_INT param;
  param.fromString(PARAMS().getValue());
  mPinNumber = param;

  bytes = snprintf(buffer, scmBuffer, "%d", mPinNumber);
  write(fd, buffer, bytes);
  close(fd);

  snprintf(path, scmBuffer, "/sys/class/gpio/gpio%d/direction", mPinNumber);
  fd = open(path, O_WRONLY);
  if(fd < 0){
    STATUS() = scmNotInitialised;
    return false;
  }

  if(m_bInputOrOutput){
    res = write(fd, "in", 2);
  }
  else if(!m_bInputOrOutput){
    res = write(fd, "out", 3);
  }
  if(res < 0){
    STATUS() = scmNotInitialised;
    return false;
  }
  close(fd);

  STATUS() = scmInitDeinitOK;
  return true;
}

bool CSysFsProcessInterface::deinitialise(){
  char buffer[scmBuffer];
  ssize_t bytes;
  int fd;

  fd = open("/sys/class/gpio/unexport", O_WRONLY);
  if(fd < 0){
    STATUS() = scmNotInitialised; //rework status messages
    return false;
  }

  CIEC_INT param;
  param.fromString(PARAMS().getValue());
  mPinNumber = param;

  bytes = snprintf(buffer, scmBuffer, "%d", mPinNumber);
  write(fd, buffer, bytes);
  close(fd);

  STATUS() = scmInitDeinitOK;
  return true;
}

bool CSysFsProcessInterface::readPin(){
  char path[scmBuffer];
  int fd;
  char result[scmBuffer] = { 0 };

  CIEC_INT param;
  param.fromString(PARAMS().getValue());
  mPinNumber = param;

  snprintf(path, scmBuffer, "/sys/class/gpio/gpio%d/value", mPinNumber);
  fd = open(path, O_RDONLY);
  if(fd < 0){
    STATUS() = "Kann net lesen";
    return false;
  }
  if(read(fd, result, 3) < 0){
    STATUS() = "Kann net lesen";
    return false;
  }
  close(fd);

  if('0' == result[0]){
    IN_X() = false;
  }
  else if('1' == result[0]){
    IN_X() = true;
  }
  STATUS() = "Konnte lesen";
  return true;
}

bool CSysFsProcessInterface::writePin(){
  char path[scmBuffer];
  int fd;
  int res;

  CIEC_INT param;
  param.fromString(PARAMS().getValue());
  mPinNumber = param;

  snprintf(path, scmBuffer, "/sys/class/gpio/gpio%d/value", mPinNumber);
  fd = open(path, O_WRONLY);
  if(fd < 0){
    STATUS() = "Kann net schreiben";
    return false;
  }

  if(OUT_X()){
    res = write(fd, "0", 1);
  }
  else{
    res = write(fd, "1", 1);
  }
  if(res < 0){
    STATUS() = "Kann net schreiben";
    return false;
  }

  close(fd);
  STATUS() = "Konnte lesen";
  return true;
}
