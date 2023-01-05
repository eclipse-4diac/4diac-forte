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

#ifndef SRC_MODULES_PLCNEXT_SLAVEHANDLE_H_
#define SRC_MODULES_PLCNEXT_SLAVEHANDLE_H_

#include <stdint.h>
#include <cstring>
#include <forte_sync.h>
#include <forte_bool.h>
#include <forte_dword.h>
#include "../../../core/io/mapper/io_handle.h"
#include "../deviceController.h"

class PLCnextSlaveHandler;

class PLCnextSlaveHandle : public forte::core::io::IOHandle {
  public:
    PLCnextSlaveHandle(forte::core::io::IODeviceController* paController, forte::core::io::IOMapper::Direction paDirection, CIEC_ANY::EDataTypeID type, PLCnextSlaveHandler* paSlave);
    ~PLCnextSlaveHandle();

    virtual void set(const CIEC_ANY&) = 0;
    virtual void get(CIEC_ANY&) = 0;

  protected:
    virtual void reset() {}

    virtual void onObserver(forte::core::io::IOObserver* paObserver);
    virtual void dropObserver();

    const uint8_t mOffset;

    PLCnextSlaveHandler *mSlave;
};

#endif /* SRC_MODULES_PLCNEXT_SLAVEHANDLE_H_ */
