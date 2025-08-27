/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH
 *
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

#pragma once

#include "forte/io/mapper/io_handle.h"
#include <stdint.h>
#include <cstring>
#include <forte_sync.h>

class EmbrickSlaveHandler;

class EmbrickSlaveHandle : public forte::core::io::IOHandle {
  public:
    EmbrickSlaveHandle(forte::core::io::IODeviceController *paController,
                       forte::core::io::IOMapper::Direction paDirection,
                       CIEC_ANY::EDataTypeID type,
                       uint8_t paOffset,
                       EmbrickSlaveHandler *paSlave);
    ~EmbrickSlaveHandle() override = default;

    void set(const CIEC_ANY &) override;
    virtual bool equal(unsigned char *) = 0;

  protected:
    virtual void reset() {
    }

    void onObserver(forte::core::io::IOObserver *paObserver) override;
    void dropObserver() override;

    unsigned char *mBuffer;
    const uint8_t mOffset;
    EmbrickSlaveHandler *mSlave;
    CSyncObject *mUpdateMutex;
};
