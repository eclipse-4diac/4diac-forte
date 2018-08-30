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

#ifndef SRC_MODULES_EMBRICK_SLAVE_HANDLES_ANALOG_H_
#define SRC_MODULES_EMBRICK_SLAVE_HANDLES_ANALOG_H_

#include <slave/slave.h>
#include <slave/handle.h>

class EmbrickAnalogSlaveHandle : public EmbrickSlaveHandle {
  public:
    EmbrickAnalogSlaveHandle(forte::core::io::IODeviceController *paController, forte::core::io::IOMapper::Direction paDirection, uint8_t paOffset,
        EmbrickSlaveHandler *paSlave);

    virtual void set(const CIEC_ANY &);
    void get(CIEC_ANY &);

    bool equal(unsigned char* paOldBuffer);

  protected:
    const CIEC_DWORD getValue(const unsigned char* paBuffer);
};

#endif /* SRC_MODULES_EMBRICK_SLAVE_HANDLES_ANALOG_H_ */
