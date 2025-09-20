/*************************************************************************
 * Copyright (c) 2018, 2025 fortiss GmbH, Monika Wenger
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

#include "forte/datatypes/forte_word.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/io/mapper/io_handle.h"
#include "WagoDeviceController.h"

class WagoHandle : public forte::io::IOHandle {
  public:
    WagoHandle(WagoDeviceController *paController,
               CIEC_ANY::EDataTypeID paType,
               forte::io::IOMapper::Direction paDirection,
               tApplicationDeviceInterface *paAppDevInterface,
               uint32_t paTaskId,
               tDeviceId paKBusDeviceId,
               TForteUInt32 paOutputOffset,
               TForteUInt32 paInputOffset);

    ~WagoHandle() override;

    virtual void set(const CIEC_ANY &) override;

    void get(CIEC_ANY &) override;

    bool check();

  private:
    void getBoolean(CIEC_BOOL &paState);
    void setBoolean(const CIEC_BOOL &paState);

    void getWord(CIEC_WORD &paState);
    void setWord(const CIEC_WORD &paState);

    void getDWord(CIEC_DWORD &paState);
    void setDWord(const CIEC_DWORD &paState);

    template<typename T>
    bool checkValue();

    tApplicationDeviceInterface *mAppDevInterface;
    uint32_t mTaskId;
    tDeviceId mKBusDeviceId;
    TForteUInt32 mOutputOffset;
    TForteUInt32 mInputOffset;
    CIEC_ANY *mLastValue;

    virtual void dropObserver() override;
};

#endif /* SRC_MODULES_WAGOKBUS_MODULAR_WAGOHANDLE_H_ */
