/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "slave.h"
#include <handler/bus.h>
#include <io/mapper/io_mapper.h>
#include <processinterface.h>

namespace EmBrick {
namespace Handlers {

const int Slave::MaxUpdateErrors = 50;

Slave::Slave(Bus* bus, int address, Packages::SlaveInit init) :
    bus(bus), delegate(0), address(address), type((SlaveType) init.deviceId), dataSendLength(
        init.dataSendLength), dataReceiveLength(init.dataReceiveLength), status(
        NotInitialized), oldStatus(NotInitialized) {
  updateSendImage = new unsigned char[dataSendLength];
  updateReceiveImage = new unsigned char[dataReceiveLength];
  updateReceiveImageOld = new unsigned char[dataReceiveLength];

  memset(updateSendImage, 0, dataSendLength);
  memset(updateReceiveImage, 0, dataReceiveLength);
  memset(updateReceiveImageOld, 0, dataReceiveLength);

  updateErrorCounter = 0;

  // Default config
  config.UpdateInterval = 25;
}

Slave::~Slave() {
  dropHandles();

  delete updateSendImage;
  delete updateReceiveImage;
  delete updateReceiveImageOld;

  if (delegate != 0)
    delegate->onSlaveDestroy();
}

void Slave::setConfig(Config config) {
  if (config.UpdateInterval < 20) {
    DEVLOG_WARNING(
        "embrick[Slave]: Configured UpdateInterval not in range (>= 20). Set to 25.\n");
    config.UpdateInterval = 25;
  }

  this->config = config;
}

Slave* Slave::sendInit(Bus* bus, int address) {
  Packages::MasterInit masterInit;
  masterInit.slaveAddress = (unsigned char) address;
  masterInit.syncGapMultiplicator = SyncGapMultiplicator;

  unsigned char sendBuffer[sizeof(Packages::MasterInit)];
  unsigned char receiveBuffer[sizeof(Packages::SlaveInit)];

  masterInit.toBuffer(sendBuffer);

  // Send init via broadcast. Due to the sequential slave select activation, only one slave will respond.
  if (!bus->broadcast(Init, sendBuffer, sizeof(Packages::MasterInit),
      receiveBuffer, sizeof(Packages::SlaveInit)))
    return 0;

  Packages::SlaveInit initPackage = Packages::SlaveInit::fromBuffer(
      receiveBuffer);

  // Alter the value of data receive length -> the status byte is handled in the BusHandler
  initPackage.dataReceiveLength--;

  DEVLOG_INFO(
      "emBrick[Slave]: ID - %d, ReceiveLength - %d, SendLength - %d, Producer - %d \n",
      initPackage.deviceId, initPackage.dataReceiveLength,
      initPackage.dataSendLength, initPackage.producerId);

  // Return slave instance
  return new Slave(bus, address, initPackage);
}

int Slave::update() {
  // Send update request to bus
  if (!bus->transfer(address, Data, updateSendImage, dataSendLength,
      updateReceiveImage, dataReceiveLength, &status, &updateMutex)) {
    updateErrorCounter++;
    if (updateErrorCounter % 10 == 0)
      DEVLOG_WARNING(
          "emBrick[Slave]: Slave %d reached transfer error threshold - %d out of %d\n",
          address, updateErrorCounter, MaxUpdateErrors);
    return updateErrorCounter <= MaxUpdateErrors ? 0 : -1;
  }

  // Handle the received image
  handleMutex.lock();
  TSlaveHandleList::Iterator itEnd = inputs.end();
  for (TSlaveHandleList::Iterator it = inputs.begin(); it != itEnd; ++it)
    if ((*it)->hasObserver() && !(*it)->equal(updateReceiveImageOld)) {
      // Inform Process Interface about change
      if ((*it)->getObserver()->onChange()) {
        // Send indication event
        bus->startNewEventChain((CProcessInterface*) (*it)->getObserver());
      }
    }
  handleMutex.unlock();

  // Clone current image to old image
  memcpy(updateReceiveImageOld, updateReceiveImage, dataReceiveLength);

  // Reset error counter
  if (updateErrorCounter > 0)
    updateErrorCounter = 0;

  // Check status
  if (delegate != 0 && oldStatus != status) {
    delegate->onSlaveStatus(status, oldStatus);
    oldStatus = status;
  }

  return 1;
}

void Slave::forceUpdate() {
  return bus->forceUpdate(index());
}

void Slave::dropHandles() {
  handleMutex.lock();

  Mapper& mapper = Mapper::getInstance();

  TSlaveHandleList::Iterator itEnd = inputs.end();
  for (TSlaveHandleList::Iterator it = inputs.begin(); it != itEnd; ++it) {
    mapper.deregisterHandle(*it);
    delete *it;
  }
  itEnd = outputs.end();
  for (TSlaveHandleList::Iterator it = outputs.begin(); it != itEnd; ++it) {
    mapper.deregisterHandle(*it);
    delete *it;
  }

  inputs.clearAll();
  outputs.clearAll();

  handleMutex.unlock();
}

void Slave::addHandle(TSlaveHandleList* list, SlaveHandle* handle) {
  handleMutex.lock();
  list->push_back(handle);
  handleMutex.unlock();

  // TODO Maybe send indication event after connecting
}

SlaveHandle* Slave::getHandle(TSlaveHandleList* list, int index) {
  TSlaveHandleList::Iterator itEnd = list->end();
  int i = 0;
  for (TSlaveHandleList::Iterator it = list->begin(); it != itEnd; ++it, i++)
    if (index == i)
      return *it;
  return NULL;
}

} /* namespace Handlers */
} /* namespace EmBrick */

