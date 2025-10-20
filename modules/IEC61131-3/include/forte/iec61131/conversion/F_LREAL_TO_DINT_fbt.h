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
#include "forte/datatypes/forte_dint.h"
#include "forte/datatypes/forte_lreal.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61131::conversion {
  class FORTE_F_LREAL_TO_DINT final : public CSimpleFB {
      DECLARE_FIRMWARE_FB(FORTE_F_LREAL_TO_DINT)

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
      FORTE_F_LREAL_TO_DINT(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_LREAL var_IN;

      CIEC_DINT var_OUT;

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;

      COutDataConnection<CIEC_DINT> conn_OUT;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_LREAL &paIN, COutputParameter<CIEC_DINT> paOUT) {
        COutputGuard guard_paOUT(paOUT);
        var_IN = paIN;
        executeEvent(scmEventREQID, nullptr);
        *paOUT = var_OUT;
      }

      void operator()(const CIEC_LREAL &paIN, COutputParameter<CIEC_DINT> paOUT) {
        evt_REQ(paIN, paOUT);
      }
  };
} // namespace forte::iec61131::conversion
