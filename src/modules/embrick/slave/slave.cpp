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
#include "packages.h"

namespace EmBrick {

Slave::Slave(int address) :
		address(address) {
	// TODO Auto-generated constructor stub
}

Slave::~Slave() {
	// TODO Auto-generated destructor stub
}

Slave* Slave::sendInit(int address) {
	BusHandler &bus = BusHandler::getInstance();

	Packages::MasterInit masterInit;
	masterInit.slaveAddress = (unsigned char) address;
	masterInit.syncGapFactor = 3; // ToDo: Replace with dynamic constant

	unsigned char sendBuffer[sizeof(Packages::MasterInit)];
	unsigned char receiveBuffer[sizeof(Packages::SlaveInit)];

	masterInit.toBuffer(sendBuffer);

	// Send init via broadcast. Due to the sequential slave select activation, only one slave will respond.
	if (!bus.broadcast(Init, sendBuffer, sizeof(Packages::MasterInit),
			receiveBuffer, sizeof(Packages::SlaveInit)))
		return 0;
	Packages::SlaveInit initPackage = Packages::SlaveInit::fromBuffer(
			receiveBuffer);

	// TODO Create slave instance from init package

	return new Slave(address);
}

} /* namespace EmBrick */
