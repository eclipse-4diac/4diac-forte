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

#ifndef SRC_MODULES_EMBRICK_SLAVE_HANDLES_BIT_H_
#define SRC_MODULES_EMBRICK_SLAVE_HANDLES_BIT_H_

#include <slave/slave.h>
#include <slave/handle.h>

class EmbrickBitSlaveHandle : public EmbrickSlaveHandle {
  public:
    EmbrickBitSlaveHandle(forte::core::io::IODeviceController *paController, forte::core::io::IOMapper::Direction paDirection, uint8_t paOffset,
        uint8_t paPosition, EmbrickSlaveHandler *paSlave);

    virtual void set(const CIEC_ANY &);
    void get(CIEC_ANY &);

    bool equal(unsigned char* mOldBuffer);

  protected:
    virtual void reset() {
      CIEC_BOOL s = false;
      set(s);
    }

    const uint8_t mMask;
};

#endif /* SRC_MODULES_EMBRICK_SLAVE_HANDLES_BIT_H_ */
