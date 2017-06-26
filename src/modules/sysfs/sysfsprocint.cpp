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
#include <string>
#include <unistd.h>
const char * const CSysFsProcessInterface::scmOK = "OK";
const char * const CSysFsProcessInterface::scmPinInUse = "Pin already in use by other FB";
const char * const CSysFsProcessInterface::scmNotInitialised = "FB not initialized";
const char * const CSysFsProcessInterface::scmError = "Error";
const char * const CSysFsProcessInterface::scmCouldNotRead = "Could not read";
const char * const CSysFsProcessInterface::scmCouldNotWrite = "Could not write";

CSysFsProcessInterface::CSysFsProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData){
  mFile.rdbuf()->pubsetbuf(0, 0); //disable buffer to avoid latency
}

CSysFsProcessInterface::~CSysFsProcessInterface(){
    deinitialise(); //Will unexport everything, so next time FORTE starts it won't fail to initialize.
}

bool CSysFsProcessInterface::setDirection(bool paIsInput){
    bool retVal = false;
    std::string fileName = "/sys/class/gpio/gpio" + std::string(PARAMS().getValue()) + "/direction";
    std::ofstream mDirectionFile;
    mDirectionFile.open(fileName.c_str());
    if(mDirectionFile.is_open()){
      if(paIsInput){
           DEVLOG_INFO("3in\n");
        mDirectionFile << "in";   
      }else{
           DEVLOG_INFO("3out\n");
        mDirectionFile << "out";
      }
      retVal = true;
    }else{
      retVal = false;
    }
    
    return retVal;
}

bool CSysFsProcessInterface::exportGPIO(){
    bool retVal = false; 
    std::string fileName = "/sys/class/gpio/export";
    std::ofstream mExportFile;
    mExportFile.open(fileName.c_str());
  if(mExportFile.is_open()){
    DEVLOG_INFO("1\n");
    mExportFile << PARAMS().getValue();
    retVal = true;
  }else{
    retVal = false;
    DEVLOG_ERROR("Opening file %s failed.\n", fileName.c_str());
  }  
  
  return retVal;   
}

bool CSysFsProcessInterface::valueGPIO(bool paIsInput){
    bool retVal = false;
    std::string fileName = "/sys/class/gpio/gpio" + std::string(PARAMS().getValue()) + "/value";
    if(paIsInput){
      mFile.open(fileName.c_str(), std::fstream::in); 
      retVal = true;
    }else{
      mFile.open(fileName.c_str(), std::fstream::out);
      retVal = true;
    }
    return retVal;
}

bool CSysFsProcessInterface::initialise(bool paIsInput){
  bool retVal = false;
  STATUS() = scmNotInitialised;
  if(CSysFsProcessInterface::exportGPIO()){
    sleep(1);
    if(CSysFsProcessInterface::setDirection(paIsInput)){
      sleep(1);
      if(CSysFsProcessInterface::valueGPIO(paIsInput)){
          retVal = true;
          STATUS() = scmOK;
      }
      else{
          retVal = false;
      }
    }else{
        retVal = false;
    }
  }else{
      retVal = false;
  }
 
  return retVal;
}

bool CSysFsProcessInterface::deinitialise(){
  bool retVal = false;
  STATUS() = scmError;
  std::string fileName = "/sys/class/gpio/unexport";
  std::ofstream mUnExport;
  
  mUnExport.open(fileName.c_str(), std::fstream::out);
  if(mUnExport.is_open()){
      mUnExport << PARAMS().getValue();
      if(!mUnExport.fail()){
        retVal = true;
        STATUS() = scmOK;
      }
      else{
        DEVLOG_ERROR("Error writing PARAMS() to file %s.\n", fileName.c_str());
      }
  }
  else{
      DEVLOG_ERROR("Opening file %s failed.\n", fileName.c_str());
  }
  return retVal;
}

bool CSysFsProcessInterface::readPin(){
  bool retVal = false;
  if(mFile.is_open()){
    char binData = 0;
    mFile.clear();
    mFile.seekg(0, std::ios::beg);
    mFile.read(&binData, 1);
    if(mFile.fail()){
      STATUS() = scmCouldNotRead;
    }
    else{
      IN_X() = ('0' != binData) ? true : false;
      STATUS() = scmOK;
      retVal = true;
    }
  }else{
    STATUS() = scmNotInitialised;
  }

  return retVal;
}

bool CSysFsProcessInterface::writePin(){
  bool retVal = false;
  if(mFile.is_open()){
    mFile.clear();
    mFile.seekp(0, std::ios::beg);
    unsigned int val = (false != OUT_X()) ? 1 : 0; //if true set the led to full glowing
    mFile << val;
    if(mFile.fail()){
      STATUS() = scmCouldNotWrite;
    }
    else{
      STATUS() = scmOK;
      retVal = true;
    }
  }else{
    STATUS() = scmNotInitialised;
  }

  return retVal;
}