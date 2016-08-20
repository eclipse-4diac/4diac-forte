/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "processinterface.h"

COdroidProcessInterface::COdroidProcessInterface(CResource *paSrcRes,
    const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId,
    TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CSysFsProcessInterface(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData){

}

COdroidProcessInterface::~COdroidProcessInterface(){

}

bool COdroidProcessInterface::initialise(bool paIsInput){
  bool retVal = true;
  if((paIsInput) && (getDO(2)->getDataTypeID() == CIEC_ANY::e_WORD)){
    std::string fileName("/sys/class/saradc/ch");
    fileName += PARAMS().getValue();
    mFile.open(fileName.c_str(), std::fstream::in);

    if(mFile.is_open()){
      STATUS() = scmOK;
    }
    else{
      STATUS() = scmNotInitialised;
      DEVLOG_ERROR("Opening file %s failed.\n", fileName.c_str());
      retVal = false;
    }
  }
  else{
    retVal = CSysFsProcessInterface::initialise(paIsInput);
  }
  return retVal;
}

bool COdroidProcessInterface::deinitialise(){
  bool retVal = true;
  STATUS() = scmOK;
  if(getDO(2)->getDataTypeID() == CIEC_ANY::e_WORD){
    mFile.close();
  }
  else{
    retVal = CSysFsProcessInterface::deinitialise();
  }
  return retVal;
}

bool COdroidProcessInterface::readWord(){
  bool retVal = false;
  if(mFile.is_open()){
    char binData[6];
    memset(binData, 0, 6);
    mFile.clear();
    mFile.seekg(0, std::ios::beg);
    mFile.read(binData, 5);
    if(mFile.fail()){
      STATUS() = scmCouldNotRead;
    }
    else{
      binData[5] = '\0';
      if(-1 != IN_W().fromString(binData)){
        STATUS() = scmOK;
        retVal = true;
      } else {
        STATUS() = scmError;
      }
    }
  }
  else{
    STATUS() = scmNotInitialised;
  }

  return retVal;
}
