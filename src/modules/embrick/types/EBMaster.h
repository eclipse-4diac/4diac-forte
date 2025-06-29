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

#include "core/io/configFB/io_master_multi.h"
#include "core/datatypes/forte_bool.h"
#include "core/datatypes/forte_uint.h"
#include "core/datatypes/forte_udint.h"
#include "core/datatypes/forte_wstring.h"
#include "EBBusAdapter.h"

class FORTE_EBMaster final : public forte::core::io::IOConfigFBMultiMaster {
    DECLARE_FIRMWARE_FB(FORTE_EBMaster)

  private:
    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventINDID = 1;
    static const int scmBusAdapterOutAdpNum = 0;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  protected:
    forte::core::io::IODeviceController *createDeviceController(CDeviceExecution &paDeviceExecution) override;

    void setConfig() override;

    void onStartup(CEventChainExecutionThread *const paECET) override;

  public:
    FORTE_EBMaster(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_UINT var_BusInterface;
    CIEC_UINT var_BusSelectPin;
    CIEC_UDINT var_BusInitSpeed;
    CIEC_UDINT var_BusLoopSpeed;
    CIEC_UINT var_SlaveUpdateInterval;

    CIEC_BOOL var_QO;
    CIEC_WSTRING var_STATUS;

    CEventConnection conn_INITO;
    CEventConnection conn_IND;

    CDataConnection *conn_QI;
    CDataConnection *conn_BusInterface;
    CDataConnection *conn_BusSelectPin;
    CDataConnection *conn_BusInitSpeed;
    CDataConnection *conn_BusLoopSpeed;
    CDataConnection *conn_SlaveUpdateInterval;

    COutDataConnection<CIEC_BOOL> conn_QO;
    COutDataConnection<CIEC_WSTRING> conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;

    forte::CPlugPin<FORTE_EBBusAdapter_Plug> var_BusAdapterOut;

    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;
    forte::IPlugPin *getPlugPinUnchecked(size_t) override;

    void evt_INIT(const CIEC_BOOL &paQI,
                  const CIEC_UINT &paBusInterface,
                  const CIEC_UINT &paBusSelectPin,
                  const CIEC_UDINT &paBusInitSpeed,
                  const CIEC_UDINT &paBusLoopSpeed,
                  const CIEC_UINT &paSlaveUpdateInterval,
                  CIEC_BOOL &paQO,
                  CIEC_WSTRING &paSTATUS) {
      var_QI = paQI;
      var_BusInterface = paBusInterface;
      var_BusSelectPin = paBusSelectPin;
      var_BusInitSpeed = paBusInitSpeed;
      var_BusLoopSpeed = paBusLoopSpeed;
      var_SlaveUpdateInterval = paSlaveUpdateInterval;
      receiveInputEvent(scmEventINITID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI,
                    const CIEC_UINT &paBusInterface,
                    const CIEC_UINT &paBusSelectPin,
                    const CIEC_UDINT &paBusInitSpeed,
                    const CIEC_UDINT &paBusLoopSpeed,
                    const CIEC_UINT &paSlaveUpdateInterval,
                    CIEC_BOOL &paQO,
                    CIEC_WSTRING &paSTATUS) {
      evt_INIT(paQI, paBusInterface, paBusSelectPin, paBusInitSpeed, paBusLoopSpeed, paSlaveUpdateInterval, paQO,
               paSTATUS);
    }
};
