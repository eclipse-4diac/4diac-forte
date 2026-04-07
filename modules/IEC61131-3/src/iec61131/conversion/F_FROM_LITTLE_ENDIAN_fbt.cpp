/*************************************************************************
 *** Copyright (c) 2026 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202604152003!
 ***
 *** Name: F_FROM_LITTLE_ENDIAN
 *** Description: Convert from Little Endian
 *** Version:
 ***     1.0: 2026-01-31/Franz Höpfinger - HR Agrartechnik GmbH - conform to IEC 61131-3 specification for this Block
 *************************************************************************/

#include "forte/iec61131/conversion/F_FROM_LITTLE_ENDIAN_fbt.h"

#include "forte/datatypes/forte_any_variant.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131_functions/func_FROM_LITTLE_ENDIAN.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61131::conversion {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"IN"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_F_FROM_LITTLE_ENDIAN, "iec61131::conversion::F_FROM_LITTLE_ENDIAN"_STRID, TypeHash)

  FORTE_F_FROM_LITTLE_ENDIAN::FORTE_F_FROM_LITTLE_ENDIAN(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      var_IN(CIEC_ANY_VARIANT()),
      var_OUT(CIEC_ANY_VARIANT()),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_OUT(*this, 0, var_OUT) {
  }

  void FORTE_F_FROM_LITTLE_ENDIAN::setInitialValues() {
    CSimpleFB::setInitialValues();
    var_IN = CIEC_ANY_VARIANT();
    var_OUT = CIEC_ANY_VARIANT();
  }

  void FORTE_F_FROM_LITTLE_ENDIAN::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID: enterStateREQ(paECET); break;
      default: break;
    }
  }

  void FORTE_F_FROM_LITTLE_ENDIAN::enterStateREQ(CEventChainExecutionThread *const paECET) {
    alg_REQ();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_F_FROM_LITTLE_ENDIAN::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        break;
      }
      default: break;
    }
  }

  void FORTE_F_FROM_LITTLE_ENDIAN::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(1, var_OUT, conn_OUT);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_F_FROM_LITTLE_ENDIAN::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_FROM_LITTLE_ENDIAN::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_FROM_LITTLE_ENDIAN::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_FROM_LITTLE_ENDIAN::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_FROM_LITTLE_ENDIAN::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_FROM_LITTLE_ENDIAN::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_F_FROM_LITTLE_ENDIAN::alg_REQ(void) {
    var_OUT = var_IN;
    if constexpr (std::endian::native == std::endian::little) {
      std::visit(
          []<typename T>(T &&paOUT) {
            if constexpr (std::is_same_v<std::remove_reference_t<T>, CIEC_ANY_UNIQUE_PTR<CIEC_ARRAY>> ||
                          std::is_same_v<std::remove_reference_t<T>, CIEC_ANY_UNIQUE_PTR<CIEC_STRUCT>>) {
              paOUT = func_FROM_LITTLE_ENDIAN(paOUT);
            } else {
              paOUT = func_FROM_LITTLE_ENDIAN(paOUT);
            }
          },
          static_cast<CIEC_ANY_VARIANT::variant &>(var_OUT));
    }
  }
} // namespace forte::iec61131::conversion
