/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#ifndef SRC_MODULES_EMBRICK_SLAVE_PACKAGES_H_
#define SRC_MODULES_EMBRICK_SLAVE_PACKAGES_H_

#include <netinet/in.h>

#pragma pack(push, 1) // Disable padding for protocol structs

struct EmbrickHeaderPackage {
  char address;
  char command;
  char checksum;
};

struct EmbrickSlaveInitPackage {
  uint8_t protocolVersion;
  uint8_t moduleVersion;
  uint16_t deviceId;
  uint16_t producerId;
  uint8_t dataSendLength; // Amount of bytes that the slave expects from the master
  uint8_t dataReceiveLength; // Amount of bytes that the master expects from the slave

  static EmbrickSlaveInitPackage fromBuffer(unsigned char* buffer) {
    EmbrickSlaveInitPackage pkg;
    memcpy(&pkg, buffer, sizeof(EmbrickSlaveInitPackage));

    pkg.deviceId = ntohs(pkg.deviceId);
    // Switch bytes of deviceId as it is transmitted with a different endianess
    pkg.deviceId = (uint16_t) (((pkg.deviceId & 0xFF00) >> 8)
        | ((pkg.deviceId & 0xFF) << 8));

    pkg.producerId = ntohs(pkg.producerId);

    return pkg;
  }
};

struct EmbrickMasterInitPackage {
  uint8_t slaveAddress;
  uint16_t syncGapMultiplicator;

  void toBuffer(unsigned char* buffer) {
    buffer[0] = slaveAddress;

    uint16_t syncGapFactor = htons(this->syncGapMultiplicator);
    memcpy(buffer + 1, &syncGapFactor, 2);
  }
};

#pragma pack(pop)

#endif /* SRC_MODULES_EMBRICK_SLAVE_PACKAGES_H_ */
