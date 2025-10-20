/*************************************************************************
 * Copyright (c) 2018, 2025 fortiss GmbH, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#pragma once

#include "WagoBusAdapter.h"
#include "forte/io/configFB/io_master_multi.h"

class FORTE_WagoMaster : public forte::io::IOConfigFBMultiMaster {
    DECLARE_FIRMWARE_FB(FORTE_WagoMaster)

  public:
    FORTE_WagoMaster(forte::StringId paInstanceNameId, CFBContainer &paContainer);
    ~FORTE_WagoMaster() override = default;

    CIEC_BOOL var_QI;
    CIEC_UINT var_UpdateInterval;

    CIEC_BOOL var_QO;
    CIEC_WSTRING var_STATUS;

    CEventConnection conn_INITO;
    CEventConnection conn_IND;

    CDataConnection *conn_QI;
    CDataConnection *conn_UpdateInterval;

    COutDataConnection<CIEC_BOOL> conn_QO;
    COutDataConnection<CIEC_WSTRING> conn_STATUS;

    forte::CPlugPin<FORTE_WagoBusAdapter_Plug> var_BusAdapterOut;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;

    forte::IPlugPin *getPlugPinUnchecked(size_t) override;

    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI,
                  const CIEC_UINT &paUpdateInterval,
                  CAnyBitOutputParameter<CIEC_BOOL> paQO,
                  COutputParameter<CIEC_WSTRING> paSTATUS) {
      COutputGuard guard_paQO(paQO);
      COutputGuard guard_paSTATUS(paSTATUS);
      var_QI = paQI;
      var_UpdateInterval = paUpdateInterval;
      executeEvent(scmEventINITID, nullptr);
      *paQO = var_QO;
      *paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI,
                    const CIEC_UINT &paUpdateInterval,
                    CAnyBitOutputParameter<CIEC_BOOL> paQO,
                    COutputParameter<CIEC_WSTRING> paSTATUS) {
      evt_INIT(paQI, paUpdateInterval, paQO, paSTATUS);
    }

  private:
    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventINDID = 1;
    static const int scmBusAdapterOutAdpNum = 0;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

    forte::io::IODeviceController *createDeviceController(CDeviceExecution &paDeviceExecution) override;
    void setConfig() override;
};
