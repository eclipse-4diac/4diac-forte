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

namespace Packages {

SlaveInit SlaveInit::fromBuffer(unsigned char* buffer) {
	SlaveInit pkg;
	memcpy(&pkg, buffer, sizeof(SlaveInit));

	pkg.deviceId = ntohs(pkg.deviceId);
	pkg.manufactorId = ntohs(pkg.manufactorId);

	return pkg;
}

void MasterInit::toBuffer(unsigned char* buffer) {
	buffer[0] = slaveAddress;

	uint16_t syncGapFactor = htons(this->syncGapFactor);
	memcpy(buffer + 1, &syncGapFactor, 2);
}

} /* namespace Packages */

Slave::Slave() {
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
	bus.transfer(0, Init, sendBuffer, sizeof(Packages::MasterInit), receiveBuffer,
			sizeof(Packages::SlaveInit));
	Packages::SlaveInit initPackage = Packages::SlaveInit::fromBuffer(
			receiveBuffer);

	// TODO Create slave instance from init package

	return 0;
}

} /* namespace EmBrick */
