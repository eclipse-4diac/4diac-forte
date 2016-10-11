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

bool CSysFsProcessInterface::initialise(bool paIsInput){
  bool retVal = false;
  STATUS() = scmNotInitialised;
  std::string fileName = "/sys/class/gpio/export";

  mFile.clear();
  mFile.open(fileName.c_str(), std::fstream::out);
  if(mFile.is_open()){
    mFile << PARAMS().getValue();
    if(!mFile.fail()){
      mFile.close();
      fileName = "/sys/class/gpio/gpio" + std::string(PARAMS().getValue()) + "/direction";

      mFile.open(fileName.c_str(), std::fstream::out);
      if(mFile.is_open()){
        mFile.clear();
        if(paIsInput){
          mFile << "in";
        }
        else{
          mFile << "out";
        }
        if(!mFile.fail()){
          mFile.close();
          fileName = "/sys/class/gpio/gpio" + std::string(PARAMS().getValue()) + "/value";
          if(paIsInput){
            mFile.open(fileName.c_str(), std::fstream::in);
          }
          else{
            mFile.open(fileName.c_str(), std::fstream::out);
          }
          if(mFile.is_open()){
            STATUS() = scmOK;
            retVal = true;
          }
          else{
            DEVLOG_ERROR("Opening file %s failed.\n", fileName.c_str());
          }
        }
        else{
          DEVLOG_ERROR("Error writing to file %s.\n", fileName.c_str());
        }
      }
      else{
        DEVLOG_ERROR("Opening file %s failed.\n", fileName.c_str());
      }
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

bool CSysFsProcessInterface::deinitialise(){
  bool retVal = false;
  STATUS() = scmError;
  std::string fileName = "/sys/class/gpio/unexport";

  if(mFile.is_open()){
    mFile.close();
    mFile.clear();
    mFile.open(fileName.c_str(), std::fstream::out);
    if(mFile.is_open()){
      mFile << PARAMS().getValue();
      if(!mFile.fail()){
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
