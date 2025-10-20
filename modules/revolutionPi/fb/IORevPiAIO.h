/*******************************************************************************
 * Copyright (c) 2018, 2024 fortiss GmbH, Monika Wenger, Johannes Kepler University Linz
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v20.html
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *   Monika Wenger, Alois Zoitl - migration to current runtime
 *******************************************************************************/

#pragma once

#include "forte/io/configFB/io_slave_multi.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"
#include "IORevPiBusAdapter.h"

namespace forte::eclipse4diac::io::revpi {

  class FORTE_IORevPiAIO : public ::forte::io::IOConfigFBMultiSlave {
      DECLARE_FIRMWARE_FB(FORTE_IORevPiAIO)

    private:
      static const TEventID scmEventMAPID = 0;
      static const TEventID scmEventMAPOID = 0;
      static const TEventID scmEventINDID = 1;
      static const int scmBusAdapterInAdpNum = 0;
      static const int scmBusAdapterOutAdpNum = 1;

      static const TForteUInt8 scmSlaveConfigurationIO[];
      static const TForteUInt8 scmSlaveConfigurationIONum;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_IORevPiAIO(const forte::StringId paInstanceNameId, CFBContainer &paContainer);

      virtual ~FORTE_IORevPiAIO() {};

      CIEC_BOOL var_QI;
      CIEC_STRING var_AnalogInput_1;
      CIEC_STRING var_AnalogInput_2;
      CIEC_STRING var_AnalogInput_3;
      CIEC_STRING var_AnalogInput_4;
      CIEC_STRING var_RTD_1;
      CIEC_STRING var_RTD_2;
      CIEC_STRING var_AnalogOutput_1;
      CIEC_STRING var_AnalogOutput_2;

      CIEC_BOOL var_QO;
      CIEC_WSTRING var_STATUS;

      CEventConnection conn_MAPO;
      CEventConnection conn_IND;

      CDataConnection *conn_QI;
      CDataConnection *conn_AnalogInput_1;
      CDataConnection *conn_AnalogInput_2;
      CDataConnection *conn_AnalogInput_3;
      CDataConnection *conn_AnalogInput_4;
      CDataConnection *conn_RTD_1;
      CDataConnection *conn_RTD_2;
      CDataConnection *conn_AnalogOutput_1;
      CDataConnection *conn_AnalogOutput_2;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_WSTRING> conn_STATUS;

      forte::CPlugPin<FORTE_IORevPiBusAdapter_Plug> var_BusAdapterOut;
      forte::CSocketPin<FORTE_IORevPiBusAdapter_Socket> var_BusAdapterIn;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;

      forte::IPlugPin *getPlugPinUnchecked(size_t) override;
      forte::ISocketPin *getSocketPinUnchecked(size_t) override;

      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_MAP(const CIEC_BOOL &paQI,
                   const CIEC_STRING &paAnalogInput_1,
                   const CIEC_STRING &paAnalogInput_2,
                   const CIEC_STRING &paAnalogInput_3,
                   const CIEC_STRING &paAnalogInput_4,
                   const CIEC_STRING &paRTD_1,
                   const CIEC_STRING &paRTD_2,
                   const CIEC_STRING &paAnalogOutput_1,
                   const CIEC_STRING &paAnalogOutput_2,
                   CAnyBitOutputParameter<CIEC_BOOL> paQO,
                   COutputParameter<CIEC_WSTRING> paSTATUS) {
        COutputGuard guard_paQO(paQO);
        COutputGuard guard_paSTATUS(paSTATUS);
        var_QI = paQI;
        var_AnalogInput_1 = paAnalogInput_1;
        var_AnalogInput_2 = paAnalogInput_2;
        var_AnalogInput_3 = paAnalogInput_3;
        var_AnalogInput_4 = paAnalogInput_4;
        var_RTD_1 = paRTD_1;
        var_RTD_2 = paRTD_2;
        var_AnalogOutput_1 = paAnalogOutput_1;
        var_AnalogOutput_2 = paAnalogOutput_2;
        executeEvent(scmEventMAPID, nullptr);
        *paQO = var_QO;
        *paSTATUS = var_STATUS;
      }

      void operator()(const CIEC_BOOL &paQI,
                      const CIEC_STRING &paAnalogInput_1,
                      const CIEC_STRING &paAnalogInput_2,
                      const CIEC_STRING &paAnalogInput_3,
                      const CIEC_STRING &paAnalogInput_4,
                      const CIEC_STRING &paRTD_1,
                      const CIEC_STRING &paRTD_2,
                      const CIEC_STRING &paAnalogOutput_1,
                      const CIEC_STRING &paAnalogOutput_2,
                      CAnyBitOutputParameter<CIEC_BOOL> paQO,
                      COutputParameter<CIEC_WSTRING> paSTATUS) {
        evt_MAP(paQI, paAnalogInput_1, paAnalogInput_2, paAnalogInput_3, paAnalogInput_4, paRTD_1, paRTD_2,
                paAnalogOutput_1, paAnalogOutput_2, paQO, paSTATUS);
      }

    protected:
      void initHandles() override;
  };
} // namespace forte::eclipse4diac::io::revpi
