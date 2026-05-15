/*************************************************************************
 *** Copyright (c) 2026 Martin Melik Merkumians, Demmler Andreas Fahrzeugbau
 ***
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** Contributors:
 ***     Martin Melik Merkumians - Initial implementation
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.0.202603111726!
 ***
 *** Name: F_UPPER_BOUND
 *** Description: Returns the upper bound of the dimension of the given array
 *** Version:
 ***     1.0: 2026-01-23/Franz Höpfinger - Demmler Andreas Fahrzeugbau - Initial Implementation
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_any_derived_variant.h"
#include "forte/datatypes/forte_any_int_variant.h"
#include "forte/forte_st_util.h"

namespace forte::iec61131::arrays {
  class FORTE_F_UPPER_BOUND final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_F_UPPER_BOUND)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_F_UPPER_BOUND(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_ANY_INT_VARIANT var_DIM;
      CIEC_ANY_DERIVED_VARIANT var_ARR;

      CIEC_ANY_INT_VARIANT var_OUT;

      CEventConnection conn_CNF;

      CDataConnection *conn_DIM;
      CDataConnection *conn_ARR;

      COutDataConnection<CIEC_ANY_INT_VARIANT> conn_OUT;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_ANY_INT_VARIANT &paDIM,
                   const CIEC_ANY_DERIVED_VARIANT &paARR,
                   COutputParameter<CIEC_ANY_INT_VARIANT> paOUT) {
        COutputGuard guard_OUT(paOUT);
        var_DIM = paDIM;
        var_ARR = paARR;
        executeEvent(scmEventREQID, nullptr);
        paOUT->setValue(var_OUT.unwrap());
      }

      void operator()(const CIEC_ANY_INT_VARIANT &paDIM,
                      const CIEC_ANY_DERIVED_VARIANT &paARR,
                      COutputParameter<CIEC_ANY_INT_VARIANT> paOUT) {
        evt_REQ(std::forward<const CIEC_ANY_INT_VARIANT &>(paDIM),
                std::forward<const CIEC_ANY_DERIVED_VARIANT &>(paARR),
                std::forward<COutputParameter<CIEC_ANY_INT_VARIANT>>(paOUT));
      }
  };
} // namespace forte::iec61131::arrays
