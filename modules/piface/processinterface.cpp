/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "core/fmi/processinterface.h"
#include "forte/util/extevhandlerhelper.h"

CPiFaceProcessInterface::CPiFaceProcessInterface(forte::CFBContainer &paContainer,
                                                 const SFBInterfaceSpec &paInterfaceSpec,
                                                 const forte::StringId paInstanceNameId) :
    CProcessInterfaceBase(paContainer, paInterfaceSpec, paInstanceNameId),
    mPin(0) {
}

CPiFaceProcessInterface::~CPiFaceProcessInterface() {
  deinitialise();
}

bool CPiFaceProcessInterface::initialise(bool paIsInput, CEventChainExecutionThread *const paECET) {
  bool retVal = false;
  if (paIsInput) {
    getExtEvHandler<CPiFaceIOHandler>(*this).registerIXFB(this);
  }
  QO() = QI();
  if (!getExtEvHandler<CPiFaceIOHandler>(*this).isAlive()) {
    getExtEvHandler<CPiFaceIOHandler>(*this).start();
  }
  CIEC_INT pinNum;
  if ((-1 != pinNum.fromString(PARAMS().getValue())) && (8 > pinNum)) {
    mPin = pinNum;
    retVal = true;
  }
  return retVal;
}

bool CPiFaceProcessInterface::deinitialise() {
  getExtEvHandler<CPiFaceIOHandler>(*this).unregisterIXFB(this);
  return true;
}

bool CPiFaceProcessInterface::getValue(long paValue) {
  return 0 != (paValue & (1 << mPin));
}

bool CPiFaceProcessInterface::readPin() {
  // We don't need to do anything in readPin output update is handled as part of checkInputData
  return true;
}

bool CPiFaceProcessInterface::writePin() {
  getExtEvHandler<CPiFaceIOHandler>(*this).updateWriteData(OUT_X(), mPin);
  return true;
}

bool CPiFaceProcessInterface::checkInputData(long paValue) {
  bool retval = false;
  bool newValue = getValue(paValue);
  if (newValue != IN_X()) {
    IN_X() = newValue;
    retval = true;
  }
  return retval;
}

/********************************************************************************************
 ***  CPiFaceProcessInterface::CPiFaceIOHandler
 ********************************************************************************************/

DEFINE_HANDLER(CPiFaceProcessInterface::CPiFaceIOHandler)

CPiFaceProcessInterface::CPiFaceIOHandler::CPiFaceIOHandler(CDeviceExecution &paDeviceExecution) :
    CExternalEventHandler(paDeviceExecution),
    mOutBuffer(0) {
}

CPiFaceProcessInterface::CPiFaceIOHandler::~CPiFaceIOHandler() {
}

void CPiFaceProcessInterface::CPiFaceIOHandler::run() {
  // this gives as the default values of Mode0, Speed 1000000, and Bitsperword 8 which wed need also for piFace
  CONMELEON::CSpiDevice spiDev("/dev/spidev0.0"); // TODO correctly setup SPI interface
  setupPiFaceIOChip(spiDev);

  if (spiDev.isOpen()) {
    TForteUInt8 inBuffer;

    while (isAlive()) {
      CThread::sleepThread(10);

      writePiFaceRegister(spiDev, eGPIOPortA, mOutBuffer); // update outputs
      inBuffer = readInputs(spiDev);
      spiDev.read(&inBuffer, 1);
      updateReadData(inBuffer);
    }
  } else {
    DEVLOG_ERROR("SPI interface has not been opened!n");
  }
}

void CPiFaceProcessInterface::CPiFaceIOHandler::registerIXFB(CPiFaceProcessInterface *paFB) {
  CCriticalRegion readList(mReadFBListSync);
  mReadFBList.push_back(paFB);
}

void CPiFaceProcessInterface::CPiFaceIOHandler::unregisterIXFB(CPiFaceProcessInterface *paFB) {
  CCriticalRegion readList(mReadFBListSync);
  mReadFBList.erase(std::remove(mReadFBList.begin(), mReadFBList.end(), paFB), mReadFBList.end());
}

void CPiFaceProcessInterface::CPiFaceIOHandler::updateReadData(TForteUInt8 paInBuffer) {
  CCriticalRegion readList(mReadFBListSync);
  for (CPiFaceProcessInterface *it : mReadFBList) {
    if (it->checkInputData(paInBuffer)) {
      startNewEventChain(it);
    }
  }
}

void CPiFaceProcessInterface::CPiFaceIOHandler::updateWriteData(bool paValue, int paPin) {
  if (true == paValue) {
    mOutBuffer |= static_cast<TForteUInt8>(1 << paPin);
  } else {
    mOutBuffer &= static_cast<TForteUInt8>(~(1 << paPin));
  }
}

void CPiFaceProcessInterface::CPiFaceIOHandler::enableHandler() {
}

void CPiFaceProcessInterface::CPiFaceIOHandler::disableHandler() {
}

void CPiFaceProcessInterface::CPiFaceIOHandler::setupPiFaceIOChip(CONMELEON::CSpiDevice &paDev) {
  writePiFaceRegister(paDev, eIOConfiguration, 8);
  writePiFaceRegister(paDev, eIODirectionPortA, 0); // set all port A pins as outputs
  writePiFaceRegister(paDev, eIODirectionPortB, 0xFF); // set all port B pins as inputs
  writePiFaceRegister(paDev, eGPIOPullupResistorsPortB, 0xFF); // enable for all port B pins the pull up resistors
}

TForteByte CPiFaceProcessInterface::CPiFaceIOHandler::readInputs(CONMELEON::CSpiDevice &paDev) {
  unsigned char txBuf[3] = {scmPiFaceWrite, eGPIOPortB, 0};
  unsigned char rxBuf[3] = {0, 0, 0};
  paDev.transfer(txBuf, rxBuf, 3);
  return static_cast<TForteByte>(~rxBuf[2]);
}

void CPiFaceProcessInterface::CPiFaceIOHandler::writePiFaceRegister(CONMELEON::CSpiDevice &paDev,
                                                                    EPiFaceRegister paRegister,
                                                                    TForteByte paValue) {
  unsigned char txBuf[3] = {scmPiFaceWrite, paRegister, paValue};
  unsigned char rxBuf[3];
  paDev.transfer(txBuf, rxBuf, 3);
}
