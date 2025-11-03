/*******************************************************************************
 * Copyright (c) 2023, 2025 OFFIS e.V.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jörg Walter - initial implementation
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/io/configFB/io_configFB_controller.h"

namespace forte::eclipse4diac::io::gpiochip {

  class FORTE_GPIOChip : public forte::io::IOConfigFBController {
      DECLARE_FIRMWARE_FB(FORTE_GPIOChip)

    private:
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventINDID = 1;
      static const TEventID scmEventINITID = 0;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_GPIOChip(const forte::StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_WSTRING var_VALUE;
      CIEC_UINT var_ChipNumber;
      CIEC_UINT var_LineNumber;
      CIEC_UINT var_ReadWriteMode;
      CIEC_UINT var_BiasMode;
      CIEC_BOOL var_ActiveLow;

      CIEC_BOOL var_QO;
      CIEC_WSTRING var_STATUS;

      CEventConnection conn_INITO;
      CEventConnection conn_IND;

      CDataConnection *conn_QI;
      CDataConnection *conn_VALUE;
      CDataConnection *conn_ChipNumber;
      CDataConnection *conn_LineNumber;
      CDataConnection *conn_ReadWriteMode;
      CDataConnection *conn_BiasMode;
      CDataConnection *conn_ActiveLow;

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
}
