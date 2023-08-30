/*******************************************************************************
 * Copyright (c) 2012 - 2023 AIT, Davor Cihlar
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *   Davor Cihlar - multiple FBs sharing a single Modbus connection
 *******************************************************************************/
#include "modbusclientconnection.h"
#include "devlog.h"
#include "modbuspoll.h"
#include <forte_thread.h>
#include <unistd.h> // for sleep
// for open and disabling DTR
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace modbus_connection_event;

/*************************************
 * CModbusClientConnection class
 *************************************/

CModbusClientConnection::CModbusClientConnection(CModbusHandler* pa_modbusHandler) :
    CModbusConnection(pa_modbusHandler), mModbusConnEvent(nullptr), mSlaveId(0xFF){
}

CModbusClientConnection::~CModbusClientConnection() {
  if (mConnected){
    disconnect();
  }
  for (auto itRunner : mPollList) {
    delete itRunner;
  }
  if (mModbusConnEvent != nullptr){
    delete mModbusConnEvent;
  }
}

int CModbusClientConnection::readData(CModbusIOBlock* paIOBlock, void* paData, unsigned int paMaxDataSize){
  const unsigned int size = std::min(paMaxDataSize, paIOBlock->getReadSize());
  memcpy(paData, paIOBlock->getCache(), size);
  return (int)size;
}

void CModbusClientConnection::writeDataRange(EModbusFunction paFunction, unsigned int paStartAddress, unsigned int paNrAddresses, const void *paData){
  CCriticalRegion criticalRegion(mModbusLock);
  if (!mConnected) {
    // TODO: error
    return;
  }
  switch (paFunction) {
    case eCoil:
      modbus_write_bits(mModbusConn, paStartAddress, paNrAddresses, (const uint8_t*)paData);
      break;
    case eHoldingRegister:
      modbus_write_registers(mModbusConn, paStartAddress, paNrAddresses, (const uint16_t*)paData);
      break;
    default:
      // TODO: error
      break;
  }
}

int CModbusClientConnection::connect(){
  CModbusConnection::connect();

  if(mSlaveId != 0xFF){
    modbus_set_slave(mModbusConn, mSlaveId);
  }

  mModbusConnEvent = new CModbusConnectionEvent(1000, getFlowControl(), getDevice());
  mModbusConnEvent->activate();

  this->start();

  return 0;
}

void CModbusClientConnection::disconnect(){
  this->end();
  if (mConnected){
    modbus_close(mModbusConn);
    mConnected = false;
  }
  CModbusConnection::disconnect();
}

void CModbusClientConnection::addNewPoll(long paPollInterval, CModbusIOBlock* paIOBlock){
  CModbusPoll *newPoll = nullptr;

  for (auto it : mPollList) {
    if(it->getUpdateInterval() == paPollInterval){
      newPoll = it;
      break;
    }
  }
  if(newPoll == nullptr){
    newPoll = new CModbusPoll(mModbusHandler, paPollInterval);
    mPollList.push_back(newPoll);
  }

  newPoll->addPollBlock(paIOBlock);
}

void CModbusClientConnection::setSlaveId(unsigned int paSlaveId){
  mSlaveId = paSlaveId;
}

void CModbusClientConnection::run(){

  while(isAlive()){
    if(mConnected){
      tryPolling();
    }
    else{
      tryConnect();
    }

    CThread::sleepThread(1);
  }
}

void CModbusClientConnection::tryPolling(){
  unsigned int nrErrors = 0, nrPolls = 0;

  for (size_t index = 0; index < mPollList.size(); ++index) {
    auto itPoll = mPollList[index];

    if(itPoll->readyToExecute()){
      CCriticalRegion criticalRegion(mModbusLock);

      int nrVals = itPoll->executeEvent(mModbusConn, 0);

      if(nrVals < 0){
        DEVLOG_ERROR("Error reading input status :: %s\n", modbus_strerror(errno));
        itPoll->deactivate();

        nrErrors++;
      }
      ++nrPolls;
    }
  }

  if((nrErrors == nrPolls) && nrPolls && !mPollList.empty()){
    DEVLOG_WARNING("Too many errors on Modbus, reconnecting\n");
    CCriticalRegion criticalRegion(mModbusLock);
    modbus_close(mModbusConn); // in any case it is worth trying to close the socket
    mConnected = false;
    mModbusConnEvent = new CModbusConnectionEvent(1000, getFlowControl(), getDevice());
    mModbusConnEvent->activate();
  }
}

void CModbusClientConnection::tryConnect(){
  if(mModbusConnEvent != nullptr){
    if(mModbusConnEvent->readyToExecute()){
      CCriticalRegion criticalRegion(mModbusLock);
      if(mModbusConnEvent->executeEvent(mModbusConn, nullptr) < 0) {
        DEVLOG_ERROR("Connection to Modbus server failed: %s\n", modbus_strerror(errno));
      } else {
        DEVLOG_INFO("Connection to Modbus server succeded\n");

        delete mModbusConnEvent;
        mModbusConnEvent = nullptr;

        mConnected = true;

        // Start polling
        for (auto itPoll : mPollList) {
          itPoll->activate();
        }
      }
    }
  }
}

/*************************************
 * CModbusConnectionEvent class
 *************************************/
CModbusConnectionEvent::CModbusConnectionEvent(long paReconnectInterval, EModbusFlowControl paFlowControl, const char *paDevice) :
    CModbusTimedEvent((TForteUInt32)paReconnectInterval), mFlowControl(paFlowControl){
  strcpy(mDevice, paDevice);
}

int CModbusConnectionEvent::executeEvent(modbus_t *paModbusConn, void *paRetVal){
  (void)paRetVal; // avoid warning

  restartTimer();

  switch (mFlowControl) {
    case eFlowArduino: {
      int fd = open(mDevice, O_RDWR);
      if (fd >= 0) {
        termios tty;
        tcgetattr(fd, &tty);
        if (tty.c_cflag & (HUPCL | CRTSCTS)) {
          tty.c_cflag &= ~(HUPCL | CRTSCTS);
          if (!tcsetattr(fd, TCSANOW, &tty)) {
            DEVLOG_INFO("Hardware flow control for Modbus RTU disabled\n");
            // Disabling DTR is not perfect and it will be toggled by this open for disabling it.
            // Therefore, wait for Arduino to boot only if flags weren't previously set.
            sleep(2);
          } else {
            DEVLOG_ERROR("Failed disabling flow control for Modbus RTU\n");
            return -1;
          }
        } else {
          DEVLOG_INFO("Hardware flow control for Modbus RTU was already disabled\n");
        }
        close(fd);
      }
      break;
    }
    default:
      // ignore
      break;
  };

  int retVal = modbus_connect(paModbusConn);

  if (retVal >= 0) {
    switch (mFlowControl) {
      case eFlowLongDelay:
        sleep(3);
        // fall through
      case eFlowDelay:
        sleep(2);
        break;
      default:
        // ignore
        break;
    }
  }

  return retVal;
}

