/*************************************************************************
 *** Copyright (c) 2022 Peirlberger Juergen
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: PLCnextMaster
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2022-04-07/Peirlberger Juergen -  - initial API and implementation and/or initial documentation
 *************************************************************************/

#pragma once

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/io/configFB/io_master_multi.h"
#include "PLCnextBusAdapter.h"

class FORTE_PLCnextMaster final : public forte::io::IOConfigFBMultiMaster {
    DECLARE_FIRMWARE_FB(FORTE_PLCnextMaster)

  private:
    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventINDID = 1;
    static const int scmBusAdapterOutAdpNum = 0;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

    forte::io::IODeviceController *createDeviceController(CDeviceExecution &paDeviceExecution) override;
    void setConfig() override;

  public:
    FORTE_PLCnextMaster(forte::StringId paInstanceNameId, CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_UINT var_SlaveUpdateInterval;

    CIEC_BOOL var_QO;
    CIEC_WSTRING var_STATUS;

    forte::CPlugPin<FORTE_PLCnextBusAdapter_Plug> var_BusAdapterOut;

    CEventConnection conn_INITO;
    CEventConnection conn_IND;

    CDataConnection *conn_QI;
    CDataConnection *conn_SlaveUpdateInterval;

    COutDataConnection<CIEC_BOOL> conn_QO;
    COutDataConnection<CIEC_WSTRING> conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;

    forte::IPlugPin *getPlugPinUnchecked(size_t) override;

    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI,
                  const CIEC_UINT &paSlaveUpdateInterval,
                  CAnyBitOutputParameter<CIEC_BOOL> paQO,
                  COutputParameter<CIEC_WSTRING> paSTATUS) {
      COutputGuard guard_paQO(paQO);
      COutputGuard guard_paSTATUS(paSTATUS);
      var_QI = paQI;
      var_SlaveUpdateInterval = paSlaveUpdateInterval;
      receiveInputEvent(scmEventINITID, nullptr);
      *paQO = var_QO;
      *paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI,
                    const CIEC_UINT &paSlaveUpdateInterval,
                    CAnyBitOutputParameter<CIEC_BOOL> paQO,
                    COutputParameter<CIEC_WSTRING> paSTATUS) {
      evt_INIT(paQI, paSlaveUpdateInterval, paQO, paSTATUS);
    }
};
