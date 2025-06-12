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
#include "genbitbase_fbt.h"
#include <memory>
#include "string_utils.h"

USE_STRING_ID(ANY_BIT);
USE_STRING_ID(CNF);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

namespace {
  const auto cDataOutputNames = std::array{STRID(OUT)};
  const auto cEventInputNames = std::array{STRID(REQ)};
  const auto cEventOutputNames = std::array{STRID(CNF)};
} // namespace

CGenBitBase::CGenBitBase(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
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
    numDIs = static_cast<TPortId>(forte::core::util::strtoul(pcPos, nullptr, 10));
    DEVLOG_DEBUG("DIs: %d;\n", paInterfaceSpec.mNumDIs);
  } else {
    return false;
  }

  if (numDIs < 2) {
    return false;
  }

  // now the number of needed eventInputs and dataOutputs are available in the integer array
  // create the eventInputs
  if (numDIs < CFunctionBlock::scmMaxInterfaceEvents) {

    // create the data inputs
    generateGenericInterfacePointNameArray("IN_", mDataInputNames, numDIs);

    // setup the interface Specification
    paInterfaceSpec.mEINames = cEventInputNames;
    paInterfaceSpec.mEONames = cEventOutputNames;
    paInterfaceSpec.mDINames = mDataInputNames;
    paInterfaceSpec.mDONames = cDataOutputNames;
    return true;
  }
  return false;
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
