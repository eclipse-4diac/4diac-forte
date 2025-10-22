/*******************************************************************************
 * Copyright (c) 2014 - 2015 Profactor GmbH, fortiss GmbH
 *                      2018 Johannes Kepler University
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *   Martin Jobst
 *     - refactor for ANY variant
 *     - add generic readInputData and writeOutputData
 *******************************************************************************/
#include "forte/eclipse4diac/convert/GEN_VALUES2ARRAY_fbt.h"
#include <memory>
#include "forte/arch/forte_printer.h"
#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace forte::eclipse4diac::convert {
  namespace {
    const auto cDataOutputNames = std::array{"OUT"_STRID};
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
  } // namespace

  DEFINE_GENERIC_FIRMWARE_FB(GEN_VALUES2ARRAY, "eclipse4diac::convert::GEN_VALUES2ARRAY"_STRID)

  GEN_VALUES2ARRAY::GEN_VALUES2ARRAY(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_OUT(*this, 0, var_OUT) {
  }

  void GEN_VALUES2ARRAY::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        for (unsigned int input_index = 0; input_index < getFBInterfaceSpec().getNumDIs(); input_index++) {
          // copy input values to array
          var_OUT[input_index].setValue(*getDI(input_index));
        }
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void GEN_VALUES2ARRAY::readInputData(TEventID) {
    for (TPortId i = 0; i < getFBInterfaceSpec().getNumDIs(); ++i) {
      readData(i, mGenDIs[i], mGenDIConns[i]);
    }
  }

  void GEN_VALUES2ARRAY::writeOutputData(TEventID) {
    writeData(getFBInterfaceSpec().getNumDIs() + 0, var_OUT, conn_OUT);
  }

  bool GEN_VALUES2ARRAY::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    const char *dNumberPos = strchr(paConfigString, '_');

    if (dNumberPos == nullptr) {
      return false;
    }
    ++dNumberPos;
    // get position of a second underscore
    const char *dTypePos = strchr(dNumberPos, '_');

    if (dTypePos == nullptr) {
      return false;
    }
    // there is a number and a data type of inputs within the typename
    unsigned int numDIs = static_cast<unsigned int>(util::strtoul(dNumberPos, nullptr, 10));
    auto arrayValueTypeId = StringId::lookup(++dTypePos);

    if (numDIs < 2) {
      return false;
    }
    // create the data inputs
    generateGenericInterfacePointNameArray("IN_", mDataInputNames, numDIs);

    // create data output type
    var_OUT.setup(numDIs, arrayValueTypeId);

    // create the interface Specification
    paInterfaceSpec.mEINames = cEventInputNames;
    paInterfaceSpec.mEONames = cEventOutputNames;
    paInterfaceSpec.mDINames = mDataInputNames;
    paInterfaceSpec.mDONames = cDataOutputNames;
    return true;
  }

  void GEN_VALUES2ARRAY::createGenInputData() {
    mGenDIs = std::make_unique<CIEC_ANY_VARIANT[]>(getFBInterfaceSpec().getNumDIs());
  }

  CEventConnection *GEN_VALUES2ARRAY::getEOConUnchecked(TPortId paEONum) {
    return (paEONum == 0) ? &conn_CNF : nullptr;
  }

  CIEC_ANY *GEN_VALUES2ARRAY::getDI(size_t paIndex) {
    return &mGenDIs[paIndex];
  }

  CIEC_ANY *GEN_VALUES2ARRAY::getDO(size_t paIndex) {
    return (paIndex == 0) ? &var_OUT : nullptr;
  }

  CDataConnection *GEN_VALUES2ARRAY::getDOConUnchecked(TPortId paDONum) {
    return (paDONum == 0) ? &conn_OUT : nullptr;
  }
} // namespace forte::eclipse4diac::convert
