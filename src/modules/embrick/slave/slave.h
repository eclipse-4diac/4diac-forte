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

#ifndef SRC_MODULES_EMBRICK_SLAVE_SLAVE_H_
#define SRC_MODULES_EMBRICK_SLAVE_SLAVE_H_

#include <stdint.h>
#include <cstring>
#include <netinet/in.h>

namespace EmBrick {

namespace Packages {

#pragma pack(push, 1) // Disable padding for protocol structs

struct SlaveInit {
	uint8_t protocolVersion;
	uint8_t moduleVersion;
	uint16_t deviceId;
	uint16_t manufactorId;
	uint8_t dataSendLength; // Amount of bytes that the slave expects from the master
	uint8_t dataReceiveLength; // Amount of bytes that the master expects from the slave

	static SlaveInit fromBuffer(unsigned char* buffer);
};

struct MasterInit {
	uint8_t slaveAddress;
	uint16_t syncGapFactor;

	void toBuffer(unsigned char* buffer);
};

#pragma pack(pop)

} /* namespace Packages */

class Slave {
public:
	static Slave* sendInit(int address);
	virtual ~Slave();

protected:
	Slave();
};

} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_SLAVE_SLAVE_H_ */
