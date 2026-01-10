/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.0.0.202512201229!
 ***
 *** Name: SPLIT_DWORD_INTO_BYTES
 *** Description: this Function extracts the 4 BYTE from a dword
 *** Version:
 ***     1.0: 2025-12-21/Franz Höpfinger - HR Agrartechnik - initial Version
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/forte_st_util.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils::splitting {
  class FORTE_SPLIT_DWORD_INTO_BYTES final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_SPLIT_DWORD_INTO_BYTES)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_SPLIT_DWORD_INTO_BYTES(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_DWORD var_IN;

      CIEC_BYTE var_BYTE_00;
      CIEC_BYTE var_BYTE_01;
      CIEC_BYTE var_BYTE_02;
      CIEC_BYTE var_BYTE_03;

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;

      COutDataConnection<CIEC_BYTE> conn_BYTE_00;
      COutDataConnection<CIEC_BYTE> conn_BYTE_01;
      COutDataConnection<CIEC_BYTE> conn_BYTE_02;
      COutDataConnection<CIEC_BYTE> conn_BYTE_03;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_DWORD &paIN, CAnyBitOutputParameter<CIEC_BYTE> paBYTE_00, CAnyBitOutputParameter<CIEC_BYTE> paBYTE_01, CAnyBitOutputParameter<CIEC_BYTE> paBYTE_02, CAnyBitOutputParameter<CIEC_BYTE> paBYTE_03) {
        COutputGuard guard_BYTE_00(paBYTE_00);
        COutputGuard guard_BYTE_01(paBYTE_01);
        COutputGuard guard_BYTE_02(paBYTE_02);
        COutputGuard guard_BYTE_03(paBYTE_03);
        var_IN = paIN;
        executeEvent(scmEventREQID, nullptr);
        *paBYTE_00 = var_BYTE_00;
        *paBYTE_01 = var_BYTE_01;
        *paBYTE_02 = var_BYTE_02;
        *paBYTE_03 = var_BYTE_03;
      }

      void operator()(const CIEC_DWORD &paIN, CAnyBitOutputParameter<CIEC_BYTE> paBYTE_00, CAnyBitOutputParameter<CIEC_BYTE> paBYTE_01, CAnyBitOutputParameter<CIEC_BYTE> paBYTE_02, CAnyBitOutputParameter<CIEC_BYTE> paBYTE_03) {
        evt_REQ(std::forward<const CIEC_DWORD &>(paIN), std::forward<CAnyBitOutputParameter<CIEC_BYTE>>(paBYTE_00), std::forward<CAnyBitOutputParameter<CIEC_BYTE>>(paBYTE_01), std::forward<CAnyBitOutputParameter<CIEC_BYTE>>(paBYTE_02), std::forward<CAnyBitOutputParameter<CIEC_BYTE>>(paBYTE_03));
      }
  };

  void func_SPLIT_DWORD_INTO_BYTES(const CIEC_DWORD &st_lv_IN, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_00, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_01, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_02, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_03);
}

