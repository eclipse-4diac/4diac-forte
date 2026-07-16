/*******************************************************************************
 * Copyright (c) 2026 Sichuan Qunyuan Technology Co., Ltd.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Zijun Tang - initial API and implementation
 *******************************************************************************/

#pragma once

#include "forte/io/configFB/io_master_multi.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/datatypes/forte_time.h"
#include "ECBusAdapter.h"

namespace forte::eclipse4diac::io::ethercat {

  class FORTE_ECController final : public forte::io::IOConfigFBMultiMaster {
      DECLARE_FIRMWARE_FB(FORTE_ECController)
    
    private:
      static const TEventID scmEventINITID = 0;
      static const TEventID scmEventREQID = 1;
      static const TEventID scmEventINITO = 0;
      static const TEventID scmEventCNF = 1;
      static const TEventID scmEventIND = 2;
      static const int scmBusAdapterOutAdpNum = 0;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    protected:
      forte::io::IODeviceController *createDeviceController(CDeviceExecution &paDeviceExecution) override;

      void setConfig() override;

      void onStartup(CEventChainExecutionThread *const paECET) override;
      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    public:
      FORTE_ECController(forte::StringId paInstanceNameId, CFBContainer &paContainer);
      ~FORTE_ECController() override;

      EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;

      CIEC_BOOL var_QI;
      CIEC_BOOL var_Enable;
      CIEC_UINT var_ControllerId;
      CIEC_TIME var_UpdateInterval;

      CIEC_BOOL var_QO;
      CIEC_WSTRING var_STATUS;

      CEventConnection conn_INITO;
      CEventConnection conn_IND;
      CEventConnection conn_CNF;

      CDataConnection *conn_QI;
      CDataConnection *conn_Enable;
      CDataConnection *conn_ControllerId;
      CDataConnection *conn_UpdateInterval;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_WSTRING> conn_STATUS;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;

      forte::CPlugPin<FORTE_ECBusAdapter_Plug> var_BusAdapterOut;

      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
      forte::IPlugPin *getPlugPinUnchecked(size_t) override;

      void evt_INIT(const CIEC_BOOL &paQI,
                    const CIEC_BOOL &paEnable,
                    const CIEC_UINT &paControllerId,
                    const CIEC_TIME &paUpdateInterval,
                    CAnyBitOutputParameter<CIEC_BOOL> paQO,
                    COutputParameter<CIEC_WSTRING> paSTATUS) {
        COutputGuard guard_paQO(paQO);
        COutputGuard guard_paSTATUS(paSTATUS);
        var_QI = paQI;
        var_Enable = paEnable;
        var_ControllerId = paControllerId;
        var_UpdateInterval = paUpdateInterval;
        receiveInputEvent(scmEventINITID, nullptr);
        *paQO = var_QO;
        *paSTATUS = var_STATUS;
      }

      void operator()(const CIEC_BOOL &paQI,
                      const CIEC_BOOL &paEnable,
                      const CIEC_UINT &paControllerId,
                      const CIEC_TIME &paUpdateInterval,
                      CAnyBitOutputParameter<CIEC_BOOL> paQO,
                      COutputParameter<CIEC_WSTRING> paSTATUS) {
        evt_INIT(paQI, paEnable, paControllerId, paUpdateInterval, paQO, paSTATUS);
      }
  };
}