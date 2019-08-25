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

#ifndef SRC_MODULES_EMBRICK_SLAVE_HANDLE_H_
#define SRC_MODULES_EMBRICK_SLAVE_HANDLE_H_

#include <stdint.h>
#include <cstring>
#include <forte_sync.h>
#include <forte_bool.h>
#include <forte_dword.h>

#include <io/mapper/io_handle.h>

class EmbrickSlaveHandler;

class EmbrickSlaveHandle : public forte::core::io::IOHandle {
  public:
    EmbrickSlaveHandle(forte::core::io::IODeviceController *paController, forte::core::io::IOMapper::Direction paDirection, CIEC_ANY::EDataTypeID type,
        uint8_t paOffset, EmbrickSlaveHandler *paSlave);
    virtual ~EmbrickSlaveHandle();

    virtual void set(const CIEC_ANY &);
    virtual bool equal(unsigned char*) = 0;

  protected:
    virtual void reset() {

    }

    virtual void onObserver(forte::core::io::IOObserver *paObserver);
    virtual void dropObserver();

    unsigned char* mBuffer;
    const uint8_t mOffset;
    EmbrickSlaveHandler* mSlave;
    CSyncObject *mUpdateMutex;
};

#endif /* SRC_MODULES_EMBRICK_SLAVE_HANDLE_H_ */
