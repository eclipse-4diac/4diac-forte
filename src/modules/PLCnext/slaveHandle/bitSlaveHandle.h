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

#ifndef SRC_MODULES_PLCNEXT_SLAVEHANDLES_BIT_H_
#define SRC_MODULES_PLCNEXT_SLAVEHANDLES_BIT_H_

#include <slaveHandle/slaveHandle.h>
#include "../slaveHandler.h"

class PLCnextBitSlaveHandle : public PLCnextSlaveHandle {
  public:
    PLCnextBitSlaveHandle(forte::core::io::IODeviceController *paController, forte::core::io::IOMapper::Direction paDirection, uint16_t paPosition, PLCnextSlaveHandler *paSlave);

    void set(const CIEC_ANY&) override;
    void get(CIEC_ANY&) override;

    bool equal(unsigned char* mOldBuffer);

  protected:
    virtual void reset() {
      set((CIEC_BOOL)false);
    }

    const uint16_t mMask;
};

#endif /* SRC_MODULES_PLCNEXT_SLAVEHANDLES_BIT_H_ */
