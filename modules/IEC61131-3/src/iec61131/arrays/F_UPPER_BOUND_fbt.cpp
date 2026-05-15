/*************************************************************************
 *** Copyright (c) 2026 Martin Melik Merkumians, Demmler Andreas Fahrzeugbau
 ***
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** Contributors:
 ***     Martin Melik Merkumians - Initial implementation
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.0.202603111726!
 ***
 *** Name: F_UPPER_BOUND
 *** Description: Returns the upper bound of the dimension of the given array
 *** Version:
 ***     1.0: 2026-01-23/Franz Höpfinger - Demmler Andreas Fahrzeugbau - Initial Implementation
 *************************************************************************/

#include "forte/iec61131/arrays/F_UPPER_BOUND_fbt.h"

#include "forte/datatypes/forte_udint.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131_functions/func_UPPER_BOUND.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61131::arrays {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"DIM"_STRID, "ARR"_STRID};
    const auto cDataOutputNames = std::array{"OUT"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_F_UPPER_BOUND, "iec61131::arrays::F_UPPER_BOUND"_STRID, TypeHash)

  FORTE_F_UPPER_BOUND::FORTE_F_UPPER_BOUND(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_DIM(1_UDINT),
      var_ARR(CIEC_ANY_DERIVED_VARIANT()),
      var_OUT(CIEC_ANY_INT_VARIANT()),
      conn_CNF(*this, 0),
      conn_DIM(nullptr),
      conn_ARR(nullptr),
      conn_OUT(*this, 0, var_OUT) {};

  void FORTE_F_UPPER_BOUND::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_DIM = 1_UDINT;
    var_ARR = CIEC_ANY_DERIVED_VARIANT();
    var_OUT = CIEC_ANY_INT_VARIANT();
  }

  void FORTE_F_UPPER_BOUND::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        std::visit(
            [](auto &&arr, auto &&dim, auto &&out) {
              using T = std::decay_t<decltype(arr)>;
              using U = std::decay_t<decltype(dim)>;
              using R = std::decay_t<decltype(out)>;

              if constexpr (std::is_base_of_v<CIEC_ARRAY, typename T::element_type>) {
                out = func_UPPER_BOUND<R, U>(*arr, dim);
              } else {
                DEVLOG_ERROR("Provided variable is not an array type");
                out = R(0);
              }
            },
            static_cast<const CIEC_ANY_DERIVED_VARIANT::variant &>(var_ARR),
            static_cast<const CIEC_ANY_INT_VARIANT::variant &>(var_DIM),
            static_cast<CIEC_ANY_INT_VARIANT::variant &>(var_OUT));

        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_F_UPPER_BOUND::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_DIM, conn_DIM);
        readData(1, var_ARR, conn_ARR);
        break;
      }
      default: break;
    }
  }

  void FORTE_F_UPPER_BOUND::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(2, var_OUT, conn_OUT);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_F_UPPER_BOUND::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_DIM;
      case 1: return &var_ARR;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_UPPER_BOUND::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_UPPER_BOUND::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_UPPER_BOUND::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_DIM;
      case 1: return &conn_ARR;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_UPPER_BOUND::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

} // namespace forte::iec61131::arrays
