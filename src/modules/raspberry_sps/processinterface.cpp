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

CProcessInterface::CProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
    const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData,
    TForteByte *paFBVarsData) :
    CI2CProcessInterface(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData){
}

CProcessInterface::~CProcessInterface(){
}

bool CProcessInterface::readPin(){
  bool retVal = CI2CProcessInterface::readPin();
  if(retVal){
    //Raspberry PI SPS has inverted inputs so we need to re-invert here
    IN_X() = !(IN_X().operator bool());
  }
  return retVal;
}

bool CProcessInterface::writePin(){

  //Raspberry PI SPS has inverted inputs so we need to re-invert here
  OUT_X() = !(OUT_X().operator bool());
  bool retVal = CI2CProcessInterface::writePin();
  //Put the value back to the original one
  OUT_X() = !(OUT_X().operator bool());
  return retVal;
}

bool CProcessInterface::readWord(){
  int res;
  uint8_t buffer[3];

  if(ioctl(mFd, I2C_SLAVE, mDeviceAddress) < 0){
    STATUS() = "Kann net lesen";
    return false;
  }

  res = i2c_smbus_read_i2c_block_data(mFd, (uint8_t) mValueAddress, 3, buffer);
  if(res < 0)
      {
    STATUS() = "Kann net lesen";
    return false;
  }
  IN_W() = static_cast<TForteWord>(static_cast<TForteWord>(buffer[2] << 8)
      | static_cast<TForteWord>(buffer[1]));
  STATUS() = "Konnte lesen";
  return true;
}
