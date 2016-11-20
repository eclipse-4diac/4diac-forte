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
		spi(NULL), nextLoop(0) {
	// Set init time
	struct timespec ts;
	// TODO Check compile error. Had to to add rt libary to c++ make flags
	clock_gettime(CLOCK_REALTIME, &ts);

	initTime = ts.tv_sec;
}

BusHandler::~BusHandler() {

}

void BusHandler::run() {
	// Active spi controller
	spi = new SPIHandler();

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
			ms = millis();
		}

		// send(0, Init);

		// Sleep till next cycle
		unsigned long diff = millis() - ms;
		DEVLOG_INFO("Loop %d - %d\n", ms, diff);
		ms = millis();
		nextLoop = ms + (1000 - ms % 1000);
	}
}

void BusHandler::init() {
	int slaveCounter = 1;
	int attempts = 0;
	Slave *slave;

	while ((slave = Slave::sendInit(slaveCounter)) != 0 || attempts++ < 3) {
		if (slave) {

			slaveCounter++;
		}
	}
}

bool BusHandler::transfer(char target, Command cmd, unsigned char* dataSend,
		int dataSendLength, unsigned char* dataReceive, int dataReceiveLength) {
	int dataLength = std::max(dataSendLength, dataReceiveLength + 1); // + 1 status byte

	int bufferLength = sizeof(Header) + dataLength + 1; // + 1 checksum byte
	unsigned char *buffer = (unsigned char*) calloc(bufferLength, 1);

	Header* header = (Header*) buffer;

	header->address = target;
	header->command = cmd;
	header->checksum = calcChecksum((unsigned char*) header, 2);

	memcpy(buffer + sizeof(Header), dataSend, dataSendLength);
	buffer[sizeof(Header) + dataSendLength] = calcChecksum(dataSend,
			dataSendLength);

	unsigned char *recvBuffer = (unsigned char*) calloc(bufferLength, 1);

	// Invert data of master
	for (int i = 0; i < bufferLength; i++)
		buffer[i] = ~buffer[i];

	// Send and receive buffer via SPI
	int attempts = 3;
	bool ok;
	do {
		// Wait required microseconds between messages
		microsleep(56);

		ok = spi->transfer(buffer, recvBuffer, bufferLength);
		if (!ok) {
			DEVLOG_ERROR("Failed to transfer buffer.\n");
			continue;
		}

		ok = calcChecksum(recvBuffer, bufferLength) == 0;
		if (!ok) {
			DEVLOG_ERROR("Invalid checksum\n");
		}
	} while (!ok && --attempts);

	DEVLOG_INFO("emBrick: TX - %s\n", bytesToHex(buffer, bufferLength).c_str());
	DEVLOG_INFO("emBrick: RX - %s\n",
			bytesToHex(recvBuffer, bufferLength).c_str());

	if (!ok) {
		DEVLOG_ERROR("Failed to send command %d to slave %d.\n", cmd, target);
		return false;
	}

	// Copy result
	SlaveStatus status = (SlaveStatus) recvBuffer[0]; // TODO Handle slave status
	memcpy(dataReceive, recvBuffer + 1, dataReceiveLength);

	free(buffer);
	free(recvBuffer);

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
	char buffer[length * 3];
	buffer[length * 3 - 1] = 0;
	for (int i = 0; i < length; i++)
		sprintf(&buffer[3 * i], "%02X ", bytes[i]);

	return buffer;
}

} /* namespace EmBrick */

