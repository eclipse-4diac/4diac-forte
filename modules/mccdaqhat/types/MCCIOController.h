/*******************************************************************************
 * Copyright (c) 2026 Thomas Oellinger
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Oellinger - initial implementation
 *******************************************************************************/

#pragma once

#include "forte/io/configFB/io_master_multi.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_wstring.h"
#include "MCCBusAdapter.h"

namespace forte::eclipse4diac::io::mccdaqhat {

  class FORTE_MCCIOController final : public forte::io::IOConfigFBMultiMaster {
      DECLARE_FIRMWARE_FB(FORTE_MCCIOController)

    private:
      static const TEventID scmEventINITID = 0;
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventINDID = 1;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    protected:
      forte::io::IODeviceController *createDeviceController(CDeviceExecution &paDeviceExecution) override;
      void setConfig() override;

    public:
      FORTE_MCCIOController(forte::StringId paInstanceNameId, CFBContainer &paContainer);
      ~FORTE_MCCIOController() override;

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

      forte::CPlugPin<FORTE_MCCBusAdapter_Plug> var_BusAdapterOut;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
      forte::IPlugPin *getPlugPinUnchecked(size_t) override;
  };

} // namespace forte::eclipse4diac::io::mccdaqhat
