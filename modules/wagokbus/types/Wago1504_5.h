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

namespace forte::eclipse4diac::io::wago {

  class FORTE_Wago1504_5 : public WagoSlaveBase {
      DECLARE_FIRMWARE_FB(FORTE_Wago1504_5)

    public:
      FORTE_Wago1504_5(const forte::StringId paInstanceNameId, CFBContainer &paContainer);
      ~FORTE_Wago1504_5() override = default;

      CIEC_BOOL var_QI;
      CIEC_STRING var_DigitalOutput_1;
      CIEC_STRING var_DigitalOutput_2;
      CIEC_STRING var_DigitalOutput_3;
      CIEC_STRING var_DigitalOutput_4;
      CIEC_STRING var_DigitalOutput_5;
      CIEC_STRING var_DigitalOutput_6;
      CIEC_STRING var_DigitalOutput_7;
      CIEC_STRING var_DigitalOutput_8;
      CIEC_STRING var_DigitalOutput_9;
      CIEC_STRING var_DigitalOutput_10;
      CIEC_STRING var_DigitalOutput_11;
      CIEC_STRING var_DigitalOutput_12;
      CIEC_STRING var_DigitalOutput_13;
      CIEC_STRING var_DigitalOutput_14;
      CIEC_STRING var_DigitalOutput_15;
      CIEC_STRING var_DigitalOutput_16;

      CIEC_BOOL var_QO;
      CIEC_WSTRING var_STATUS;

      CEventConnection conn_MAPO;
      CEventConnection conn_IND;

      CDataConnection *conn_QI;
      CDataConnection *conn_DigitalOutput_1;
      CDataConnection *conn_DigitalOutput_2;
      CDataConnection *conn_DigitalOutput_3;
      CDataConnection *conn_DigitalOutput_4;
      CDataConnection *conn_DigitalOutput_5;
      CDataConnection *conn_DigitalOutput_6;
      CDataConnection *conn_DigitalOutput_7;
      CDataConnection *conn_DigitalOutput_8;
      CDataConnection *conn_DigitalOutput_9;
      CDataConnection *conn_DigitalOutput_10;
      CDataConnection *conn_DigitalOutput_11;
      CDataConnection *conn_DigitalOutput_12;
      CDataConnection *conn_DigitalOutput_13;
      CDataConnection *conn_DigitalOutput_14;
      CDataConnection *conn_DigitalOutput_15;
      CDataConnection *conn_DigitalOutput_16;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_WSTRING> conn_STATUS;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;

      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

    protected:
      INIT_HANDLES(0, 16, 0, 0)

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

} // namespace forte::eclipse4diac::io::wago
