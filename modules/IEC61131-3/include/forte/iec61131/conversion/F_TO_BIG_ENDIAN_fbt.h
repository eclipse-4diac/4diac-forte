/*************************************************************************
 *** Copyright (c) 2026 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202604152003!
 ***
 *** Name: F_TO_BIG_ENDIAN
 *** Description: Convert to Big Endian
 *** Version:
 ***     1.0: 2026-01-31/Franz Höpfinger - HR Agrartechnik GmbH - conform to IEC 61131-3 specification for this Block
 *************************************************************************/

#pragma once

#include "forte/simplefb.h"
#include "forte/datatypes/forte_any_variant.h"
#include "forte/forte_st_util.h"

namespace forte::iec61131::conversion {
  class FORTE_F_TO_BIG_ENDIAN final : public CSimpleFB {
      DECLARE_FIRMWARE_FB(FORTE_F_TO_BIG_ENDIAN)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_REQ(void);

      void enterStateREQ(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_F_TO_BIG_ENDIAN(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_ANY_VARIANT var_IN;

      CIEC_ANY_VARIANT var_OUT;

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;

      COutDataConnection<CIEC_ANY_VARIANT> conn_OUT;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_ANY_VARIANT &paIN, COutputParameter<CIEC_ANY_VARIANT> paOUT) {
        COutputGuard guard_OUT(paOUT);
        var_IN = paIN;
        executeEvent(scmEventREQID, nullptr);
        paOUT->setValue(var_OUT.unwrap());
      }

      void operator()(const CIEC_ANY_VARIANT &paIN, COutputParameter<CIEC_ANY_VARIANT> paOUT) {
        evt_REQ(std::forward<const CIEC_ANY_VARIANT &>(paIN), std::forward<COutputParameter<CIEC_ANY_VARIANT>>(paOUT));
      }
  };
} // namespace forte::iec61131::conversion
