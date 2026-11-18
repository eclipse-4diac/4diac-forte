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

#include "MCCIODevice.h"
#include "../mccdaqhat_config.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_usint.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"

#include <array>

namespace forte::eclipse4diac::io::mccdaqhat {

  class FORTE_MCC128 final : public MCCIODevice {
      DECLARE_FIRMWARE_FB(FORTE_MCC128)

    private:
      static const TEventID scmEventMAPID = 0;
      static const TEventID scmEventMAPOID = 0;
      static const TEventID scmEventINDID = 1;
      static constexpr size_t scmFirstChannelDI = 4;
      static constexpr size_t scmNumDIs = scmFirstChannelDI + scmMCC128Channels;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

      unsigned int getConfiguredAddress() override;
      const char *init() override;
      void initHandles() override;

      TChannelUsage getChannelUsage();

    public:
      FORTE_MCC128(forte::StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_USINT var_Address;
      CIEC_USINT var_InputMode;
      CIEC_USINT var_InputRange;
      std::array<CIEC_STRING, scmMCC128Channels> var_AI;

      CIEC_BOOL var_QO;
      CIEC_WSTRING var_STATUS;

      CEventConnection conn_MAPO;
      CEventConnection conn_IND;

      CDataConnection *conn_QI;
      CDataConnection *conn_Address;
      CDataConnection *conn_InputMode;
      CDataConnection *conn_InputRange;
      std::array<CDataConnection *, scmMCC128Channels> conn_AI;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_WSTRING> conn_STATUS;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };

} // namespace forte::eclipse4diac::io::mccdaqhat
