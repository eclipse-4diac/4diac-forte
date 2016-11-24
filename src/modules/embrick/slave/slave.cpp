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
#include "slave2181.h"
#include "../handler/bus.h"
#include "slave2301.h"

namespace EmBrick {

Slave::Slave(int address, Packages::SlaveInit init) :
		address(address), deviceId(init.deviceId), dataSendLength(
				init.dataSendLength), dataReceiveLength(init.dataReceiveLength), status(
				NotInitialized) {
	bus = &BusHandler::getInstance();
	updateSendImage = new unsigned char[dataSendLength];
	updateReceiveImage = new unsigned char[dataReceiveLength];
}

Slave::~Slave() {
	delete updateSendImage;
	delete updateReceiveImage;
}

Slave* Slave::sendInit(int address) {
	BusHandler &bus = BusHandler::getInstance();

	Packages::MasterInit masterInit;
	masterInit.slaveAddress = (unsigned char) address;
	masterInit.syncGapMultiplicator = SyncGapMultiplicator; // ToDo: Replace with dynamic constant

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

	// Create slave instance based on type
	switch ((SlaveType) initPackage.deviceId) {

	case G_8Di8Do:
		return new Slave2181(address, initPackage);

	case G_2RelNo4RelCo:
		return new Slave2301(address, initPackage);

	default:
		DEVLOG_ERROR("emBrick[Slave]: Unknown slave type %d\n",
				initPackage.deviceId);
	}

	// Return a global slave instance
	return new Slave(address, initPackage);
}

bool Slave::update() {
	// Prepare the send image
	prepareUpdate();

	// Send update request to bus
	if (!bus->transfer(address, Data, updateSendImage, dataSendLength,
			updateReceiveImage, dataReceiveLength, &status))
		return false;

	// Handle the received image
	handleUpdate();

	return true;
}

} /* namespace EmBrick */
