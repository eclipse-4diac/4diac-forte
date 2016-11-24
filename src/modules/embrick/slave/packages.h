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

#ifndef SRC_MODULES_EMBRICK_SLAVE_PACKAGES_H_
#define SRC_MODULES_EMBRICK_SLAVE_PACKAGES_H_

#include <netinet/in.h>

namespace EmBrick {

namespace Packages {

#pragma pack(push, 1) // Disable padding for protocol structs

struct Header {
	char address;
	char command;
	char checksum;
};

struct SlaveInit {
	uint8_t protocolVersion;
	uint8_t moduleVersion;
	uint16_t deviceId;
	uint16_t producerId;
	uint8_t dataSendLength; // Amount of bytes that the slave expects from the master
	uint8_t dataReceiveLength; // Amount of bytes that the master expects from the slave

	static SlaveInit fromBuffer(unsigned char* buffer) {
		SlaveInit pkg;
		memcpy(&pkg, buffer, sizeof(SlaveInit));

		// pkg.deviceId = ntohs(pkg.deviceId);
		pkg.producerId = ntohs(pkg.producerId);

		return pkg;
	}
};

struct MasterInit {
	uint8_t slaveAddress;
	uint16_t syncGapMultiplicator;

	void toBuffer(unsigned char* buffer) {
		buffer[0] = slaveAddress;

		uint16_t syncGapFactor = htons(this->syncGapMultiplicator);
		memcpy(buffer + 1, &syncGapFactor, 2);
	}
};

#pragma pack(pop)

} /* namespace Packages */

}

#endif /* SRC_MODULES_EMBRICK_SLAVE_PACKAGES_H_ */
