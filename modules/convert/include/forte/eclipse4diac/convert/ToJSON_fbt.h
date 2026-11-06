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
  class FORTE_ToJSON final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_ToJSON)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;
	  void alg_REQ(void);


    public:
      FORTE_ToJSON(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_STRING var_JSONIn;
      CIEC_ANY_VARIANT var_Value;
      CIEC_STRING var_FieldName;

      CIEC_STRING var_JSONOut;

      CEventConnection conn_CNF;

      CDataConnection *conn_JSONIn;
      CDataConnection *conn_Value;
      CDataConnection *conn_FieldName;

      COutDataConnection<CIEC_STRING> conn_JSONOut;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_STRING &paJSONIn, const CIEC_ANY_VARIANT &paValue, const CIEC_STRING &paFieldName, COutputParameter<CIEC_STRING> paJSONOut) {
        COutputGuard guard_JSONOut(paJSONOut);
        var_JSONIn = paJSONIn;
        var_Value = paValue;
        var_FieldName = paFieldName;
        executeEvent(scmEventREQID, nullptr);
        *paJSONOut = var_JSONOut;
      }

      void operator()(const CIEC_STRING &paJSONIn, const CIEC_ANY_VARIANT &paValue, const CIEC_STRING &paFieldName, COutputParameter<CIEC_STRING> paJSONOut) {
        evt_REQ(std::forward<const CIEC_STRING &>(paJSONIn), std::forward<const CIEC_ANY_VARIANT &>(paValue), std::forward<const CIEC_STRING &>(paFieldName), std::forward<COutputParameter<CIEC_STRING>>(paJSONOut));
      }
  };
}

