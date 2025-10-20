/*******************************************************************************
 * Copyright (c) 2013 ACIN
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_date_and_time.h"
#include "forte/datatypes/forte_time.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61131::arithmetic {
  class FORTE_F_SUB_DT_TIME : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_F_SUB_DT_TIME)

    private:
      static const TEventID scmEventREQID = 0;

      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

    public:
      FORTE_F_SUB_DT_TIME(const StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_DATE_AND_TIME var_IN1;
      CIEC_TIME var_IN2;
      CIEC_DATE_AND_TIME var_OUT;

      CEventConnection conn_CNF;
      CDataConnection *conn_IN1;
      CDataConnection *conn_IN2;
      COutDataConnection<CIEC_DATE_AND_TIME> conn_OUT;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void
      evt_REQ(const CIEC_DATE_AND_TIME &pa_IN1, const CIEC_TIME &pa_IN2, COutputParameter<CIEC_DATE_AND_TIME> pa_OUT) {
        COutputGuard guard_pa_OUT(pa_OUT);
        var_IN1 = pa_IN1;
        var_IN2 = pa_IN2;
        receiveInputEvent(scmEventREQID, nullptr);
        *pa_OUT = var_OUT;
      }

      void operator()(const CIEC_DATE_AND_TIME &pa_IN1,
                      const CIEC_TIME &pa_IN2,
                      COutputParameter<CIEC_DATE_AND_TIME> pa_OUT) {
        evt_REQ(pa_IN1, pa_IN2, pa_OUT);
      }

    protected:
      void setInitialValues() override;
  };
} // namespace forte::iec61131::arithmetic
