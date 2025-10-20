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
#include "forte/datatypes/forte_any_bit_variant.h"
#include "forte/datatypes/forte_any_int_variant.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61131::bitwiseOperators {
  class FORTE_F_ROL : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_F_ROL)

    private:
      static const TEventID scmEventREQID = 0;

      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

    public:
      FORTE_F_ROL(const StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_ANY_BIT_VARIANT var_IN;
      CIEC_ANY_INT_VARIANT var_N;
      CIEC_ANY_BIT_VARIANT var_OUT;

      CEventConnection conn_CNF;
      CDataConnection *conn_IN;
      CDataConnection *conn_N;
      COutDataConnection<CIEC_ANY_BIT_VARIANT> conn_OUT;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_ANY_BIT &pa_IN, const CIEC_ANY_INT &pa_N, COutputParameter<CIEC_ANY_BIT_VARIANT> pa_OUT) {
        COutputGuard guard_pa_OUT(pa_OUT);
        var_IN = pa_IN;
        var_N = pa_N;
        receiveInputEvent(scmEventREQID, nullptr);
        pa_OUT->setValue(var_OUT.unwrap());
      }

      void
      operator()(const CIEC_ANY_BIT &pa_IN, const CIEC_ANY_INT &pa_N, COutputParameter<CIEC_ANY_BIT_VARIANT> pa_OUT) {
        evt_REQ(pa_IN, pa_N, pa_OUT);
      }

    protected:
      void setInitialValues() override;
  };
} // namespace forte::iec61131::bitwiseOperators
