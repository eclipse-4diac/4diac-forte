/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: GEN_F_MUX
 *** Description: multiplexer
 *** Version:
 ***     1.0: 2013-08-30/Monika Wenger - TU Wien ACIN -
 ***     1.1: 2024-07-15/Franz Höpfinger - HR Agrartechnik GmbH - F_MUX_2 copy and added 1 IN
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 *************************************************************************/

#pragma once

#include "forte/genfb.h"
#include "forte/datatypes/forte_any_int_variant.h"
#include "forte/datatypes/forte_any_variant.h"

namespace forte::iec61131::selection {
  class GEN_F_MUX final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_F_MUX)

    protected:
      size_t getGenEOOffset() override {
        // CNF
        return 1;
      }

      size_t getGenDOOffset() override {
        // OUT
        return 1;
      }

      size_t getGenDIOffset() override {
        // K
        return 1;
      }

      void createGenInputData() override;

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      std::unique_ptr<CIEC_ANY_VARIANT[]> mGenDIs;
      std::vector<StringId> mDINames;

    public:
      GEN_F_MUX(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_ANY_INT_VARIANT var_K;

      CIEC_ANY_VARIANT var_OUT;

      CEventConnection conn_CNF;

      CDataConnection *conn_K;

      COutDataConnection<CIEC_ANY_VARIANT> conn_OUT;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };
} // namespace forte::iec61131::selection
