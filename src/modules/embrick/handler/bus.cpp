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

#include "bus.h"

#include <algorithm>
#include <slave/slave.h>

namespace EmBrick {

DEFINE_SINGLETON(BusHandler)

BusHandler::BusHandler() :
		nextLoop(0), spi(NULL), slaveSelect(NULL) {
	// Set init time
	struct timespec ts;
	// TODO Check compile error. Had to to add rt libary to c++ make flags
	clock_gettime(CLOCK_REALTIME, &ts);

	initTime = ts.tv_sec;
}

BusHandler::~BusHandler() {
}

void BusHandler::run() {
	// Start handlers
	spi = new SPIHandler();
	slaveSelect = new PinHandler(49);
	// TODO Check status of handler errors
	DEVLOG_INFO("emBrick[BusHandler]: Handlers ready.\n");

	// Init bus
	init();

	unsigned long ms;
	while (isAlive()) {
		ms = millis();
		if (nextLoop == 0)
			nextLoop = ms + (100 - ms % 100);

		while (nextLoop > ms) {
			microsleep(std::min((nextLoop - ms), (unsigned long) 2) * 1000);

			// TODO Check for updates of forte runtime -> maybe schedule next loop earlier
			// DISCUSS Use cond_timed_wait pthread to wake up thread
			ms = millis();
		}

		// TODO Perform requests to slaves
		// DISCUSS Use scheduling strategy or a cyclic routine

		// Sleep till next cycle
		unsigned long diff = millis() - ms;
		DEVLOG_INFO("emBrick[BusHandler]: Loop %d - %d\n", ms, diff);
		ms = millis();
		nextLoop = ms + (1000 - ms % 1000);
	}

	delete spi;
	delete slaveSelect;

	DEVLOG_INFO("emBrick[BusHandler]: Stopped.\n");
}

void BusHandler::init() {
	// Disable slave select -> reset all slaves
	slaveSelect->disable();

	// Wait for reset
	microsleep(1000); // TODO replace with dynamic variable

	// Enable slave select -> the first slave waits for initialization
	slaveSelect->enable();

	// Wait for init
	microsleep(1000); // TODO replace with dynamic variable

	// Init the slaves sequentially. Abort if the init package is ignored -> no further slaves found.
	int slaveCounter = 1;
	int attempts = 0;
	Slave *slave;

	while ((slave = Slave::sendInit(slaveCounter)) != 0 || attempts++ < 3) {
		if (slave) {
			// TODO Add slave to list

			// Activate next slave by sending the 'SelectNextSlave' command to the current slave
			// It enables the slave select pin for the next slave on the bus
			transfer(slave->address, SelectNextSlave);

			slaveCounter++;
		}
	}
}

bool BusHandler::transfer(unsigned int target, Command cmd,
		unsigned char* dataSend, int dataSendLength, unsigned char* dataReceive,
		int dataReceiveLength) {
	int dataLength = std::max(dataSendLength, dataReceiveLength + 1); // + 1 status byte

	int bufferLength = sizeof(Header) + dataLength + 1; // + 1 checksum byte
	unsigned char buffer[bufferLength];
	unsigned char recvBuffer[bufferLength];
	memset(buffer, 0, bufferLength);
	memset(recvBuffer, 0, bufferLength);

	Header* header = (Header*) buffer;

	header->address = (char) target;
	header->command = cmd;
	header->checksum = calcChecksum((unsigned char*) header, 2);

	memcpy(buffer + sizeof(Header), dataSend, dataSendLength);
	buffer[bufferLength - 1] = calcChecksum(dataSend, dataSendLength);

	DEVLOG_DEBUG("emBrick[BusHandler]: TX - %s\n", bytesToHex(buffer, bufferLength).c_str());

	// Invert data of master
	for (int i = 0; i < bufferLength; i++)
		buffer[i] = ~buffer[i];

	// Send and receive buffer via SPI
	int attempts = 3;
	bool ok;
	do {
		// Wait required microseconds between messages
		microsleep(56); // TODO replace with dynamic variable

		ok = spi->transfer(buffer, recvBuffer, bufferLength);
		if (!ok) {
			DEVLOG_ERROR("emBrick[BusHandler]: Failed to transfer buffer.\n");
			continue;
		}

		DEVLOG_DEBUG("emBrick[BusHandler]: RX - %s, CS - %d\n",
				bytesToHex(recvBuffer, bufferLength).c_str(),
				calcChecksum(recvBuffer, bufferLength));

		ok = calcChecksum(recvBuffer, bufferLength) == 0;
		if (!ok) {
			DEVLOG_DEBUG("emBrick[BusHandler]: Transfer - Invalid checksum\n");
		}
	} while (!ok && --attempts);

	if (!ok) {
		DEVLOG_ERROR("emBrick[BusHandler]: Failed to send command %d to slave %d.\n", cmd, target);
		return false;
	}

	// Copy result
	SlaveStatus status = (SlaveStatus) recvBuffer[0]; // TODO Handle slave status
	memcpy(dataReceive, recvBuffer + sizeof(Header) + 1, dataReceiveLength);

	return true;
}

unsigned char BusHandler::calcChecksum(unsigned char * data, int dataLen) {
	unsigned char c = 0;
	for (int i = 0; i < dataLen; i++)
		c += data[i];

	return ChecksumConstant - c;
}

void BusHandler::disableHandler() {

}

void BusHandler::enableHandler() {

}

void BusHandler::setPriority(int) {

}

int BusHandler::getPriority() const {
	return 0;
}

unsigned long BusHandler::millis() {
	// TODO Improve timing func. Maybe replace with existing forte implementation.
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);

	return round(ts.tv_nsec / 1.0e6) + (ts.tv_sec - initTime) * 1000;
}

void BusHandler::microsleep(unsigned long microseconds) {
	struct timespec ts;
	ts.tv_sec = microseconds / (unsigned long) 1000000;
	ts.tv_nsec = microseconds * 1000 - ts.tv_sec * 1000000000;
	nanosleep(&ts, NULL);
}

std::string BusHandler::bytesToHex(unsigned char* bytes, int length) {
	// TODO Move helper functions to helper class or general forte utils class
	char buffer[length * 3];
	buffer[length * 3 - 1] = 0;
	for (int i = 0; i < length; i++)
		sprintf(&buffer[3 * i], "%02X ", bytes[i]);

	return buffer;
}

} /* namespace EmBrick */

