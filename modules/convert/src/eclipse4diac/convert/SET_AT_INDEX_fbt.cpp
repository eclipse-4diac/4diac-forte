/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "forte/eclipse4diac/convert/SET_AT_INDEX_fbt.h"

using namespace forte::literals;

namespace forte::eclipse4diac::convert {
  namespace {
    const auto cDataInputNames = std::array{"IN_ARRAY"_STRID, "INDEX"_STRID, "VALUE"_STRID};

    const auto cDataOutputNames = std::array{"QO"_STRID, "OUT_ARRAY"_STRID};

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID};

    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = cEventInputTypeIds,
        .mEONames = cEventOutputNames,
        .mEOTypeNames = cEventOutputTypeIds,
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_SET_AT_INDEX, "eclipse4diac::convert::SET_AT_INDEX"_STRID)

  FORTE_SET_AT_INDEX::FORTE_SET_AT_INDEX(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_IN_ARRAY(CIEC_ANY_VARIANT()),
      var_INDEX(0_UINT),
      var_VALUE(CIEC_ANY_VARIANT()),
      var_QO(false_BOOL),
      var_OUT_ARRAY(CIEC_ANY_VARIANT()),
      conn_CNF(*this, 0),
      conn_IN_ARRAY(nullptr),
      conn_INDEX(nullptr),
      conn_VALUE(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_OUT_ARRAY(*this, 1, var_OUT_ARRAY) {};

  void FORTE_SET_AT_INDEX::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        if (std::holds_alternative<CIEC_ANY_UNIQUE_PTR<CIEC_ARRAY>>(var_IN_ARRAY)) {
          auto &inArray = std::get<CIEC_ANY_UNIQUE_PTR<CIEC_ARRAY>>(var_IN_ARRAY);
          // check if data types match and index is within bounds
          if (inArray->getElementDataTypeID() == var_VALUE.unwrap().getDataTypeID() &&
              static_cast<CIEC_UINT::TValueType>(var_INDEX) >= inArray->getLowerBound() &&
              static_cast<CIEC_UINT::TValueType>(var_INDEX) <= inArray->getUpperBound()) {
            var_OUT_ARRAY = var_IN_ARRAY;
            auto &outArray = std::get<CIEC_ANY_UNIQUE_PTR<CIEC_ARRAY>>(var_OUT_ARRAY);
            outArray->at(var_INDEX).setValue(var_VALUE.unwrap());
            var_QO = true_BOOL;
          } else {
            var_QO = false_BOOL;
          }
        } else {
          var_QO = false_BOOL;
        }
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_SET_AT_INDEX::readInputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN_ARRAY, conn_IN_ARRAY);
        readData(1, var_INDEX, conn_INDEX);
        readData(2, var_VALUE, conn_VALUE);
        break;
      }
      default: break;
    }
  }

  void FORTE_SET_AT_INDEX::writeOutputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_OUT_ARRAY, conn_OUT_ARRAY);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_SET_AT_INDEX::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN_ARRAY;
      case 1: return &var_INDEX;
      case 2: return &var_VALUE;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_SET_AT_INDEX::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_OUT_ARRAY;
    }
    return nullptr;
  }

  CEventConnection *FORTE_SET_AT_INDEX::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_SET_AT_INDEX::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN_ARRAY;
      case 1: return &conn_INDEX;
      case 2: return &conn_VALUE;
    }
    return nullptr;
  }

  CDataConnection *FORTE_SET_AT_INDEX::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_OUT_ARRAY;
    }
    return nullptr;
  }

  void FORTE_SET_AT_INDEX::setInitialValues() {
    var_IN_ARRAY.reset();
    var_INDEX = 0_UINT;
    var_VALUE.reset();
    var_QO = false_BOOL;
    var_OUT_ARRAY.reset();
  }

} // namespace forte::eclipse4diac::convert
