/*************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: SCALE
 *** Description: Scaling Function Block
 *** Version:
 ***     1.0: 2024-09-19/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_real.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::signalprocessing {
  class FORTE_SCALE final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_SCALE)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_SCALE(forte::StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_REAL var_IN;
      CIEC_REAL var_MAX_IN;
      CIEC_REAL var_MIN_IN;
      CIEC_REAL var_MAX_OUT;
      CIEC_REAL var_MIN_OUT;

      CIEC_REAL var_;

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;
      CDataConnection *conn_MAX_IN;
      CDataConnection *conn_MIN_IN;
      CDataConnection *conn_MAX_OUT;
      CDataConnection *conn_MIN_OUT;

      COutDataConnection<CIEC_REAL> conn_;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_REAL &paIN,
                   const CIEC_REAL &paMAX_IN,
                   const CIEC_REAL &paMIN_IN,
                   const CIEC_REAL &paMAX_OUT,
                   const CIEC_REAL &paMIN_OUT,
                   COutputParameter<CIEC_REAL> pa) {
        COutputGuard guard_pa(pa);
        var_IN = paIN;
        var_MAX_IN = paMAX_IN;
        var_MIN_IN = paMIN_IN;
        var_MAX_OUT = paMAX_OUT;
        var_MIN_OUT = paMIN_OUT;
        executeEvent(scmEventREQID, nullptr);
        *pa = var_;
      }

      void operator()(const CIEC_REAL &paIN,
                      const CIEC_REAL &paMAX_IN,
                      const CIEC_REAL &paMIN_IN,
                      const CIEC_REAL &paMAX_OUT,
                      const CIEC_REAL &paMIN_OUT,
                      COutputParameter<CIEC_REAL> &pa) {
        evt_REQ(paIN, paMAX_IN, paMIN_IN, paMAX_OUT, paMIN_OUT, pa);
      }
  };

  CIEC_REAL func_SCALE(CIEC_REAL st_lv_IN,
                       CIEC_REAL st_lv_MAX_IN,
                       CIEC_REAL st_lv_MIN_IN,
                       CIEC_REAL st_lv_MAX_OUT,
                       CIEC_REAL st_lv_MIN_OUT);
} // namespace forte::eclipse4diac::signalprocessing
