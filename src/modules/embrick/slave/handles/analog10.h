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

#ifndef SRC_MODULES_EMBRICK_SLAVE_HANDLES_ANALOG10_H_
#define SRC_MODULES_EMBRICK_SLAVE_HANDLES_ANALOG10_H_

#include <slave/slave.h>
#include <slave/handle.h>

class EmbrickAnalog10SlaveHandle : public EmbrickSlaveHandle {
  public:
    EmbrickAnalog10SlaveHandle(forte::core::io::IODeviceController *paController, forte::core::io::IOMapper::Direction paDirection, uint8_t paOffset,
        EmbrickSlaveHandler *paSlave);

    virtual void set(const CIEC_ANY &);
    void get(CIEC_ANY &);

    bool equal(unsigned char* paOldBuffer);

  protected:
    const CIEC_DWORD getValue(const unsigned char* paBuffer);
};

#endif /* SRC_MODULES_EMBRICK_SLAVE_HANDLES_ANALOG10_H_ */
