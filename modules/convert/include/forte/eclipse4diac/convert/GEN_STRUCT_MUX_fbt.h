/*******************************************************************************
 * Copyright (c) 2020, 2023 Johannes Kepler University
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Martin Jobst - add generic readInputData and writeOutputData
 *******************************************************************************/
#pragma once

#include "forte/genfb.h"
#include "forte/datatypes/forte_struct.h"

#include <memory>

namespace forte::eclipse4diac::convert {
  class GEN_STRUCT_MUX final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_STRUCT_MUX)

    protected:
      size_t getGenEOOffset() override {
        return 1;
      }

      size_t getGenDOOffset() override {
        return 1;
      }

      CEventConnection *getEOConUnchecked(TPortId paEONum) override;
      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t paIndex) override;
      CDataConnection *getDOConUnchecked(const TPortId paIndex) override;

    private:
      static const TEventID scmEventREQID = 0;

      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEI) override;
      void writeOutputData(TEventID paEO) override;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      CEventConnection conn_CNF;
      std::unique_ptr<CIEC_STRUCT> var_OUT;
      std::unique_ptr<CGenDataConnection> conn_OUT;

    public:
      GEN_STRUCT_MUX(const GEN_STRUCT_MUX &paOther) = delete;
      GEN_STRUCT_MUX(const StringId paInstanceNameId, CFBContainer &paContainer);
      ~GEN_STRUCT_MUX() override = default;

      static StringId getStructNameId(std::string_view paConfigString);
  };
} // namespace forte::eclipse4diac::convert
