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
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_usint.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"

#include <array>

namespace forte::eclipse4diac::io::mccdaqhat {

  class FORTE_MCC152 final : public MCCIODevice {
      DECLARE_FIRMWARE_FB(FORTE_MCC152)

    private:
      static const TEventID scmEventMAPID = 0;
      static const TEventID scmEventMAPOID = 0;
      static const TEventID scmEventINDID = 1;
      static constexpr size_t scmFirstDIPortDI = 4;
      static constexpr size_t scmFirstDOPortDI = scmFirstDIPortDI + scmMCC152DioChannels;
      static constexpr size_t scmFirstAOPortDI = scmFirstDOPortDI + scmMCC152DioChannels;
      static constexpr size_t scmNumDIs = scmFirstAOPortDI + scmMCC152AoChannels;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

      unsigned int getConfiguredAddress() override;
      const char *init() override;
      void initHandles() override;

      static TChannelUsage usageOf(const std::array<CIEC_STRING, scmMCC152DioChannels> &paIds);

    public:
      FORTE_MCC152(forte::StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_USINT var_Address;
      CIEC_BYTE var_PullEnable;
      CIEC_BYTE var_PullConfig;
      std::array<CIEC_STRING, scmMCC152DioChannels> var_DI;
      std::array<CIEC_STRING, scmMCC152DioChannels> var_DO;
      std::array<CIEC_STRING, scmMCC152AoChannels> var_AO;

      CIEC_BOOL var_QO;
      CIEC_WSTRING var_STATUS;

      CEventConnection conn_MAPO;
      CEventConnection conn_IND;

      CDataConnection *conn_QI;
      CDataConnection *conn_Address;
      CDataConnection *conn_PullEnable;
      CDataConnection *conn_PullConfig;
      std::array<CDataConnection *, scmMCC152DioChannels> conn_DI;
      std::array<CDataConnection *, scmMCC152DioChannels> conn_DO;
      std::array<CDataConnection *, scmMCC152AoChannels> conn_AO;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_WSTRING> conn_STATUS;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };

} // namespace forte::eclipse4diac::io::mccdaqhat
