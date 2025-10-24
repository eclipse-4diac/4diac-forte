/*******************************************************************************
 * Copyright (c) 2013, 2024 ACIN, Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - refactored from F_MOVE
 *******************************************************************************/

#pragma once

#include "forte/genfb.h"

namespace forte::iec61131::selection {
  class GEN_F_MOVE final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_F_MOVE)

    protected:
      size_t getGenEOOffset() override {
        return 1;
      }

      size_t getGenDIOffset() override {
        return 1;
      }

      size_t getGenDOOffset() override {
        return 1;
      }

      CEventConnection *getEOConUnchecked(TPortId paEONum) override;
      CIEC_ANY *getDI(size_t paIndex) override;
      CIEC_ANY *getDO(size_t paIndex) override;
      CDataConnection **getDIConUnchecked(const TPortId paIndex) override;
      CDataConnection *getDOConUnchecked(TPortId paDONum) override;

    private:
      static const TEventID scmEventREQID = 0;

      static const TEventID scmEventCNFID = 0;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

      static StringId getDataTypeNameId(const char *paConfigString);

      CEventConnection conn_CNF;

      std::unique_ptr<CIEC_ANY> mIn;
      CDataConnection *conn_IN;

      std::unique_ptr<CGenDataConnection> conn_OUT;

    public:
      GEN_F_MOVE(const StringId paInstanceNameId, CFBContainer &paContainer);
      ~GEN_F_MOVE() override = default;
  };
} // namespace forte::iec61131::selection
