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
#include "forte/iec61131/bitwiseOperators/genbitbase_fbt.h"
#include <memory>
#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace forte::iec61131::bitwiseOperators {
  namespace {
    const auto cDataOutputNames = std::array{"OUT"_STRID};
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
  } // namespace

  CGenBitBase::CGenBitBase(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_OUT(*this, 0, var_OUT) {
  }

  void CGenBitBase::readInputData(TEventID) {
    for (TPortId i = 0; i < getGenDINums(); ++i) {
      readData(i, mGenDIs[i], mGenDIConns[i]);
    }
  }

  void CGenBitBase::writeOutputData(TEventID) {
    writeData(getFBInterfaceSpec().getNumDIs() + 0, var_OUT, conn_OUT);
  }

  bool CGenBitBase::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    const char *pcPos = strrchr(paConfigString, '_');
    size_t numDIs = 0;
    if (nullptr != pcPos) {
      pcPos++;
      // we have an underscore and it is the first underscore after AND
      numDIs = static_cast<TPortId>(util::strtoul(pcPos, nullptr, 10));
      DEVLOG_DEBUG("DIs: %d;\n", numDIs);
    } else {
      return false;
    }

    if (numDIs < 2) {
      return false;
    }

    // now the number of needed eventInputs and dataOutputs are available in the integer array
    // create the eventInputs
    if (numDIs < scmMaxInterfaceEvents) {

      // create the data inputs
      generateGenericInterfacePointNameArray("IN", mDataInputNames, numDIs);

      // setup the interface Specification
      paInterfaceSpec.mEINames = cEventInputNames;
      paInterfaceSpec.mEONames = cEventOutputNames;
      paInterfaceSpec.mDINames = mDataInputNames;
      paInterfaceSpec.mDONames = cDataOutputNames;
      return true;
    }
    return false;
  }

  CIEC_ANY *CGenBitBase::getDI(size_t paDINum) {
    return &mGenDIs[paDINum];
  }

  CIEC_ANY *CGenBitBase::getDO(size_t paDONum) {
    return (paDONum == 0) ? &var_OUT : nullptr;
  }

  CEventConnection *CGenBitBase::getEOConUnchecked(TPortId paEONum) {
    return (paEONum == 0) ? &conn_CNF : nullptr;
  }

  CDataConnection *CGenBitBase::getDOConUnchecked(TPortId paDONum) {
    return (paDONum == 0) ? &conn_OUT : nullptr;
  }

  void CGenBitBase::createGenInputData() {
    mGenDIs = std::make_unique<CIEC_ANY_BIT_VARIANT[]>(getFBInterfaceSpec().getNumDIs());
  }
} // namespace forte::iec61131::bitwiseOperators
