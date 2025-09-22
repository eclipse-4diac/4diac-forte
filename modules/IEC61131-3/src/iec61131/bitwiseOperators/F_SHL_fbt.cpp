/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "forte/iec61131/bitwiseOperators/F_SHL_fbt.h"

using namespace forte::literals;

namespace forte::iec61131::bitwiseOperators {
  namespace {
    const auto cDataInputNames = std::array{"IN"_STRID, "N"_STRID};

    const auto cDataOutputNames = std::array{"OUT"_STRID};

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID};

    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID};

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

  DEFINE_FIRMWARE_FB(FORTE_F_SHL, "iec61131::bitwiseOperators::F_SHL"_STRID)

  FORTE_F_SHL::FORTE_F_SHL(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_IN(CIEC_ANY_BIT_VARIANT()),
      var_N(CIEC_ANY_INT_VARIANT()),
      var_OUT(CIEC_ANY_BIT_VARIANT()),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_N(nullptr),
      conn_OUT(*this, 0, var_OUT) {};

  void FORTE_F_SHL::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        var_OUT = std::visit(
            [](auto &&paIN, auto &&paN) -> CIEC_ANY_BIT_VARIANT {
              using T = std::decay_t<decltype(paIN)>;
              if constexpr (!std::is_same<T, CIEC_BOOL>::value) {
                return func_SHL(paIN, paN);
              }
              DEVLOG_ERROR("Shifting left incompatible types %s and %s\n", paIN.getTypeNameID().data(),
                           paN.getTypeNameID().data());
              return CIEC_ANY_BIT_VARIANT();
            },
            static_cast<CIEC_ANY_BIT_VARIANT::variant &>(var_IN), static_cast<CIEC_ANY_INT_VARIANT::variant &>(var_N));
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_F_SHL::readInputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        readData(1, var_N, conn_N);
        break;
      }
      default: break;
    }
  }

  void FORTE_F_SHL::writeOutputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_F_SHL::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN;
      case 1: return &var_N;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_SHL::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_SHL::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_SHL::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
      case 1: return &conn_N;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_SHL::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

  void FORTE_F_SHL::setInitialValues() {
    var_IN.reset();
    var_N.reset();
    var_OUT.reset();
  }

} // namespace forte::iec61131::bitwiseOperators
