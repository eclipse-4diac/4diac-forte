/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH, fortiss GmbH
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
#include "forte/eclipse4diac/convert/GEN_ARRAY2VALUES_fbt.h"
#include <memory>
#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace forte::eclipse4diac::convert {
  namespace {
    const auto cDataInputNames = std::array{"IN"_STRID};
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
  } // namespace

  DEFINE_GENERIC_FIRMWARE_FB(GEN_ARRAY2VALUES, "eclipse4diac::convert::GEN_ARRAY2VALUES"_STRID)

  GEN_ARRAY2VALUES::GEN_ARRAY2VALUES(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_IN(nullptr) {
  }

  void GEN_ARRAY2VALUES::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:

        for (size_t output_index = 0; output_index < getFBInterfaceSpec().getNumDOs(); output_index++) {
          // copy input values to array
          getDO(static_cast<unsigned int>(output_index))->setValue(var_IN[output_index]);
        }

        sendOutputEvent(scmEventCNFID, paECET);

        break;
    }
  }

  void GEN_ARRAY2VALUES::readInputData(TEventID) {
    readData(0, var_IN, conn_IN);
  }

  void GEN_ARRAY2VALUES::writeOutputData(TEventID) {
    for (TPortId i = 0; i < getFBInterfaceSpec().getNumDOs(); ++i) {
      writeData(1 + i, mGenDOs[i], mGenDOConns[i]);
    }
  }

  bool GEN_ARRAY2VALUES::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
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
    size_t numDOs = static_cast<size_t>(util::strtoul(dNumberPos, nullptr, 10));
    StringId arrayValueTypeId = StringId::lookup(++dTypePos);

    if (numDOs < 2) {
      return false;
    }

    // create the data outputs
    generateGenericInterfacePointNameArray("OUT_", mDataOutputNames, numDOs);

    // configure array input
    var_IN.setup(numDOs, arrayValueTypeId);

    // create the interface Specification
    paInterfaceSpec.mEINames = cEventInputNames;
    paInterfaceSpec.mEONames = cEventOutputNames;
    paInterfaceSpec.mDINames = cDataInputNames;
    paInterfaceSpec.mDONames = mDataOutputNames;
    return true;
  }

  void GEN_ARRAY2VALUES::createGenOutputData() {
    mGenDOs = std::make_unique<CIEC_ANY_VARIANT[]>(getFBInterfaceSpec().getNumDOs());
  }

  CEventConnection *GEN_ARRAY2VALUES::getEOConUnchecked(TPortId paEONum) {
    return (paEONum == 0) ? &conn_CNF : nullptr;
  }

  CIEC_ANY *GEN_ARRAY2VALUES::getDI(size_t paIndex) {
    return (paIndex == 0) ? &var_IN : nullptr;
  }

  CIEC_ANY *GEN_ARRAY2VALUES::getDO(size_t paIndex) {
    return &mGenDOs[paIndex];
  }

  CDataConnection **GEN_ARRAY2VALUES::getDIConUnchecked(const TPortId paIndex) {
    return (paIndex == 0) ? &conn_IN : nullptr;
  }
} // namespace forte::eclipse4diac::convert
