/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "processinterface.h"
#include <stdio.h>

#include <mlpiGlobal.h>
#include <mlpiApiLib.h>
#include <mlpiLogicLib.h>
#include <util/wchar16.h>
//#include <util/mlpiLogicHelper.h>


MLPIHANDLE CMLPIFaceProcessInterface::smConnection  = 0;
bool CMLPIFaceProcessInterface::smInitialized = false;

CMLPIFaceProcessInterface::CMLPIFaceProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), mPin(0){
}

CMLPIFaceProcessInterface::~CMLPIFaceProcessInterface(){
  deinitialise();
}

const char* const cgConnectionConfig = "localhost";

void CMLPIFaceProcessInterface::connectToMLPI(){
  smConnection = MLPI_INVALIDHANDLE;
  DEVLOG_INFO("Before Apiconnet\n");
  WCHAR16 *convertedConfig = new WCHAR16[strlen(cgConnectionConfig) + 1];
  mbstowcs16(convertedConfig, cgConnectionConfig, strlen(cgConnectionConfig) + 1); //+1 for the copying the null terminator
  //MLPIRESULT result = mlpiApiConnect("localhost -user=indraworks -password=indraworks", &smConnection);
  MLPIRESULT result = mlpiApiConnect(convertedConfig, &smConnection);
  if(MLPI_FAILED (result)){
    printf("Failed to connect: 0x%08X\n", (unsigned) result);
  } else {
    smInitialized = true;
  }
}

bool CMLPIFaceProcessInterface::initialise(bool ){
  bool retVal = true;

  printf("Before variable name\n");
  mVariableName = new WCHAR16[PARAMS().length() + 1];
  mbstowcs16(mVariableName, PARAMS().getValue(), PARAMS().length() + 1); //+1 for the copying the null terminator
  printf("After variable name\n");


  return retVal;
}

bool CMLPIFaceProcessInterface::deinitialise(){
  delete[] mVariableName;
  return true;
}

bool CMLPIFaceProcessInterface::readPin(){
  BOOL8 data = FALSE;
  MLPIRESULT result = mlpiLogicReadVariableBySymbolBool8(smConnection, mVariableName, &data);

  IN_X() = (data != FALSE) ? true : false;

  if(MLPI_FAILED (result)){
    printf("Failed to read: 0x%08X\n", (unsigned) result);
  }
  return !MLPI_FAILED (result);
}

bool CMLPIFaceProcessInterface::writePin(){
  BOOL8 data = OUT_X();
  MLPIRESULT result = mlpiLogicWriteVariableBySymbolBool8(smConnection, mVariableName, data);
  if(MLPI_FAILED (result)){
    printf("Failed to write: 0x%08X\n", (unsigned) result);
  }
  return !MLPI_FAILED (result);
}

