/*************************************************************************
 *** Copyright (c) 2026 Andreas Demmler Fahrzeugbau
 ***
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202604012115!
 ***
 *** Name: PROVIDE_ARR_0002_BYTE
 *** Description: FB to provide a BYTE array of size 2 (generic FB)
 *** Version:
 ***     1.1: 2026-03-21/Moritz Ortmeier - Andreas Demmler Fahrzeugbau - changed naming and removed length output
 ***     1.0: 2026-01-22/Franz Höpfinger - Andreas Demmler Fahrzeugbau -
 *************************************************************************/

#include "forte/eclipse4diac/convert/providers/GEN_PROVIDE_ARR_fbt.h"
#include "forte/util/string_utils.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::convert::providers {
  namespace {
    const auto cEventInputNames = std::array{"INIT"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID};
    const auto cDataInOutNames = std::array{"D1"_STRID};
  } // namespace

  DEFINE_GENERIC_FIRMWARE_FB(GEN_PROVIDE_ARR, "eclipse4diac::convert::providers::GEN_PROVIDE_ARR"_STRID)

  GEN_PROVIDE_ARR::GEN_PROVIDE_ARR(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      conn_INITO(*this, 0),
      conn_inout_in_D1(nullptr),
      conn_inout_out_D1(*this, 0, var_D1) {
  };


  void GEN_PROVIDE_ARR::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch(paEIID) {
      case scmEventINITID:
        sendOutputEvent(scmEventINITOID, paECET);
        break;
    }
  }

  void GEN_PROVIDE_ARR::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventINITID: {
        readData(0, var_D1, &conn_inout_out_D1);
        break;
      }
      default:
        break;
    }
  }

  void GEN_PROVIDE_ARR::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventINITOID: {
        writeData(0, var_D1, conn_inout_out_D1);
        break;
      }
      default:
        break;
    }
  }

  bool GEN_PROVIDE_ARR::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    const char *dNumberPos = strchr(paConfigString, '_');

    if (dNumberPos == nullptr) {
      return false;
    }

    // Skip the second part of the name (e.g. for INIT_ARR_...)
    ++dNumberPos;
    dNumberPos = strchr(dNumberPos, '_');

    if (dNumberPos == nullptr) {
      return false;
    }

    ++dNumberPos;
    // get position of the underscore separating number and type
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
    var_D1.setup(arrayLength, valueTypeID);

    conn_inout_out_D1.getDefiningValue().setup(arrayLength, valueTypeID);

    // create the interface Specification
    paInterfaceSpec.mEINames = cEventInputNames;
    paInterfaceSpec.mEITypeNames = cEventInputTypeIds;
    paInterfaceSpec.mEONames = cEventOutputNames;
    paInterfaceSpec.mEOTypeNames = cEventOutputTypeIds;
    paInterfaceSpec.mDIONames = cDataInOutNames;
    return true;
  }

  CIEC_ANY *GEN_PROVIDE_ARR::getDI(size_t) {
    return nullptr;
  }

  CIEC_ANY *GEN_PROVIDE_ARR::getDO(size_t) {
    return nullptr;
  }

  CIEC_ANY *GEN_PROVIDE_ARR::getDIO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_D1;
    }
    return nullptr;
  }

  CEventConnection *GEN_PROVIDE_ARR::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_INITO;
    }
    return nullptr;
  }

  CDataConnection **GEN_PROVIDE_ARR::getDIConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *GEN_PROVIDE_ARR::getDOConUnchecked(TPortId) {
    return nullptr;
  }

  CInOutDataConnection **GEN_PROVIDE_ARR::getDIOInConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_inout_in_D1;
    }
    return nullptr;
  }

  CInOutDataConnection *GEN_PROVIDE_ARR::getDIOOutConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_inout_out_D1;
    }
    return nullptr;
  }

}