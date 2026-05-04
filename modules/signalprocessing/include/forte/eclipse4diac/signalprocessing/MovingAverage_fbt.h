/*******************************************************************************
 * Copyright (c) 2026 Thomas Mayr
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Mayr - Initial implementation.
 *******************************************************************************/

#pragma once

#include "forte/basicfb.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_real.h"
#include "forte/forte_st_util.h"

namespace forte::eclipse4diac::signalprocessing {
  class FORTE_MovingAverage final : public CBasicFB {
      DECLARE_FIRMWARE_FB(FORTE_MovingAverage)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventRESETID = 1;

      CIEC_INT var_count;
      CIEC_REAL var_sum;
      CIEC_INT var_index;
      CIEC_ARRAY_FIXED<CIEC_REAL, 0, 99> var_buffer;
      CIEC_INT var_effectiveWindow;
      CIEC_INT var_lastWindow;
      CIEC_INT var_i;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_ALG_REQ(void);
      void alg_ALG_RESET(void);

      static const TForteInt16 scmStateSTART = 0;
      static const TForteInt16 scmStateREQ = 1;
      static const TForteInt16 scmStateRESET = 2;

      void enterStateSTART(CEventChainExecutionThread *const paECET);
      void enterStateREQ(CEventChainExecutionThread *const paECET);
      void enterStateRESET(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_MovingAverage(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_REAL var_IN;
      CIEC_INT var_WINDOW;

      CIEC_REAL var_OUT;

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;
      CDataConnection *conn_WINDOW;

      COutDataConnection<CIEC_REAL> conn_OUT;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_REAL &paIN, const CIEC_INT &paWINDOW, COutputParameter<CIEC_REAL> paOUT) {
        COutputGuard guard_OUT(paOUT);
        var_IN = paIN;
        var_WINDOW = paWINDOW;
        executeEvent(scmEventREQID, nullptr);
        *paOUT = var_OUT;
      }

      void evt_RESET(const CIEC_REAL &paIN, const CIEC_INT &paWINDOW, COutputParameter<CIEC_REAL> paOUT) {
        COutputGuard guard_OUT(paOUT);
        var_IN = paIN;
        var_WINDOW = paWINDOW;
        executeEvent(scmEventRESETID, nullptr);
        *paOUT = var_OUT;
      }
  };
}

