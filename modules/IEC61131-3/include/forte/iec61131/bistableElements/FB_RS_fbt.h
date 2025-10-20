/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/simplefb.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61131::bistableElements {
  class FORTE_FB_RS final : public CSimpleFB {
      DECLARE_FIRMWARE_FB(FORTE_FB_RS)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_REQ(void);

      void enterStateREQ(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_FB_RS(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_S;
      CIEC_BOOL var_R1;

      CIEC_BOOL var_Q1;

      CEventConnection conn_CNF;

      CDataConnection *conn_S;
      CDataConnection *conn_R1;

      COutDataConnection<CIEC_BOOL> conn_Q1;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_BOOL &paS, const CIEC_BOOL &paR1, CAnyBitOutputParameter<CIEC_BOOL> paQ1) {
        COutputGuard guard_paQ1(paQ1);
        var_S = paS;
        var_R1 = paR1;
        executeEvent(scmEventREQID, nullptr);
        *paQ1 = var_Q1;
      }

      void operator()(const CIEC_BOOL &paS, const CIEC_BOOL &paR1, CAnyBitOutputParameter<CIEC_BOOL> paQ1) {
        evt_REQ(paS, paR1, paQ1);
      }
  };
} // namespace forte::iec61131::bistableElements
