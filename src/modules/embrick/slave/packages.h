/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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
    char mAddress;
    char mCommand;
    char mChecksum;
};

struct EmbrickSlaveInitPackage {
    uint8_t mProtocolVersion;
    uint8_t mModuleVersion;
    uint16_t mDeviceId;
    uint16_t mProducerId;
    uint8_t mDataSendLength; // Amount of bytes that the slave expects from the master
    uint8_t mDataReceiveLength; // Amount of bytes that the master expects from the slave

    static EmbrickSlaveInitPackage fromBuffer(unsigned char* paBuffer) {
      EmbrickSlaveInitPackage pkg;
      memcpy(&pkg, paBuffer, sizeof(EmbrickSlaveInitPackage));

      pkg.mDeviceId = ntohs(pkg.mDeviceId);
      // Switch bytes of deviceId as it is transmitted with a different endianess
      pkg.mDeviceId = (uint16_t) (((pkg.mDeviceId & 0xFF00) >> 8) | ((pkg.mDeviceId & 0xFF) << 8));

      pkg.mProducerId = ntohs(pkg.mProducerId);

      return pkg;
    }
};

struct EmbrickMasterInitPackage {
    uint8_t mSlaveAddress;
    uint16_t mSyncGapMultiplicator;

    void toBuffer(unsigned char* paBuffer) {
      paBuffer[0] = mSlaveAddress;

      uint16_t syncGapFactor = htons(this->mSyncGapMultiplicator);
      memcpy(paBuffer + 1, &syncGapFactor, 2);
    }
};

#pragma pack(pop)

#endif /* SRC_MODULES_EMBRICK_SLAVE_PACKAGES_H_ */
