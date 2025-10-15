/*******************************************************************************
 * Copyright (c) 2020, 2025 Johannes Kepler University, Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
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
 *   Markus Meingast - add support for configured struct demux instances
 *******************************************************************************/
#pragma once

#include "forte/genfb.h"
#include "forte/datatypes/forte_struct.h"

#include <memory>

namespace forte::eclipse4diac::convert {
  class GEN_STRUCT_DEMUX final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_STRUCT_DEMUX)

    protected:
      size_t getGenEOOffset() override {
        return 1;
      }

      size_t getGenDIOffset() override {
        return 1;
      }

      CEventConnection *getEOConUnchecked(TPortId paEONum) override;
      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t paIndex) override;
      CDataConnection **getDIConUnchecked(const TPortId paIndex) override;

    private:
      static constexpr char NESTED_VAR_SEPARATOR = '%';
      static constexpr std::string_view STRUCT_NAME_SEPARATOR = "____";

      static const TEventID scmEventREQID = 0;

      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEI) override;
      void writeOutputData(TEventID paEO) override;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      void fillConfiguredInterfaceSpec(std::vector<std::string_view> &paConfiguredMemberNames);
      void fillInterfaceSpec();

      std::vector<std::string_view> getConfiguredMemberNames(std::string_view paMemberNameString);
      CIEC_ANY *getNestedMember(const StringId paNameId, CIEC_STRUCT *paStructType);

      CEventConnection conn_CNF;
      std::unique_ptr<CIEC_STRUCT> var_IN;
      CDataConnection *conn_IN;
      std::vector<StringId> mDoNames;
      std::vector<CIEC_ANY *> mConfiguredDOPorts;

    public:
      GEN_STRUCT_DEMUX(const GEN_STRUCT_DEMUX &paOther) = delete;
      GEN_STRUCT_DEMUX(const StringId paInstanceNameId, CFBContainer &paContainer);
      ~GEN_STRUCT_DEMUX() override = default;
  };
} // namespace forte::eclipse4diac::convert
