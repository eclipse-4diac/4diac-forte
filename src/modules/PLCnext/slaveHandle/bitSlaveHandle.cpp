/*******************************************************************************
 * Copyright (c) 2022 Peirlberger Juergen
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Peirlberger Juergen - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "bitSlaveHandle.h"
#include <criticalregion.h>
#include "../slaveHandler.h"

PLCnextBitSlaveHandle::PLCnextBitSlaveHandle(forte::core::io::IODeviceController *paController, forte::core::io::IOMapper::Direction paDirection, uint16_t paBitPosition, PLCnextSlaveHandler *paSlave) :
    PLCnextSlaveHandle(paController, paDirection, mType = CIEC_ANY::e_BOOL, paSlave), mMask((uint16_t)(1 << paBitPosition)) {
}


void PLCnextBitSlaveHandle::set(const CIEC_ANY &paState) {
    if (mDirection == forte::core::io::IOMapper::In) {
        DEVLOG_ERROR("SET operation not allowed for type INPUT\n");
        return; 
    }

    CCriticalRegion criticalRegion(mSlave->handleMutex);

    if(static_cast<const CIEC_BOOL&>(paState)) {
        *(mSlave->imageBuffer + mOffset) = (uint16_t) (*(mSlave->imageBuffer + mOffset) | mMask);   // set

    } else {
        *(mSlave->imageBuffer + mOffset) = (uint16_t) (*(mSlave->imageBuffer + mOffset) & ~mMask);  // clear
    }
}


void PLCnextBitSlaveHandle::get(CIEC_ANY &paState) {
    if (mDirection == forte::core::io::IOMapper::Out) {
        DEVLOG_ERROR("GET operation not allowed for type OUTPUT\n");
        return; 
    }
    
    CCriticalRegion criticalRegion(mSlave->handleMutex);
    static_cast<CIEC_BOOL&>(paState) = (*(mSlave->imageBuffer + mOffset) & mMask) != 0;
}


bool PLCnextBitSlaveHandle::equal(unsigned char* paOldBuffer) {
    return (*(mSlave->imageBuffer + mOffset) & mMask) == (*(paOldBuffer + mOffset) & mMask);
}
