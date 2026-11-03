/*************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.3.0.qualifier!
 ***
 *** Name: SPLIT_BYTE_INTO_QUARTERS
 *** Description: this Function extracts the 4 QUARTER BYTE from a byte
 *** Version:
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/forte_st_util.h"

namespace forte::eclipse4diac::utils::splitting {
  class FORTE_SPLIT_BYTE_INTO_QUARTERS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_SPLIT_BYTE_INTO_QUARTERS)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_SPLIT_BYTE_INTO_QUARTERS(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BYTE var_IN;

      CIEC_BYTE var_QUARTER_BYTE_00;
      CIEC_BYTE var_QUARTER_BYTE_01;
      CIEC_BYTE var_QUARTER_BYTE_02;
      CIEC_BYTE var_QUARTER_BYTE_03;

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;

      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_00;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_01;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_02;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_03;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };

  void func_SPLIT_BYTE_INTO_QUARTERS(const CIEC_BYTE &st_lv_IN,
                                     CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_00,
                                     CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_01,
                                     CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_02,
                                     CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_03);
} // namespace forte::eclipse4diac::utils::splitting
