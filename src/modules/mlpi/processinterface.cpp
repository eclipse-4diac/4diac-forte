/*******************************************************************************
 * Copyright (c) 2016 - 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of code and error logging added
 *******************************************************************************/
#include "processinterface.h"
#include <extevhandlerhelper.h>
#include <unistd.h>
#include <mlpiGlobal.h>
#include <mlpiApiLib.h>
#include <mlpiLogicLib.h>
#include <util/wchar16.h>

const char * const CMLPIFaceProcessInterface::scmOK = "OK";
const char * const CMLPIFaceProcessInterface::scmAPINotInitialised = "API not initialized";
const char * const CMLPIFaceProcessInterface::scmFBNotInitialised = "FB not initialized";
const char * const CMLPIFaceProcessInterface::scmCallToApiFailed = "Call to API Failed";

MLPIHANDLE CMLPIFaceProcessInterface::smConnection  = MLPI_INVALIDHANDLE;

CMLPIFaceProcessInterface::CMLPIFaceProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), mVariableName(0){
}

CMLPIFaceProcessInterface::~CMLPIFaceProcessInterface(){
  deinitialise();
}

void CMLPIFaceProcessInterface::disconnectFromMLPI(){ //TODO: this is never called. Where should it be?
  if(MLPI_INVALIDHANDLE != smConnection){
    MLPIRESULT result = mlpiApiDisconnect(&smConnection);
    if(!MLPI_FAILED(result)){
      DEVLOG_INFO("Successfully disconnected from MLPI!\n");
    }
    else{
      DEVLOG_INFO("Error on disconnect from MLPI with 0x%08x\n", (unsigned ) result);
    }
  }
}

bool CMLPIFaceProcessInterface::connectToMLPI(){
  smConnection = MLPI_INVALIDHANDLE;
  bool retVal = false;
  DEVLOG_INFO("Trying to connect to the Api\n");
  MLPIRESULT result = mlpiApiConnect(MLPI_LOCALHOST, &smConnection);
  if(!MLPI_FAILED(result)){
    DEVLOG_INFO("Connection to the API succeed\n");
    retVal = true;
  }
  else{
    DEVLOG_ERROR("Failed to connect to the API: 0x%08X\n", (unsigned ) result);
  }
  return retVal;
}

bool CMLPIFaceProcessInterface::initialise(bool paInput){
  bool retVal = false;
  /*
   * Starting forte at boot in the PLC has the effect of failing when connecting to the MLPI
   * probably because the stack is not ready yet. So this waits until mMaxNumberOfTriesToReconnect seconds or succeed,
   * whichever happens first, in order to continue.
   */
  if(MLPI_INVALIDHANDLE == smConnection){
    unsigned int connectRetries = 0;
    CMLPIFaceProcessInterface::connectToMLPI();
    while(MLPI_INVALIDHANDLE == smConnection && mMaxNumberOfTriesToReconnect > connectRetries){
      CThread::sleepThread(1000);
      CMLPIFaceProcessInterface::connectToMLPI();
      connectRetries++;
    }
  }
  if(MLPI_INVALIDHANDLE != smConnection){
    mVariableName = new WCHAR16[PARAMS().length() + 1];
    if(0 != mbstowcs16(mVariableName, PARAMS().getValue(), PARAMS().length() + 1)){ //+1 for the copying the null terminator
      STATUS() = scmOK;
      retVal = true;
      if(paInput){
        getExtEvHandler<CMLPIFaceProcessInterface::CIOHandler>(*this).registerIXFB(this);
        if(!getExtEvHandler<CMLPIFaceProcessInterface::CIOHandler>(*this).isAlive()){
          getExtEvHandler<CMLPIFaceProcessInterface::CIOHandler>(*this).start();
        }
      }
    }
    else{
      DEVLOG_ERROR("Fail transforming the PARAM name\n");
      STATUS() = scmFBNotInitialised;
    }
  }
  else{
    DEVLOG_ERROR("Couldn't initialize API\n");
    STATUS() = scmAPINotInitialised;
  }
  return retVal;
}

bool CMLPIFaceProcessInterface::deinitialise(){
  getExtEvHandler<CMLPIFaceProcessInterface::CIOHandler>(*this).unregisterIXFB(this);
  delete[] mVariableName;
  STATUS() = scmAPINotInitialised;
  return true;
}

bool CMLPIFaceProcessInterface::readPin(){
   return true;
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

bool CMLPIFaceProcessInterface::checkInputData(){
  bool retVal = false;

  if(MLPI_INVALIDHANDLE != smConnection){
    BOOL8 data = FALSE;
    MLPIRESULT result = mlpiLogicReadVariableBySymbolBool8(smConnection, mVariableName, &data);
    if(!MLPI_FAILED(result)){
      bool newData = (data != FALSE) ? true : false;
      if(newData != IN_X()){
        IN_X() = newData;
        retVal = true;
      }
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


DEFINE_HANDLER(CMLPIFaceProcessInterface::CIOHandler)

CMLPIFaceProcessInterface::CIOHandler::CIOHandler(CDeviceExecution& pa_poDeviceExecution) : CExternalEventHandler(pa_poDeviceExecution){
}

CMLPIFaceProcessInterface::CIOHandler::~CIOHandler(){
}

void CMLPIFaceProcessInterface::CIOHandler::registerIXFB(CMLPIFaceProcessInterface *pa_poFB){
  mReadFBListSync.lock();
  mReadFBList.pushBack(pa_poFB);
  mReadFBListSync.unlock();
}

void CMLPIFaceProcessInterface::CIOHandler::unregisterIXFB(CMLPIFaceProcessInterface *pa_poFB){
  mReadFBListSync.lock();
  TReadFBContainer::Iterator itRunner(mReadFBList.begin());
  TReadFBContainer::Iterator itRefNode(mReadFBList.end());
  TReadFBContainer::Iterator itEnd(mReadFBList.end());
  while(itRunner != itEnd){
    if(*itRunner == pa_poFB){
      if(itRefNode == itEnd){
        mReadFBList.popFront();
      }
      else{
        mReadFBList.eraseAfter(itRefNode);
      }
      break;
    }
    itRefNode = itRunner;
   ++itRunner;
  }
  mReadFBListSync.unlock();
}

void CMLPIFaceProcessInterface::CIOHandler::run(){
  while(isAlive()){
    CThread::sleepThread(1);
    updateReadData();
  }
}

void CMLPIFaceProcessInterface::CIOHandler::updateReadData(){
  TReadFBContainer::Iterator itEnd(mReadFBList.end());
  for(TReadFBContainer::Iterator itRunner = mReadFBList.begin(); itRunner != itEnd; ++itRunner){
    if((*itRunner)->checkInputData()){
      startNewEventChain(*itRunner);
    }
  }
}

void CMLPIFaceProcessInterface::CIOHandler::enableHandler(void){
}

void CMLPIFaceProcessInterface::CIOHandler::disableHandler(void){
}

void CMLPIFaceProcessInterface::CIOHandler::setPriority(int){
}

int CMLPIFaceProcessInterface::CIOHandler::getPriority(void) const{
 return 0;
}
