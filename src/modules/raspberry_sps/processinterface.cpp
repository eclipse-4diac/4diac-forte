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
 *    Alois Zoitl - made better use of generic i2c process interface implementation
 *                  changed from i2c-dev to simple read writes
 *******************************************************************************/

#include "processinterface.h"
#include <unistd.h>

CProcessInterface::CProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
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
  bool retVal = false;

  //set the register to read the analog value from
  if(1 == write(mFd, &mValueAddress, 1)){
    TForteByte buffer[3];
    if(3 == read(mFd, buffer, 3)){
      IN_W() = static_cast<TForteWord>(((static_cast<TForteWord>(buffer[2]) << 8) & 0xFF00) | 
                 buffer[1]);
      STATUS() = scmOK;
      retVal = true;
    }
    else{
      STATUS() = scmCouldNotRead;
    }
  }
  else{
    STATUS() = scmCouldNotRead;
  }
  return retVal;
}
