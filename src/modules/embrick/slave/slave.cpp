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

namespace EmBrick {

Slave::Slave(int address, Packages::SlaveInit init) :
    address(address), deviceId(init.deviceId), dataSendLength(
        init.dataSendLength), dataReceiveLength(init.dataReceiveLength), status(
        NotInitialized) {
  bus = &BusHandler::getInstance();
  updateSendImage = new unsigned char[dataSendLength];
  updateReceiveImage = new unsigned char[dataReceiveLength];
  updateReceiveImageOld = new unsigned char[dataReceiveLength];
}

Slave::~Slave() {
  delete updateSendImage;
  delete updateReceiveImage;
  delete updateReceiveImageOld;

  TSlaveHandleList::Iterator itEnd = inputs.end();
  for (TSlaveHandleList::Iterator it = inputs.begin(); it != itEnd; ++it)
    delete *it;
  itEnd = outputs.end();
  for (TSlaveHandleList::Iterator it = outputs.begin(); it != itEnd; ++it)
    delete *it;
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
    return NULL;

  Packages::SlaveInit initPackage = Packages::SlaveInit::fromBuffer(
      receiveBuffer);

  // Alter the value of data receive length -> the status byte is handled in the BusHandler
  initPackage.dataReceiveLength--;

  DEVLOG_INFO(
      "emBrick[Slave]: ID - %d, ReceiveLength - %d, SendLength - %d, Producer - %d \n",
      initPackage.deviceId, initPackage.dataReceiveLength,
      initPackage.dataSendLength, initPackage.producerId);

  // Create slave instance
  Slave* slave = new Slave(address, initPackage);

  // Init slave handles
  // TODO Move this logic to the config
  switch ((SlaveType) initPackage.deviceId) {

  case G_8Di8Do:
    // 8 Inputs
    for (uint8_t pos = 0; pos < 8; pos++)
      slave->inputs.push_back(
          new BitSlaveHandle(slave->updateReceiveImage, pos));
    // 8 Outputs
    for (uint8_t pos = 0; pos < 8; pos++)
      slave->outputs.push_back(new BitSlaveHandle(slave->updateSendImage, pos));
    break;

  case G_2RelNo4RelCo:
    // 6 Relays
    for (uint8_t pos = 0; pos < 6; pos++)
      slave->outputs.push_back(new BitSlaveHandle(slave->updateSendImage, pos));
    break;

  default:
    DEVLOG_ERROR("emBrick[Slave]: Unknown slave type %d\n",
        initPackage.deviceId);
  }

  return slave;
}

bool Slave::update() {
  // TODO Prepare the send image

  // Send update request to bus
  if (!bus->transfer(address, Data, updateSendImage, dataSendLength,
      updateReceiveImage, dataReceiveLength, &status))
    return false;

  // Handle the received image
  TSlaveHandleList::Iterator itEnd = inputs.end();
  int i = 0;
  for (TSlaveHandleList::Iterator it = inputs.begin(); it != itEnd; ++it) {
    i++;
    if (!(*it)->equal(updateReceiveImageOld)) {
      // TODO Fire event handler and send indication event
      DEVLOG_INFO("Input changed %d\n", i);
    }
  }

  // Clone current image to old image
  memcpy(updateReceiveImageOld, updateReceiveImage, dataReceiveLength);

  return true;
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
