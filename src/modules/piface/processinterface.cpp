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


CPiFaceProcessInterface::CPiFaceProcessInterface(CResource *paSrcRes,
    const SFBInterfaceSpec *paInterfaceSpec,
    const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData,
    TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData),
        mPin(0){
}

CPiFaceProcessInterface::~CPiFaceProcessInterface(){
  deinitialise();
}

bool CPiFaceProcessInterface::initialise(bool paInput){
  bool retVal = false;
  if(paInput){
    CPiFaceIOHandler::getInstance().registerIXFB(this);
  }
  QO() = QI();
  if(!CPiFaceIOHandler::getInstance().isAlive()){
    CPiFaceIOHandler::getInstance().start();
  }
  CIEC_INT pinNum;
  if((-1 != pinNum.fromString(PARAMS().getValue())) && (8 > pinNum)){
    mPin = pinNum;
    retVal = true;
  }
  return retVal;
}

bool CPiFaceProcessInterface::deinitialise(){
  CPiFaceIOHandler::getInstance().unregisterIXFB(this);
  return true;
}

bool CPiFaceProcessInterface::getValue(long paValue){
  return 0 != (paValue & (1 << mPin));
}

bool CPiFaceProcessInterface::readPin(){
  //We don't need to do anything in readPin output update is handled as part of checkInputData
  return true;
}

bool CPiFaceProcessInterface::writePin(){
  CPiFaceIOHandler::getInstance().updateWriteData(OUT_X(), mPin);
  return true;
}

bool CPiFaceProcessInterface::checkInputData(long paValue){
  bool retval = false;
  bool newValue = getValue(paValue);
  if(newValue != IN_X()){
    IN_X() = newValue;
    retval = true;
  }
  return retval;
}


/********************************************************************************************
 ***  CPiFaceProcessInterface::CPiFaceIOHandler
 ********************************************************************************************/

DEFINE_SINGLETON(CPiFaceProcessInterface::CPiFaceIOHandler)


CPiFaceProcessInterface::CPiFaceIOHandler::CPiFaceIOHandler(){
}

CPiFaceProcessInterface::CPiFaceIOHandler::~CPiFaceIOHandler(){
}

void CPiFaceProcessInterface::CPiFaceIOHandler::run(){
  //this gives as the default values of Mode0, Speed 1000000, and Bitsperword 8 which wed need also for piFace
  CONMELEON::CSpiDevice spiDev("/dev/spidev0.0");  //TODO correctly setup SPI interface
  setupPiFaceIOChip(spiDev);

  if(spiDev.isOpen()){
    timespec req, rem;
    TForteUInt8 inBuffer;

    req.tv_nsec = 10 * 1000 * 1000;  //TODO make scan cycle setable from the application

    while(isAlive()){
      nanosleep(&req, &rem);

      writePiFaceRegister(spiDev, eGPIOPortA, mOutBuffer);  //update outputs
      inBuffer = readInputs(spiDev);
      spiDev.read(&inBuffer, 1);
      updateReadData(inBuffer);
    }
  }
  else{
    DEVLOG_ERROR("SPI interface has not been opened!n");
  }
}

void CPiFaceProcessInterface::CPiFaceIOHandler::registerIXFB(CPiFaceProcessInterface *pa_poFB){
  m_oReadFBListSync.lock();
  m_lstReadFBList.push_back(pa_poFB);
  m_oReadFBListSync.unlock();
}

void CPiFaceProcessInterface::CPiFaceIOHandler::unregisterIXFB(CPiFaceProcessInterface *pa_poFB){
  m_oReadFBListSync.lock();
  TReadFBContainer::Iterator itRunner(m_lstReadFBList.begin());
  TReadFBContainer::Iterator itRefNode(m_lstReadFBList.end());
  TReadFBContainer::Iterator itEnd(m_lstReadFBList.end());
  while(itRunner != itEnd){
    if(*itRunner == pa_poFB){
      if(itRefNode == itEnd){
        m_lstReadFBList.pop_front();
      }
      else{
        m_lstReadFBList.eraseAfter(itRefNode);
      }
      break;
    }
    itRefNode = itRunner;
    ++itRunner;
  }
  m_oReadFBListSync.unlock();
}

void CPiFaceProcessInterface::CPiFaceIOHandler::updateReadData(TForteUInt8 paInBuffer){
  TReadFBContainer::Iterator itEnd(m_lstReadFBList.end());
  for(TReadFBContainer::Iterator itRunner = m_lstReadFBList.begin(); itRunner != itEnd; ++itRunner){
    if((*itRunner)->checkInputData(paInBuffer)){
      startNewEventChain(*itRunner);
    }
  }
}

void CPiFaceProcessInterface::CPiFaceIOHandler::updateWriteData(bool paValue, int paPin){
  if(true == paValue){
    mOutBuffer |= static_cast<TForteUInt8>(1 << paPin);
  }
  else{
    mOutBuffer &= static_cast<TForteUInt8>(~(1 << paPin));
  }
}

void CPiFaceProcessInterface::CPiFaceIOHandler::enableHandler(void){
}

void CPiFaceProcessInterface::CPiFaceIOHandler::disableHandler(void){
}

void CPiFaceProcessInterface::CPiFaceIOHandler::setPriority(int){
}

int CPiFaceProcessInterface::CPiFaceIOHandler::getPriority(void) const{
  return 0;
}

void CPiFaceProcessInterface::CPiFaceIOHandler::setupPiFaceIOChip(CONMELEON::CSpiDevice &paDev){
  writePiFaceRegister(paDev, eIOConfiguration, 8);
  writePiFaceRegister(paDev, eIODirectionPortA, 0);  //set all port A pins as outputs
  writePiFaceRegister(paDev, eIODirectionPortB, 0xFF);  //set all port B pins as inputs
  writePiFaceRegister(paDev, eGPIOPullupResistorsPortB, 0xFF);  //enable for all port B pins the pull up resistors
}

TForteByte CPiFaceProcessInterface::CPiFaceIOHandler::readInputs(CONMELEON::CSpiDevice &paDev){
  unsigned char txBuf[3] = {scmPiFaceWrite, eGPIOPortB, 0};
  unsigned char rxBuf[3] = {0, 0 ,0 };
  paDev.transfer(txBuf, rxBuf, 3);
  return static_cast<TForteByte>(~rxBuf[2]);
}

void CPiFaceProcessInterface::CPiFaceIOHandler::writePiFaceRegister(CONMELEON::CSpiDevice &paDev, EPiFaceRegister paRegister, TForteByte paValue){
  unsigned char txBuf[3] = {scmPiFaceWrite, paRegister, paValue};
  unsigned char rxBuf[3];
  paDev.transfer(txBuf, rxBuf, 3);
}
