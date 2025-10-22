/*******************************************************************************
 * Copyright (c) 2014, 2023 Profactor GmbH, fortiss GmbH
 *                          Johannes Kepler University
 *                          Martin Erich Jobst
 *
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
 *    Martin Jobst - add generic readInputData and writeOutputData
 *******************************************************************************/
#include "forte/eclipse4diac/convert/GEN_ARRAY2ARRAY_fbt.h"
#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace forte::eclipse4diac::convert {
  namespace {
    const auto cDataInputNames = std::array{"IN"_STRID};
    const auto cDataOutputNames = std::array{"OUT"_STRID};
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
  } // namespace

  DEFINE_GENERIC_FIRMWARE_FB(GEN_ARRAY2ARRAY, "eclipse4diac::convert::GEN_ARRAY2ARRAY"_STRID)

  GEN_ARRAY2ARRAY::GEN_ARRAY2ARRAY(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_OUT(*this, 0, var_IN) {
  }

  void GEN_ARRAY2ARRAY::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID: sendOutputEvent(scmEventCNFID, paECET); break;
    }
  }

  void GEN_ARRAY2ARRAY::readInputData(TEventID) {
    readData(0, var_IN, conn_IN);
  }

  void GEN_ARRAY2ARRAY::writeOutputData(TEventID) {
    writeData(1 + 0, var_IN, conn_OUT);
  }

  bool GEN_ARRAY2ARRAY::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
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
    TForteUInt16 arrayLength = static_cast<TForteUInt16>(util::strtoul(dNumberPos, nullptr, 10));
    auto valueTypeID = StringId::lookup(++dTypePos);

    if (arrayLength == 0) {
      return false;
    }

    // create data input type
    var_IN.setup(arrayLength, valueTypeID);

    conn_OUT.getValue().setup(arrayLength, valueTypeID);

    // create the interface Specification
    paInterfaceSpec.mEINames = cEventInputNames;
    paInterfaceSpec.mEONames = cEventOutputNames;
    paInterfaceSpec.mDINames = cDataInputNames;
    paInterfaceSpec.mDONames = cDataOutputNames;
    return true;
  }

  CEventConnection *GEN_ARRAY2ARRAY::getEOConUnchecked(TPortId paEONum) {
    return (paEONum == 0) ? &conn_CNF : nullptr;
  }

  CIEC_ANY *GEN_ARRAY2ARRAY::getDI(size_t paIndex) {
    return (paIndex == 0) ? &var_IN : nullptr;
  }

  CIEC_ANY *GEN_ARRAY2ARRAY::getDO(size_t paIndex) {
    return (paIndex == 0) ? &var_IN : nullptr;
  }

  CDataConnection **GEN_ARRAY2ARRAY::getDIConUnchecked(const TPortId paIndex) {
    return (paIndex == 0) ? &conn_IN : nullptr;
  }

  CDataConnection *GEN_ARRAY2ARRAY::getDOConUnchecked(TPortId paDONum) {
    return (paDONum == 0) ? &conn_OUT : nullptr;
  }
} // namespace forte::eclipse4diac::convert
