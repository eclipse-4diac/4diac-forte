/*************************************************************************
 * Copyright (c) 2024
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * This file was generated using the 4DIAC FORTE Export Filter 3.3.0.qualifier!
 *
 * Name: TIMESTAMP_NS
 * Description: Creates by default a Unix-Epoch-Timestamp in nanoseconds. Use other start dates for other timestamp
 * types. Version: 3.0.1: 2026-09-02/Franz Höpfinger - HR Agrartechnik GmbH - add missing PACKAGE statement, improved
 * comments 3.0: 2025-04-14/Patrick Aigner -  - changed package 1.0: 2024-10-14/Monika Wenger -  -
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_date_and_time.h"
#include "forte/datatypes/forte_ulint.h"

namespace forte::eclipse4diac::utils::timing {
  class FORTE_TIMESTAMP_NS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_TIMESTAMP_NS)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_TIMESTAMP_NS(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_DATE_AND_TIME var_startDate;

      CIEC_ULINT var_;

      CEventConnection conn_CNF;

      CDataConnection *conn_startDate;

      COutDataConnection<CIEC_ULINT> conn_;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };

  CIEC_ULINT func_TIMESTAMP_NS(const CIEC_DATE_AND_TIME &st_lv_startDate);
} // namespace forte::eclipse4diac::utils::timing
