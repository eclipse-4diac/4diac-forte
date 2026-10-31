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

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_any_variant.h"
#include "forte/datatypes/forte_string.h"
#include "forte/forte_st_util.h"

namespace forte::eclipse4diac::convert {
  class FORTE_ReadJSON final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_ReadJSON)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;
	  void alg_REQ(void);

    public:
      FORTE_ReadJSON(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_STRING var_JSONIn;
      CIEC_STRING var_FieldName;

      CIEC_ANY_VARIANT var_Value;

      CEventConnection conn_CNF;

      CDataConnection *conn_JSONIn;
      CDataConnection *conn_FieldName;

      COutDataConnection<CIEC_ANY_VARIANT> conn_Value;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_STRING &paJSONIn, const CIEC_STRING &paFieldName, COutputParameter<CIEC_ANY_VARIANT> paValue) {
        COutputGuard guard_Value(paValue);
        var_JSONIn = paJSONIn;
        var_FieldName = paFieldName;
        executeEvent(scmEventREQID, nullptr);
        paValue->setValue(var_Value.unwrap());
      }

      void operator()(const CIEC_STRING &paJSONIn, const CIEC_STRING &paFieldName, COutputParameter<CIEC_ANY_VARIANT> paValue) {
        evt_REQ(std::forward<const CIEC_STRING &>(paJSONIn), std::forward<const CIEC_STRING &>(paFieldName), std::forward<COutputParameter<CIEC_ANY_VARIANT>>(paValue));
      }
  };
}

