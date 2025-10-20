/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_char.h"
#include "forte/datatypes/forte_string.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils {
  class FORTE_GetInstancePath final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_GetInstancePath)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_GetInstancePath(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_CHAR var_Sep;

      CIEC_STRING var_Path;

      CEventConnection conn_CNF;

      CDataConnection *conn_Sep;

      COutDataConnection<CIEC_STRING> conn_Path;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_CHAR &paSep, COutputParameter<CIEC_STRING> paPath) {
        COutputGuard guard_paPath(paPath);
        var_Sep = paSep;
        executeEvent(scmEventREQID, nullptr);
        *paPath = var_Path;
      }

      void operator()(const CIEC_CHAR &paSep, COutputParameter<CIEC_STRING> paPath) {
        evt_REQ(paSep, paPath);
      }
  };
} // namespace forte::eclipse4diac::utils
