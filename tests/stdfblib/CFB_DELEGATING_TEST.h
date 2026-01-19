/*******************************************************************************
 * Copyright (c) 2026 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Erich Jobst - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_struct.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131/selection/GEN_F_MOVE_fbt.h"

namespace forte::test {
  class CIEC_DelegatingTestStruct final : public CIEC_STRUCT {
      DECLARE_FIRMWARE_DATATYPE(DelegatingTestStruct)

    public:
      CIEC_DelegatingTestStruct();

      CIEC_DelegatingTestStruct(const CIEC_BOOL &paVAR1, const CIEC_BOOL &paVAR2, const CIEC_BOOL &paVAR3);

      CIEC_BOOL var_VAR1;
      CIEC_BOOL var_VAR2;
      CIEC_BOOL var_VAR3;

      size_t getStructSize() const override {
        return 3;
      }

      const StringId *elementNames() const override {
        return scmElementNames;
      }

      StringId getStructTypeNameID() const override;

      void setValue(const CIEC_ANY &paValue) override;

      CIEC_ANY *getMember(size_t) override;
      const CIEC_ANY *getMember(size_t) const override;

    private:
      static const StringId scmElementNames[];
  };

  class FORTE_CFB_DELEGATING_TEST final : public CCompositeFB {
      DECLARE_FIRMWARE_FB(FORTE_CFB_DELEGATING_TEST)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      CInternalFB<forte::iec61131::selection::GEN_F_MOVE> fb_F_MOVE;
      CInternalFB<forte::iec61131::selection::GEN_F_MOVE> fb_F_MOVE_1;
      CInternalFB<forte::iec61131::selection::GEN_F_MOVE> fb_F_MOVE_2;
      CInternalFB<forte::iec61131::selection::GEN_F_MOVE> fb_F_MOVE_3;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_CFB_DELEGATING_TEST(StringId paInstanceNameId, CFBContainer &paContainer);

      CEventConnection conn_CNF;

      CDataConnection *conn_QI;
      CDataConnection *conn_DI1;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<forte::test::CIEC_DelegatingTestStruct> conn_DO1;

      COutDataConnection<CIEC_BOOL> conn_if2in_QI;
      COutDataConnection<forte::test::CIEC_DelegatingTestStruct> conn_if2in_DI1;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
      CDataConnection *getIf2InConUnchecked(TPortId) override;
  };
} // namespace forte::test
