/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH
 *                      2018 Johannes Kepler University
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB namespace forte::iec61131::arithmetic {
class for better handling generic FBs
 *   Martin Jobst
 *     - refactor for ANY variant
 *     - add generic readInputData and writeOutputData
 *******************************************************************************/

#pragma once

#include "forte/genfb.h"
#include "forte/datatypes/forte_any_magnitude_variant.h"

namespace forte::iec61131::arithmetic {
  class GEN_ADD final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_ADD)

    private:
    protected:
      size_t getGenEOOffset() override {
        return 1;
      }

      size_t getGenDOOffset() override {
        return 1;
      }

      CEventConnection *getEOConUnchecked(TPortId) override;
      void createGenInputData() override;
      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CDataConnection *getDOConUnchecked(const TPortId paIndex) override;

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEI) override;
      void writeOutputData(TEventID paEO) override;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      CEventConnection conn_CNF;
      std::unique_ptr<CIEC_ANY_MAGNITUDE_VARIANT[]> mGenDIs;
      std::vector<StringId> mDINames;
      CIEC_ANY_MAGNITUDE_VARIANT var_OUT;
      COutDataConnection<CIEC_ANY_MAGNITUDE_VARIANT> conn_OUT;

    public:
      GEN_ADD(const StringId paInstanceNameId, CFBContainer &paContainer);
      ~GEN_ADD() override = default;
  };
} // namespace forte::iec61131::arithmetic
