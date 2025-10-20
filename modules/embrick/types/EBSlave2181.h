/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH
 *
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

#pragma once

#include "Slave.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/datatypes/forte_uint.h"

namespace forte::eclipse4diac::io::embrick {

  class FORTE_EBSlave2181 final : public EmbrickSlave {
      DECLARE_FIRMWARE_FB(FORTE_EBSlave2181)

    private:
      static const TEventID scmEventMAPID = 0;
      static const TEventID scmEventMAPOID = 0;
      static const TEventID scmEventINDID = 1;
      static const int scmBusAdapterInAdpNum = 0;
      static const int scmBusAdapterOutAdpNum = 1;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

      static const TForteUInt8 scmSlaveConfigurationIO[];
      static const TForteUInt8 scmSlaveConfigurationIONum;

      void initHandles() override;

    public:
      FORTE_EBSlave2181(forte::StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_WSTRING var_DigitalInput_1;
      CIEC_WSTRING var_DigitalInput_2;
      CIEC_WSTRING var_DigitalInput_3;
      CIEC_WSTRING var_DigitalInput_4;
      CIEC_WSTRING var_DigitalInput_5;
      CIEC_WSTRING var_DigitalInput_6;
      CIEC_WSTRING var_DigitalInput_7;
      CIEC_WSTRING var_DigitalInput_8;
      CIEC_WSTRING var_DigitalOutput_1;
      CIEC_WSTRING var_DigitalOutput_2;
      CIEC_WSTRING var_DigitalOutput_3;
      CIEC_WSTRING var_DigitalOutput_4;
      CIEC_WSTRING var_DigitalOutput_5;
      CIEC_WSTRING var_DigitalOutput_6;
      CIEC_WSTRING var_DigitalOutput_7;
      CIEC_WSTRING var_DigitalOutput_8;
      CIEC_UINT var_UpdateInterval;

      CIEC_BOOL var_QO;
      CIEC_WSTRING var_STATUS;

      CEventConnection conn_MAPO;
      CEventConnection conn_IND;

      CDataConnection *conn_QI;
      CDataConnection *conn_DigitalInput_1;
      CDataConnection *conn_DigitalInput_2;
      CDataConnection *conn_DigitalInput_3;
      CDataConnection *conn_DigitalInput_4;
      CDataConnection *conn_DigitalInput_5;
      CDataConnection *conn_DigitalInput_6;
      CDataConnection *conn_DigitalInput_7;
      CDataConnection *conn_DigitalInput_8;
      CDataConnection *conn_DigitalOutput_1;
      CDataConnection *conn_DigitalOutput_2;
      CDataConnection *conn_DigitalOutput_3;
      CDataConnection *conn_DigitalOutput_4;
      CDataConnection *conn_DigitalOutput_5;
      CDataConnection *conn_DigitalOutput_6;
      CDataConnection *conn_DigitalOutput_7;
      CDataConnection *conn_DigitalOutput_8;
      CDataConnection *conn_UpdateInterval;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_WSTRING> conn_STATUS;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;

      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_MAP(const CIEC_BOOL &paQI,
                   const CIEC_WSTRING &paDigitalInput_1,
                   const CIEC_WSTRING &paDigitalInput_2,
                   const CIEC_WSTRING &paDigitalInput_3,
                   const CIEC_WSTRING &paDigitalInput_4,
                   const CIEC_WSTRING &paDigitalInput_5,
                   const CIEC_WSTRING &paDigitalInput_6,
                   const CIEC_WSTRING &paDigitalInput_7,
                   const CIEC_WSTRING &paDigitalInput_8,
                   const CIEC_WSTRING &paDigitalOutput_1,
                   const CIEC_WSTRING &paDigitalOutput_2,
                   const CIEC_WSTRING &paDigitalOutput_3,
                   const CIEC_WSTRING &paDigitalOutput_4,
                   const CIEC_WSTRING &paDigitalOutput_5,
                   const CIEC_WSTRING &paDigitalOutput_6,
                   const CIEC_WSTRING &paDigitalOutput_7,
                   const CIEC_WSTRING &paDigitalOutput_8,
                   const CIEC_UINT &paUpdateInterval,
                   CAnyBitOutputParameter<CIEC_BOOL> paQO,
                   COutputParameter<CIEC_WSTRING> paSTATUS) {
        COutputGuard guard_paQO(paQO);
        COutputGuard guard_paSTATUS(paSTATUS);
        var_QI = paQI;
        var_DigitalInput_1 = paDigitalInput_1;
        var_DigitalInput_2 = paDigitalInput_2;
        var_DigitalInput_3 = paDigitalInput_3;
        var_DigitalInput_4 = paDigitalInput_4;
        var_DigitalInput_5 = paDigitalInput_5;
        var_DigitalInput_6 = paDigitalInput_6;
        var_DigitalInput_7 = paDigitalInput_7;
        var_DigitalInput_8 = paDigitalInput_8;
        var_DigitalOutput_1 = paDigitalOutput_1;
        var_DigitalOutput_2 = paDigitalOutput_2;
        var_DigitalOutput_3 = paDigitalOutput_3;
        var_DigitalOutput_4 = paDigitalOutput_4;
        var_DigitalOutput_5 = paDigitalOutput_5;
        var_DigitalOutput_6 = paDigitalOutput_6;
        var_DigitalOutput_7 = paDigitalOutput_7;
        var_DigitalOutput_8 = paDigitalOutput_8;
        var_UpdateInterval = paUpdateInterval;
        receiveInputEvent(scmEventMAPID, nullptr);
        *paQO = var_QO;
        *paSTATUS = var_STATUS;
      }

      void operator()(const CIEC_BOOL &paQI,
                      const CIEC_WSTRING &paDigitalInput_1,
                      const CIEC_WSTRING &paDigitalInput_2,
                      const CIEC_WSTRING &paDigitalInput_3,
                      const CIEC_WSTRING &paDigitalInput_4,
                      const CIEC_WSTRING &paDigitalInput_5,
                      const CIEC_WSTRING &paDigitalInput_6,
                      const CIEC_WSTRING &paDigitalInput_7,
                      const CIEC_WSTRING &paDigitalInput_8,
                      const CIEC_WSTRING &paDigitalOutput_1,
                      const CIEC_WSTRING &paDigitalOutput_2,
                      const CIEC_WSTRING &paDigitalOutput_3,
                      const CIEC_WSTRING &paDigitalOutput_4,
                      const CIEC_WSTRING &paDigitalOutput_5,
                      const CIEC_WSTRING &paDigitalOutput_6,
                      const CIEC_WSTRING &paDigitalOutput_7,
                      const CIEC_WSTRING &paDigitalOutput_8,
                      const CIEC_UINT &paUpdateInterval,
                      CAnyBitOutputParameter<CIEC_BOOL> paQO,
                      COutputParameter<CIEC_WSTRING> paSTATUS) {
        evt_MAP(paQI, paDigitalInput_1, paDigitalInput_2, paDigitalInput_3, paDigitalInput_4, paDigitalInput_5,
                paDigitalInput_6, paDigitalInput_7, paDigitalInput_8, paDigitalOutput_1, paDigitalOutput_2,
                paDigitalOutput_3, paDigitalOutput_4, paDigitalOutput_5, paDigitalOutput_6, paDigitalOutput_7,
                paDigitalOutput_8, paUpdateInterval, paQO, paSTATUS);
      }
  };

} // namespace forte::eclipse4diac::io::embrick
