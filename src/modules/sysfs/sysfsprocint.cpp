/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Waldemar Eisenmenger, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "sysfsprocint.h"
#include "../../arch/devlog.h"
#include <extevhandlerhelper.h>
#include <criticalregion.h>
#include <string>
const char * const CSysFsProcessInterface::scmOK = "OK";
const char * const CSysFsProcessInterface::scmPinInUse = "Pin already in use by other FB";
const char * const CSysFsProcessInterface::scmNotInitialised = "FB not initialized";
const char * const CSysFsProcessInterface::scmError = "Error";
const char * const CSysFsProcessInterface::scmCouldNotRead = "Could not read";
const char * const CSysFsProcessInterface::scmCouldNotWrite = "Could not write";

CSysFsProcessInterface::CSysFsProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
    const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData) {
  mFile.rdbuf()->pubsetbuf(0, 0); //disable buffer to avoid latency
  STATUS() = scmNotInitialised;
}

CSysFsProcessInterface::~CSysFsProcessInterface() {
  unexportIO(); //Will unexport everything, so next time FORTE starts it won't fail to initialize.
}

bool CSysFsProcessInterface::setDirection(bool paIsInput) {
  bool retVal = false;
  std::string fileName = "/sys/class/gpio/gpio" + std::string(PARAMS().getValue()) + "/direction";
  std::ofstream mDirectionFile;
  mDirectionFile.open(fileName.c_str());
  if(mDirectionFile.is_open()) {
    if(paIsInput) {
      mDirectionFile << "in";
    } else {
      mDirectionFile << "out";
    }
    if(!mDirectionFile.fail()){
      retVal = true;
    } else{
      DEVLOG_ERROR("[CSysFsProcessInterface::setDirection] Error writing to file %s.\n", fileName.c_str());
    }
  } else {
    DEVLOG_ERROR("[CSysFsProcessInterface::setDirection] Opening file %s failed.\n", fileName.c_str());
  }

  return retVal;
}

bool CSysFsProcessInterface::exportGPIO() {
  bool retVal = false;
  std::string fileName = "/sys/class/gpio/export";
  std::ofstream mExportFile;
  mExportFile.open(fileName.c_str());
  if(mExportFile.is_open()) {
    mExportFile << PARAMS().getValue();
    if(!mExportFile.fail()) {
      retVal = true;
    } else {
      DEVLOG_ERROR("[CSysFsProcessInterface::exportGPIO] Error writing to file %s.\n", fileName.c_str());
    }
    mExportFile.close();
    retVal = true;
  } else {
    DEVLOG_ERROR("[CSysFsProcessInterface::exportGPIO] Opening file %s failed.\n", fileName.c_str());
  }

  return retVal;
}

bool CSysFsProcessInterface::valueGPIO(bool paIsInput) {
  bool retVal = false;
  std::string fileName = "/sys/class/gpio/gpio" + std::string(PARAMS().getValue()) + "/value";
  if(paIsInput) {
    mFile.open(fileName.c_str(), std::fstream::in);
  } else {
    mFile.open(fileName.c_str(), std::fstream::out);
  }

  if(mFile.is_open()){
    retVal = true;
  }else{
    DEVLOG_ERROR("[CSysFsProcessInterface::valueGPIO] Opening file %s failed.\n", fileName.c_str());
  }
  return retVal;
}

bool CSysFsProcessInterface::initialise(bool paIsInput) {
  bool retVal = false;
  if(CSysFsProcessInterface::exportGPIO()) {
    CThread::sleepThread(250);
    if(CSysFsProcessInterface::setDirection(paIsInput)) {
      CThread::sleepThread(250);
      if(CSysFsProcessInterface::valueGPIO(paIsInput)) {
        if(paIsInput) {
          getExtEvHandler<CSysFsProcessInterface::CIOHandler>(*this).registerIXFB(this);
          if(!getExtEvHandler<CSysFsProcessInterface::CIOHandler>(*this).isAlive()) {
            getExtEvHandler<CSysFsProcessInterface::CIOHandler>(*this).start();
          }
        }
        DEVLOG_DEBUG("[CSysFsProcessInterface::initialise] Pin with PARAM() %s was properly initialized.\n", PARAMS().getValue());
        STATUS() = scmOK;
        retVal = true;
      }
    }
  }

  return retVal;
}

