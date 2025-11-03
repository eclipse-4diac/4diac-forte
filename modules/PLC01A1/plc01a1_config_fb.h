/*******************************************************************************
 * Copyright (c) 2019, 2025 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/io/configFB/io_configFB_controller.h"

namespace forte::eclipse4diac::io::plc01a1 {

  class FORTE_PLC01A1 : public forte::io::IOConfigFBController {
      DECLARE_FIRMWARE_FB(FORTE_PLC01A1)

    private:
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventINDID = 1;
      static const TEventID scmEventINITID = 0;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_PLC01A1(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_WSTRING var_IN1;
      CIEC_WSTRING var_IN2;
      CIEC_WSTRING var_IN3;
      CIEC_WSTRING var_IN4;
      CIEC_WSTRING var_IN5;
      CIEC_WSTRING var_IN6;
      CIEC_WSTRING var_IN7;
      CIEC_WSTRING var_IN8;
      CIEC_WSTRING var_OUT1;
      CIEC_WSTRING var_OUT2;
      CIEC_WSTRING var_OUT3;
      CIEC_WSTRING var_OUT4;
      CIEC_WSTRING var_OUT5;
      CIEC_WSTRING var_OUT6;
      CIEC_WSTRING var_OUT7;
      CIEC_WSTRING var_OUT8;
      CIEC_UINT var_UpdateInterval;

      CIEC_BOOL var_QO;
      CIEC_WSTRING var_STATUS;

      CEventConnection conn_INITO;
      CEventConnection conn_IND;

      CDataConnection *conn_QI;
      CDataConnection *conn_IN1;
      CDataConnection *conn_IN2;
      CDataConnection *conn_IN3;
      CDataConnection *conn_IN4;
      CDataConnection *conn_IN5;
      CDataConnection *conn_IN6;
      CDataConnection *conn_IN7;
      CDataConnection *conn_IN8;
      CDataConnection *conn_OUT1;
      CDataConnection *conn_OUT2;
      CDataConnection *conn_OUT3;
      CDataConnection *conn_OUT4;
      CDataConnection *conn_OUT5;
      CDataConnection *conn_OUT6;
      CDataConnection *conn_OUT7;
      CDataConnection *conn_OUT8;
      CDataConnection *conn_UpdateInterval;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_WSTRING> conn_STATUS;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

    protected:
      forte::io::IODeviceController *createDeviceController(CDeviceExecution &paDeviceExecution) override;
      void setConfig() override;
      void onStartup(CEventChainExecutionThread *const paECET) override;
  };
} // namespace forte::eclipse4diac::io::plc01a1
