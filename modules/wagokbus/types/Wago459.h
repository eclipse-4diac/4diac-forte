/*************************************************************************
 * Copyright (c) 2016, 2025 fortiss GmbH, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#pragma once

#include "WagoSlaveBase.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"

class FORTE_Wago459 : public WagoSlaveBase {
    DECLARE_FIRMWARE_FB(FORTE_Wago459)

  public:
    FORTE_Wago459(const forte::StringId paInstanceNameId, CFBContainer &paContainer);
    ~FORTE_Wago459() override = default;

    CIEC_BOOL var_QI;
    CIEC_STRING var_AnalogInput_1;
    CIEC_STRING var_AnalogInput_2;
    CIEC_STRING var_AnalogInput_3;
    CIEC_STRING var_AnalogInput_4;

    CIEC_BOOL var_QO;
    CIEC_WSTRING var_STATUS;

    CEventConnection conn_MAPO;
    CEventConnection conn_IND;

    CDataConnection *conn_QI;
    CDataConnection *conn_AnalogInput_1;
    CDataConnection *conn_AnalogInput_2;
    CDataConnection *conn_AnalogInput_3;
    CDataConnection *conn_AnalogInput_4;

    COutDataConnection<CIEC_BOOL> conn_QO;
    COutDataConnection<CIEC_WSTRING> conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;

    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_MAP(const CIEC_BOOL &paQI,
                 const CIEC_STRING &paAnalogInput_1,
                 const CIEC_STRING &paAnalogInput_2,
                 const CIEC_STRING &paAnalogInput_3,
                 const CIEC_STRING &paAnalogInput_4,
                 CAnyBitOutputParameter<CIEC_BOOL> paQO,
                 COutputParameter<CIEC_WSTRING> paSTATUS) {
      COutputGuard guard_paQO(paQO);
      COutputGuard guard_paSTATUS(paSTATUS);
      var_QI = paQI;
      var_AnalogInput_1 = paAnalogInput_1;
      var_AnalogInput_2 = paAnalogInput_2;
      var_AnalogInput_3 = paAnalogInput_3;
      var_AnalogInput_4 = paAnalogInput_4;
      executeEvent(scmEventMAPID, nullptr);
      *paQO = var_QO;
      *paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI,
                    const CIEC_STRING &paAnalogInput_1,
                    const CIEC_STRING &paAnalogInput_2,
                    const CIEC_STRING &paAnalogInput_3,
                    const CIEC_STRING &paAnalogInput_4,
                    CAnyBitOutputParameter<CIEC_BOOL> paQO,
                    COutputParameter<CIEC_WSTRING> paSTATUS) {
      evt_MAP(paQI, paAnalogInput_1, paAnalogInput_2, paAnalogInput_3, paAnalogInput_4, paQO, paSTATUS);
    }

  protected:
    INIT_HANDLES(0, 0, 4, 0)

  private:
    static const TEventID scmEventMAPID = 0;
    static const TEventID scmEventMAPOID = 0;
    static const TEventID scmEventINDID = 1;
    static const int scmBusAdapterInAdpNum = 0;
    static const int scmBusAdapterOutAdpNum = 1;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;
};
