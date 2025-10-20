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
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_usint.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61131::conversion {
  class FORTE_F_USINT_AS_STRING final : public CSimpleFB {
      DECLARE_FIRMWARE_FB(FORTE_F_USINT_AS_STRING)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_REQ(void);

      void enterStateREQ(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_F_USINT_AS_STRING(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_USINT var_IN;

      CIEC_STRING var_OUT;

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;

      COutDataConnection<CIEC_STRING> conn_OUT;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_USINT &paIN, COutputParameter<CIEC_STRING> paOUT) {
        COutputGuard guard_paOUT(paOUT);
        var_IN = paIN;
        executeEvent(scmEventREQID, nullptr);
        *paOUT = var_OUT;
      }

      void operator()(const CIEC_USINT &paIN, COutputParameter<CIEC_STRING> paOUT) {
        evt_REQ(paIN, paOUT);
      }
  };
} // namespace forte::iec61131::conversion
