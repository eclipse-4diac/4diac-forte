/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH
 *                      2018 Johannes Kepler University
 *               2023 Martin Erich Jobst
 *               2026 HR Agrartechnik GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *   Martin Jobst
 *     - refactor for ANY variant
 *     - add generic readInputData and writeOutputData
 *   Franz Höpfinger
 *     - factored out of GEN_ADD into a shared generic arithmetic FB base
 *******************************************************************************/

#pragma once

#include "forte/genfb.h"
#include "forte/util/string_utils.h"

#include <memory>

namespace forte::iec61131::arithmetic {
  template<typename TVariant>
  class CGenArithBase : public CGenFunctionBlock<CFunctionBlock> {

    protected:
      CGenArithBase(const StringId paInstanceNameId, CFBContainer &paContainer);
      ~CGenArithBase() override = default;

      TVariant &var_IN(size_t paIndex) {
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

      TVariant var_OUT;

    private:
      std::vector<StringId> mDataInputNames;

      void readInputData(TEventID paEI) override;
      void writeOutputData(TEventID paEO) override;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      std::unique_ptr<TVariant[]> mGenDIs;

      CEventConnection conn_CNF;

      COutDataConnection<TVariant> conn_OUT;
  };

  template<typename TVariant>
  CGenArithBase<TVariant>::CGenArithBase(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_OUT(*this, 0, var_OUT) {
  }

  template<typename TVariant>
  void CGenArithBase<TVariant>::readInputData(TEventID) {
    for (TPortId i = 0; i < getFBInterfaceSpec().getNumDIs(); ++i) {
      readData(i, mGenDIs[i], mGenDIConns[i]);
    }
  }

  template<typename TVariant>
  void CGenArithBase<TVariant>::writeOutputData(TEventID) {
    writeData(getFBInterfaceSpec().getNumDIs() + 0, var_OUT, conn_OUT);
  }

  template<typename TVariant>
  bool CGenArithBase<TVariant>::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    using namespace forte::literals;
    static const forte::StringId cEventInputNames[] = {"REQ"_STRID};
    static const forte::StringId cEventOutputNames[] = {"CNF"_STRID};
    static const forte::StringId cDataOutputNames[] = {"OUT"_STRID};

    const char *pcPos = strrchr(paConfigString, '_');
    if (pcPos == nullptr) {
      return false;
    }

    pcPos++;
    // we have an underscore and it is the first underscore after the FB type name
    size_t numDIs = static_cast<size_t>(util::strtoul(pcPos, nullptr, 10));
    DEVLOG_DEBUG("DIs: %zu;\n", numDIs);

    if (numDIs < 2 || numDIs >= scmMaxInterfaceEvents) {
      return false;
    }

    // now the number of needed data inputs is available
    // create the data inputs
    generateGenericInterfacePointNameArray("IN", mDataInputNames, numDIs);

    paInterfaceSpec.mEINames = cEventInputNames;
    paInterfaceSpec.mEONames = cEventOutputNames;
    paInterfaceSpec.mDINames = mDataInputNames;
    paInterfaceSpec.mDONames = cDataOutputNames;

    return true;
  }

  template<typename TVariant>
  CIEC_ANY *CGenArithBase<TVariant>::getDI(size_t paDINum) {
    return &mGenDIs[paDINum];
  }

  template<typename TVariant>
  CIEC_ANY *CGenArithBase<TVariant>::getDO(size_t paDONum) {
    return (paDONum == 0) ? &var_OUT : nullptr;
  }

  template<typename TVariant>
  CEventConnection *CGenArithBase<TVariant>::getEOConUnchecked(TPortId paEONum) {
    return (paEONum == 0) ? &conn_CNF : nullptr;
  }

  template<typename TVariant>
  CDataConnection *CGenArithBase<TVariant>::getDOConUnchecked(TPortId paDONum) {
    return (paDONum == 0) ? &conn_OUT : nullptr;
  }

  template<typename TVariant>
  void CGenArithBase<TVariant>::createGenInputData() {
    mGenDIs = std::make_unique<TVariant[]>(getFBInterfaceSpec().getNumDIs());
  }
} // namespace forte::iec61131::arithmetic
