/*******************************************************************************
 * Copyright (c) 2011, 2025 AIT, ACIN, Profactor GmbH, Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Monika Wenger, Matthias Plasch
 *                - initial API and implementation and/or initial documentation
 *   Martin Jobst - refactor for ANY variant
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_any_variant.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils {
  class FORTE_OUT_ANY_CONSOLE : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_OUT_ANY_CONSOLE)

    private:
      static const TEventID scmEventREQID = 0;

      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

    public:
      FORTE_OUT_ANY_CONSOLE(const StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_STRING var_LABEL;
      CIEC_ANY_VARIANT var_IN;
      CIEC_BOOL var_QO;

      CEventConnection conn_CNF;
      CDataConnection *conn_QI;
      CDataConnection *conn_LABEL;
      CDataConnection *conn_IN;
      COutDataConnection<CIEC_BOOL> conn_QO;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_BOOL &pa_QI,
                   const CIEC_STRING &pa_LABEL,
                   const CIEC_ANY &pa_IN,
                   CAnyBitOutputParameter<CIEC_BOOL> pa_QO) {
        COutputGuard guard_pa_QO(pa_QO);
        var_QI = pa_QI;
        var_LABEL = pa_LABEL;
        var_IN = pa_IN;
        receiveInputEvent(scmEventREQID, nullptr);
        *pa_QO = var_QO;
      }

      void operator()(const CIEC_BOOL &pa_QI,
                      const CIEC_STRING &pa_LABEL,
                      const CIEC_ANY &pa_IN,
                      CAnyBitOutputParameter<CIEC_BOOL> pa_QO) {
        evt_REQ(pa_QI, pa_LABEL, pa_IN, pa_QO);
      }

    protected:
      void setInitialValues() override;

    private:
      std::string mDataOutPutBuffer;
  };
} // namespace forte::eclipse4diac::utils
