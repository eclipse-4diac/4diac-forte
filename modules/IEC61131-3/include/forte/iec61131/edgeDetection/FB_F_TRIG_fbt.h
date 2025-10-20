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

namespace forte::iec61131::edgeDetection {
  class FORTE_FB_F_TRIG final : public CSimpleFB {
      DECLARE_FIRMWARE_FB(FORTE_FB_F_TRIG)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      CIEC_BOOL var_MEM;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_REQ(void);

      void enterStateREQ(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_FB_F_TRIG(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_CLK;

      CIEC_BOOL var_Q;

      CEventConnection conn_CNF;

      CDataConnection *conn_CLK;

      COutDataConnection<CIEC_BOOL> conn_Q;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_BOOL &paCLK, CAnyBitOutputParameter<CIEC_BOOL> paQ) {
        COutputGuard guard_paQ(paQ);
        var_CLK = paCLK;
        executeEvent(scmEventREQID, nullptr);
        *paQ = var_Q;
      }

      void operator()(const CIEC_BOOL &paCLK, CAnyBitOutputParameter<CIEC_BOOL> paQ) {
        evt_REQ(paCLK, paQ);
      }
  };
} // namespace forte::iec61131::edgeDetection
