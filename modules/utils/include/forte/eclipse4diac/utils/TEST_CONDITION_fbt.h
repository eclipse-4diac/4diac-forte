/*******************************************************************************
 * Copyright (c) 2019, 2023 fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *   Martin Jobst - add readInputData and writeOutputData
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/arch/forte_sync.h"

namespace forte::eclipse4diac::utils {
  class FORTE_TEST_CONDITION final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_TEST_CONDITION)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      static unsigned int smExecutedTests;
      static unsigned int smFailedTests;

      static bool smfinalReportPrinted;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

      arch::CSyncObject mFinalReportMutex;

    public:
      FORTE_TEST_CONDITION(StringId paInstanceNameId, CFBContainer &paContainer);
      ~FORTE_TEST_CONDITION() override;

      CIEC_BOOL var_check;

      CEventConnection conn_CNF;

      CDataConnection *conn_check;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_BOOL &pacheck) {
        var_check = pacheck;
        executeEvent(scmEventREQID, nullptr);
      }

      void operator()(const CIEC_BOOL &pacheck) {
        evt_REQ(pacheck);
      }
  };
} // namespace forte::eclipse4diac::utils
