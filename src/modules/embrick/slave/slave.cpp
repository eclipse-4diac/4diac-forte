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
#include "../handler/bus.h"
#include <processinterface.h>

namespace EmBrick {

Slave::Slave(int address, Packages::SlaveInit init) :
    address(address), type((SlaveType) init.deviceId), dataSendLength(
        init.dataSendLength), dataReceiveLength(init.dataReceiveLength), status(
        NotInitialized) {
  bus = &BusHandler::getInstance();
  updateSendImage = new unsigned char[dataSendLength];
  updateReceiveImage = new unsigned char[dataReceiveLength];
  updateReceiveImageOld = new unsigned char[dataReceiveLength];

  memset(updateSendImage, 0, dataSendLength);
  memset(updateReceiveImage, 0, dataReceiveLength);
  memset(updateReceiveImageOld, 0, dataReceiveLength);
}

Slave::~Slave() {
  delete updateSendImage;
  delete updateReceiveImage;
  delete updateReceiveImageOld;

  dropHandles();
}

Slave* Slave::sendInit(int address) {
  BusHandler &bus = BusHandler::getInstance();

  Packages::MasterInit masterInit;
  masterInit.slaveAddress = (unsigned char) address;
  masterInit.syncGapMultiplicator = SyncGapMultiplicator;

  unsigned char sendBuffer[sizeof(Packages::MasterInit)];
  unsigned char receiveBuffer[sizeof(Packages::SlaveInit)];

  masterInit.toBuffer(sendBuffer);

  // Send init via broadcast. Due to the sequential slave select activation, only one slave will respond.
  if (!bus.broadcast(Init, sendBuffer, sizeof(Packages::MasterInit),
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
  return new Slave(address, initPackage);
}

bool Slave::update() {
  // TODO Prepare the send image

  // Send update request to bus
  if (!bus->transfer(address, Data, updateSendImage, dataSendLength,
      updateReceiveImage, dataReceiveLength, &status, &syncMutex))
    return false;

// Handle the received image
  syncMutex.lock();
  TSlaveHandleList::Iterator itEnd = inputs.end();
  for (TSlaveHandleList::Iterator it = inputs.begin(); it != itEnd; ++it)
    if ((*it)->hasObserver() && !(*it)->equal(updateReceiveImageOld)) {
      // Inform Process Interface about change
      if ((*it)->getObserver()->onChange()) {
        // Send indication event
        bus->startNewEventChain((ProcessInterface*) (*it)->getObserver());
      }
    }
  syncMutex.unlock();

  // Clone current image to old image
  memcpy(updateReceiveImageOld, updateReceiveImage, dataReceiveLength);

  return true;
}

void Slave::dropHandles() {
  syncMutex.lock();

  TSlaveHandleList::Iterator itEnd = inputs.end();
  for (TSlaveHandleList::Iterator it = inputs.begin(); it != itEnd; ++it)
    delete *it;
  itEnd = outputs.end();
  for (TSlaveHandleList::Iterator it = outputs.begin(); it != itEnd; ++it)
    delete *it;

  inputs.clearAll();
  outputs.clearAll();

  syncMutex.unlock();
}

void Slave::addHandle(TSlaveHandleList* list, SlaveHandle* handle) {
  syncMutex.lock();
  list->push_back(handle);
  syncMutex.unlock();

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

} /* namespace EmBrick */
