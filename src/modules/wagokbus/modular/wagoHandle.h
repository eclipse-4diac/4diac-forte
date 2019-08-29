/*************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *************************************************************************/

#ifndef SRC_MODULES_WAGOKBUS_MODULAR_WAGOHANDLE_H_
#define SRC_MODULES_WAGOKBUS_MODULAR_WAGOHANDLE_H_

#include "../../../core/datatypes/forte_word.h"
#include "../../../core/io/mapper/io_handle.h"
#include "wagoDeviceController.h"

class WagoHandle : public forte::core::io::IOHandle {
  public:
    WagoHandle(WagoDeviceController *paController, CIEC_ANY::EDataTypeID paType, forte::core::io::IOMapper::Direction paDirection,
        tApplicationDeviceInterface * paAppDevInterface, uint32_t paTaskId, tDeviceId paKBusDeviceId, TForteUInt32 paOutputOffset, TForteUInt32 paInputOffset);

    virtual ~WagoHandle();

    virtual void set(const CIEC_ANY &) override;

    void get(CIEC_ANY &) override;

    bool check();

  private:

    void getBoolean(CIEC_BOOL &paState);

    void getWord(CIEC_WORD &paState);

    void setBoolean(const CIEC_BOOL &paState);

    void setWord(const CIEC_WORD &paState);

    bool checkBoolean();

    bool checkWord();

    tApplicationDeviceInterface *mAppDevInterface;
    uint32_t mTaskId;
    tDeviceId mKBusDeviceId;
    TForteUInt32 mOutputOffset;
    TForteUInt32 mInputOffset;
    CIEC_ANY* mLastValue;

    virtual void dropObserver() override;

};

#endif /* SRC_MODULES_WAGOKBUS_MODULAR_WAGOHANDLE_H_ */
