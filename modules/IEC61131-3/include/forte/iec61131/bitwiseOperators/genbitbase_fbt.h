/*******************************************************************************
 * Copyright (c)2018 Johannes Kepler University
 *               2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *     - add generic readInputData and writeOutputData
 *******************************************************************************/
#pragma once

#include "forte/genfb.h"
#include "forte/datatypes/forte_any_bit_variant.h"

#include <memory>

namespace forte::iec61131::bitwiseOperators {
  class CGenBitBase : public CGenFunctionBlock<CFunctionBlock> {

    protected:
      CGenBitBase(const StringId paInstanceNameId, CFBContainer &paContainer);
      ~CGenBitBase() override = default;

      CIEC_ANY_BIT_VARIANT &var_IN(size_t paIndex) {
        return mGenDIs[paIndex];
      }

      size_t getGenEOOffset() override {
        return 1;
      }

      size_t getGenDOOffset() override {
        return 1;
      }

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId paDONum) override;
      void createGenInputData() override;

      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      CIEC_ANY_BIT_VARIANT var_OUT;

    private:
      std::vector<StringId> mDataInputNames;

      void readInputData(TEventID paEI) override;
      void writeOutputData(TEventID paEO) override;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      std::unique_ptr<CIEC_ANY_BIT_VARIANT[]> mGenDIs;

      CEventConnection conn_CNF;

      COutDataConnection<CIEC_ANY_BIT_VARIANT> conn_OUT;
  };
} // namespace forte::iec61131::bitwiseOperators
