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
#include <mlpiLogicLib.h>
#include <util/wchar16.h>

MLPIHANDLE CMLPIFaceProcessInterface::smConnection  = 0;
bool CMLPIFaceProcessInterface::smInitialized = false;

CMLPIFaceProcessInterface::CMLPIFaceProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), mPin(0){
}

CMLPIFaceProcessInterface::~CMLPIFaceProcessInterface(){
  deinitialise();
}

bool CMLPIFaceProcessInterface::initialise(bool ){
  bool retVal = true;

  DEVLOG_INFO("Before Apiconnet\n");

  if(!smInitialized){
    MLPIHANDLE connection = 0;
    MLPIRESULT result = mlpiApiConnect(L"192.168.0.110:5300 -user=indraworks -password=indraworks", &connection);
    if(MLPI_FAILED (result)){
      printf("Failed to connect: 0x%08X\n", (unsigned) result);
      retVal = false;
    } else {
      smConnection = connection;
      smInitialized = true;
    }
  }
  printf("Before variable name\n");
  mVariableName = new WCHAR16[PARAMS().length() + 1];
  mbstowcs16(mVariableName, PARAMS().getValue(), PARAMS().length() + 1);
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

  //We don't need to do anything in readPin output update is handled as part of checkInputData
  return (result >= 0);
}

bool CMLPIFaceProcessInterface::writePin(){
  BOOL8 data = OUT_X();
  MLPIRESULT result = mlpiLogicWriteVariableBySymbolBool8(smConnection, mVariableName, data);
  return (result >= 0);
}

