/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_any_variant.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::convert {
  class FORTE_GET_AT_INDEX : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_GET_AT_INDEX)

    private:
      static const TEventID scmEventREQID = 0;

      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

    public:
      FORTE_GET_AT_INDEX(const StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_ANY_VARIANT var_IN_ARRAY;
      CIEC_UINT var_INDEX;
      CIEC_BOOL var_QO;
      CIEC_ANY_VARIANT var_OUT;

      CEventConnection conn_CNF;
      CDataConnection *conn_IN_ARRAY;
      CDataConnection *conn_INDEX;
      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_ANY_VARIANT> conn_OUT;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_ANY &pa_IN_ARRAY,
                   const CIEC_UINT &pa_INDEX,
                   CAnyBitOutputParameter<CIEC_BOOL> pa_QO,
                   COutputParameter<CIEC_ANY_VARIANT> pa_OUT) {
        COutputGuard guard_pa_QO(pa_QO);
        COutputGuard guard_pa_OUT(pa_OUT);
        var_IN_ARRAY = pa_IN_ARRAY;
        var_INDEX = pa_INDEX;
        receiveInputEvent(scmEventREQID, nullptr);
        *pa_QO = var_QO;
        pa_OUT->setValue(var_OUT.unwrap());
      }

      void operator()(const CIEC_ANY &pa_IN_ARRAY,
                      const CIEC_UINT &pa_INDEX,
                      CAnyBitOutputParameter<CIEC_BOOL> pa_QO,
                      COutputParameter<CIEC_ANY_VARIANT> pa_OUT) {
        evt_REQ(pa_IN_ARRAY, pa_INDEX, pa_QO, pa_OUT);
      }

    protected:
      void setInitialValues() override;
  };
} // namespace forte::eclipse4diac::convert
