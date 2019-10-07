/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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
    std::string binData;

    mFile.clear();
    mFile.seekg(0, std::ios::beg);
    std::getline(mFile, binData);
    if(mFile.fail()){
      STATUS() = scmCouldNotRead;
    }
    else{
      if(-1 != IN_W().fromString(binData.c_str())){
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
