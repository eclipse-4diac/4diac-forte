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

#ifndef SRC_MODULES_EMBRICK_BUSCONTROLLER_H_
#define SRC_MODULES_EMBRICK_BUSCONTROLLER_H_

#include <extevhan.h>
#include <thread.h>
#include <devlog.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>
#include <timerha.h>
#include <string>
#include <pthread.h>
#include "spi.h"

namespace EmBrick {

enum Command {
	Init = 2, SelectNextSlave = 3, Data = 10
};

enum SlaveStatus {
	NotInitialized = 0, // Slave requires initialization
	OK = 1, // Everything works as expected
	Slow = 200, // Update frequency is too low, some errors may occurred
	Interrupted = 201, // Slave received no master updates
	Error = 202, // Connection has errors. Check hardware
};

class BusHandler: public CExternalEventHandler, public CThread {
DECLARE_SINGLETON(BusHandler)

public:
	void init();
	bool transfer(char target, Command cmd, unsigned char* dataSend = NULL,
			int dataSendLength = 0, unsigned char* dataReceive = NULL,
			int dataReceiveLength = 0);

protected:

#pragma pack(push, 1) // Disable padding for protocol structs

	struct Header {
		char address;
		char command;
		char checksum;
	};

#pragma pack(pop)

	virtual void run();

	// Functions needed for the external event handler interface
	void enableHandler(void);
	void disableHandler(void);
	void setPriority(int paPriority);
	int getPriority(void) const;

	unsigned long nextLoop;
	SPIHandler *spi;

private:
	unsigned long millis();
	time_t initTime;
	void microsleep(unsigned long microseconds);
	unsigned char calcChecksum(unsigned char * data, int dataLen);

	std::string bytesToHex(unsigned char* bytes, int length);

	static const unsigned char ChecksumConstant = 0x55;
};

}

#endif /* SRC_MODULES_EMBRICK_BUSCONTROLLER_H_ */
