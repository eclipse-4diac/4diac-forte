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
 *** Name: F_LEN_ARRAY
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2026-03-25/Martin Melik Merkumians -  -
 *************************************************************************/

#include "forte/eclipse4diac/utils/arrays/F_LEN_ARRAY_fbt.h"

#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_ulint.h"
#include "forte/datatypes/forte_lint.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131_functions/func_UPPER_BOUND.h"
#include "forte/iec61131_functions/func_LOWER_BOUND.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::arrays {
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

  DEFINE_FIRMWARE_FB(FORTE_F_LEN_ARRAY, "eclipse4diac::utils::arrays::F_LEN_ARRAY"_STRID, TypeHash)

  FORTE_F_LEN_ARRAY::FORTE_F_LEN_ARRAY(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_DIM(1_UDINT),
      var_ARR(CIEC_ANY_DERIVED_VARIANT()),
      var_OUT(CIEC_ANY_INT_VARIANT()),
      conn_CNF(*this, 0),
      conn_DIM(nullptr),
      conn_ARR(nullptr),
      conn_OUT(*this, 0, var_OUT) {};

  void FORTE_F_LEN_ARRAY::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_DIM = 1_UDINT;
    var_ARR = CIEC_ANY_DERIVED_VARIANT();
    var_OUT = CIEC_ANY_INT_VARIANT();
  }

  void FORTE_F_LEN_ARRAY::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        std::visit(
            [](auto &&arr, auto &&dim, auto &&out) {
              using T = std::decay_t<decltype(arr)>;
              using U = std::decay_t<decltype(dim)>;
              using R = std::decay_t<decltype(out)>;

              if constexpr (!std::is_base_of_v<CIEC_ARRAY, typename T::element_type>) {
                DEVLOG_ERROR("Provided variable is not an array type");
                out = R(0);
                return;
              } else {
                const auto lower = static_cast<CIEC_LINT::TValueType>(func_LOWER_BOUND<CIEC_LINT, U>(*arr, dim));
                const auto upper = static_cast<CIEC_LINT::TValueType>(func_UPPER_BOUND<CIEC_LINT, U>(*arr, dim));

                if (lower > upper) {
                  DEVLOG_ERROR("F_LEN_ARRAY: lower bound > upper bound, this should never occur in a sane program\n");
                  std::abort();
                }

                // Unsigned subtraction safely handles all sign combinations via modular wraparound
                out = R(static_cast<CIEC_ULINT::TValueType>(upper) - static_cast<CIEC_ULINT::TValueType>(lower) + 1U);
              }
            },
            static_cast<const CIEC_ANY_DERIVED_VARIANT::variant &>(var_ARR),
            static_cast<const CIEC_ANY_INT_VARIANT::variant &>(var_DIM),
            static_cast<CIEC_ANY_INT_VARIANT::variant &>(var_OUT));

        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_F_LEN_ARRAY::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_DIM, conn_DIM);
        readData(1, var_ARR, conn_ARR);
        break;
      }
      default: break;
    }
  }

  void FORTE_F_LEN_ARRAY::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(2, var_OUT, conn_OUT);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_F_LEN_ARRAY::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_DIM;
      case 1: return &var_ARR;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_LEN_ARRAY::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_LEN_ARRAY::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_LEN_ARRAY::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_DIM;
      case 1: return &conn_ARR;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_LEN_ARRAY::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

} // namespace forte::eclipse4diac::utils::arrays