bool CSysFsProcessInterface::unexportIO() {
  bool retVal = false;
  std::string fileName = "/sys/class/gpio/unexport";
  std::ofstream mUnExport;

  mFile.close();
  mUnExport.open(fileName.c_str(), std::fstream::out);
  if(mUnExport.is_open()) {
    mUnExport << PARAMS().getValue();
    if(!mUnExport.fail()) {
      retVal = true;
      STATUS() = scmOK;
    } else {
      STATUS() = scmError;
      DEVLOG_ERROR("[CSysFsProcessInterface::deinitialise] Error writing PARAMS() to file %s.\n", fileName.c_str());
    }
  } else {
    STATUS() = scmError;
    DEVLOG_ERROR("[CSysFsProcessInterface::deinitialise] Opening file %s failed.\n", fileName.c_str());
  }
  return retVal;
}

bool CSysFsProcessInterface::deinitialise() {
  getExtEvHandler<CSysFsProcessInterface::CIOHandler>(*this).unregisterIXFB(this);
  return unexportIO();
}

bool CSysFsProcessInterface::readPin() {
  return true;
}

bool CSysFsProcessInterface::checkInputData() {
  bool retVal = false;
  if(mFile.is_open()) {
    char binData = 0;
    mFile.clear();
    mFile.seekg(0, std::ios::beg);
    mFile.read(&binData, 1);
    if(mFile.fail()) {
      STATUS() = scmCouldNotRead;
    } else {
      bool newData = ('0' != binData) ? true : false;
      if(newData != IN_X()) {
        IN_X() = newData;
        retVal = true;
      }
    }
  } else {
    STATUS() = scmNotInitialised;
  }

  return retVal;
}

bool CSysFsProcessInterface::writePin() {
  bool retVal = false;
  if(mFile.is_open()) {
    mFile.clear();
    mFile.seekp(0, std::ios::beg);
    unsigned int val = (false != OUT_X()) ? 1 : 0; //if true set the led to full glowing
    mFile << val;
    if(!mFile.fail()) {
      STATUS() = scmOK;
      retVal = true;
    } else {
      DEVLOG_ERROR("[CSysFsProcessInterface::writePin] Could not write %u to output file\n", val);
      STATUS() = scmCouldNotWrite;
    }
  } else {
    DEVLOG_ERROR("[CSysFsProcessInterface::writePin] Cannot write to output since the FB was not properly initialized\n");
    STATUS() = scmNotInitialised;
  }

  return retVal;
}

DEFINE_HANDLER(CSysFsProcessInterface::CIOHandler);

CSysFsProcessInterface::CIOHandler::CIOHandler(CDeviceExecution& pa_poDeviceExecution) :
    CExternalEventHandler(pa_poDeviceExecution) {
}

CSysFsProcessInterface::CIOHandler::~CIOHandler() {
  CCriticalRegion readList(mReadFBListSync);
  mReadFBList.clearAll();
  end();
}

void CSysFsProcessInterface::CIOHandler::registerIXFB(CSysFsProcessInterface *pa_poFB) {
  CCriticalRegion readList(mReadFBListSync);
  mReadFBList.pushBack(pa_poFB);
}

void CSysFsProcessInterface::CIOHandler::unregisterIXFB(CSysFsProcessInterface *pa_poFB) {
  CCriticalRegion readList(mReadFBListSync);
  TReadFBContainer::Iterator itRunner(mReadFBList.begin());
  TReadFBContainer::Iterator itRefNode(mReadFBList.end());
  TReadFBContainer::Iterator itEnd(mReadFBList.end());
  while(itRunner != itEnd) {
    if(*itRunner == pa_poFB) {
      if(itRefNode == itEnd) {
        mReadFBList.popFront();
      } else {
        mReadFBList.eraseAfter(itRefNode);
      }
      break;
    }
    itRefNode = itRunner;
    ++itRunner;
  }
}

void CSysFsProcessInterface::CIOHandler::run() {
  while(isAlive()) {
    CThread::sleepThread(10);
    updateReadData();
  }
}

void CSysFsProcessInterface::CIOHandler::updateReadData() {
  CCriticalRegion readList(mReadFBListSync);
  TReadFBContainer::Iterator itEnd(mReadFBList.end());
  for(TReadFBContainer::Iterator itRunner = mReadFBList.begin(); itRunner != itEnd; ++itRunner) {
    if((*itRunner)->checkInputData()) {
      startNewEventChain(*itRunner);
    }
  }
}

void CSysFsProcessInterface::CIOHandler::enableHandler(void) {
  //do nothing
}

void CSysFsProcessInterface::CIOHandler::disableHandler(void) {
  end();
}

void CSysFsProcessInterface::CIOHandler::setPriority(int) {
  //do nothing
}

int CSysFsProcessInterface::CIOHandler::getPriority(void) const {
  return 0;
}

