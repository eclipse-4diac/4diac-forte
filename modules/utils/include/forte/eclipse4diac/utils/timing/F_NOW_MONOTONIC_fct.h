/*******************************************************************************
 * Copyright (c) 2024 HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0

 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: F_NOW_MONOTONIC
 *** Description: return a clear monotonic time
 *** Version:
 ***     1.0: 2024-10-01/Franz Höpfinger - HR Agrartechnik GmbH    -
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_time.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils::timing {
  class FORTE_F_NOW_MONOTONIC final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_F_NOW_MONOTONIC)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_F_NOW_MONOTONIC(forte::StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_TIME var_;

      CEventConnection conn_CNF;

      COutDataConnection<CIEC_TIME> conn_;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(COutputParameter<CIEC_TIME> pa) {
        COutputGuard guard_pa(pa);
        executeEvent(scmEventREQID, nullptr);
        *pa = var_;
      }

      void operator()(COutputParameter<CIEC_TIME> pa) {
        evt_REQ(pa);
      }
  };

  CIEC_TIME func_F_NOW_MONOTONIC();
} // namespace forte::eclipse4diac::utils::timing
