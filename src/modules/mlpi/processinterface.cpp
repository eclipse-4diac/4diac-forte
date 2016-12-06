/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of code and error logging added
 *******************************************************************************/
#include "processinterface.h"

#include <mlpiGlobal.h>
#include <mlpiApiLib.h>
#include <mlpiLogicLib.h>
#include <util/wchar16.h>
//#include <util/mlpiLogicHelper.h>

const char * const CMLPIFaceProcessInterface::scmOK = "OK";
const char * const CMLPIFaceProcessInterface::scmAPINotInitialised = "API not initialized";
const char * const CMLPIFaceProcessInterface::scmFBNotInitialised = "FB not initialized";
const char * const CMLPIFaceProcessInterface::scmCallToApiFailed = "Call to API Failed";


/*
 * localhost doesn't connect to MLPI.
 * 192.168.1.110 works perfectly, I had once errors saying that access not granted or something
 * 192.168.1.110 -user=indraworks -password=indraworks works perfectly
 * LOCALHOST -user=indraworks -password=indraworks connects to MLPI but when writing the outputs from flipflop, the PLC goes crazy, all blinking in red
 * LOCALHOST same as before
 * Using MLPI_LOCALHOST direclty in the APIConnect call, same as before
 */

const char* const cgConnectionConfig = "192.168.1.110 -user=indraworks -password=indraworks";
MLPIHANDLE CMLPIFaceProcessInterface::smConnection  = MLPI_INVALIDHANDLE;

CMLPIFaceProcessInterface::CMLPIFaceProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), mVariableName(0){
}

CMLPIFaceProcessInterface::~CMLPIFaceProcessInterface(){
  deinitialise();
}

void CMLPIFaceProcessInterface::disconnectFromMLPI(){
  if(MLPI_INVALIDHANDLE != smConnection){
    MLPIRESULT result = mlpiApiDisconnect(&smConnection);
    if(!MLPI_FAILED(result)){
      DEVLOG_INFO("Successfully disconnected!\n");
    }
    else{
      DEVLOG_INFO("Error on disconnect from MLPI with 0x%08x\n", (unsigned ) result);
    }
  }
}

bool CMLPIFaceProcessInterface::connectToMLPI(){
  smConnection = MLPI_INVALIDHANDLE;
  bool retVal = false;
  WCHAR16 *convertedConfig = new WCHAR16[strlen(cgConnectionConfig) + 1];
  if (0 != mbstowcs16(convertedConfig, cgConnectionConfig, strlen(cgConnectionConfig) + 1)){ //+1 for the copying the null terminator
    DEVLOG_INFO("Trying to connect to the Api\n");
    //MLPIRESULT result = mlpiApiConnect("localhost -user=indraworks -password=indraworks", &smConnection);
    MLPIRESULT result = mlpiApiConnect(convertedConfig, &smConnection);
    if(!MLPI_FAILED(result)){
      DEVLOG_INFO("Connection to the API succeed\n");
      retVal = true;
    }
    else{
      DEVLOG_ERROR("Failed to connect to the API: 0x%08X\n", (unsigned ) result);
    }
  }else{
    DEVLOG_ERROR("Fail transforming the connection name\n");
  }
  delete[] convertedConfig;
  return retVal;
}

bool CMLPIFaceProcessInterface::initialise(bool ){
  bool retVal = false;

  mVariableName = new WCHAR16[PARAMS().length() + 1];
  if(0 != mbstowcs16(mVariableName, PARAMS().getValue(), PARAMS().length() + 1)){ //+1 for the copying the null terminator
    STATUS() = scmOK;
    retVal = true;
  }
  else{
    DEVLOG_ERROR("Fail transforming the PARAM name\n");
    STATUS() = scmFBNotInitialised;
  }

  return retVal;
}

bool CMLPIFaceProcessInterface::deinitialise(){
  delete[] mVariableName;
  return true;
}

bool CMLPIFaceProcessInterface::readPin(){
  bool retVal = false;

  if(MLPI_INVALIDHANDLE != smConnection){
    BOOL8 data = FALSE;
    MLPIRESULT result = mlpiLogicReadVariableBySymbolBool8(smConnection, mVariableName, &data);
    if(!MLPI_FAILED(result)){
      IN_X() = (data != FALSE) ? true : false;
      STATUS() = scmOK;
      retVal = true;
    }
    else{
      DEVLOG_ERROR("Failed to read: 0x%08X\n", (unsigned ) result);
      STATUS() = scmCallToApiFailed;
    }
  }
  else{
    DEVLOG_ERROR("Cannot read pin. The API is not initialized.\n");
    STATUS() = scmAPINotInitialised;
  }

  return retVal;
}

bool CMLPIFaceProcessInterface::writePin(){
  bool retVal = false;

  if(MLPI_INVALIDHANDLE != smConnection){
    BOOL8 data = OUT_X();
    MLPIRESULT result = mlpiLogicWriteVariableBySymbolBool8(smConnection, mVariableName, data);
    if(!MLPI_FAILED(result)){
      STATUS() = scmOK;
      retVal = true;
    }
    else{
      DEVLOG_ERROR("Failed to write: 0x%08X\n", (unsigned ) result);
      STATUS() = scmCallToApiFailed;
    }
  }
  else{
    DEVLOG_ERROR("Cannot write pin. The API is not initialized.\n");
    STATUS() = scmAPINotInitialised;
  }

  return retVal;
}

