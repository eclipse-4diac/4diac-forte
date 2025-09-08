/*************************************************************************
 *** Copyright (c) 2012, 2022 TU Wien ACIN, HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: F_SEL_E_2
 *** Description: selection (event-based selection)
 *** Version:
 ***     1.0: 2012-03-25/Monika Wenger - TU Wien ACIN -
 ***     1.1: 2022-08-04/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#include "forte/eclipse4diac/utils/selection/F_SEL_E_2_fbt.h"

using namespace forte::core::literals;

using namespace forte::eclipse4diac::utils::selection;

DEFINE_FIRMWARE_FB(FORTE_F_SEL_E_2, "F_SEL_E_2"_STRID)

namespace {
  const auto cDataInputNames = std::array{"IN0"_STRID, "IN1"_STRID};
  const auto cDataOutputNames = std::array{"OUT"_STRID};
  const auto cEventInputNames = std::array{"REQ0"_STRID, "REQ1"_STRID};
  const auto cEventInputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
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

FORTE_F_SEL_E_2::FORTE_F_SEL_E_2(const forte::core::StringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
    conn_CNF(*this, 0),
    conn_IN0(nullptr),
    conn_IN1(nullptr),
    conn_OUT(*this, 0, var_OUT) {};

void FORTE_F_SEL_E_2::setInitialValues() {
  var_IN0.reset();
  var_IN1.reset();
  var_OUT.reset();
}

void FORTE_F_SEL_E_2::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQ0ID:
      var_OUT = var_IN0;
      sendOutputEvent(scmEventCNFID, paECET);
      break;
    case scmEventREQ1ID:
      var_OUT = var_IN1;
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_SEL_E_2::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQ0ID: {

      readData(0, var_IN0, conn_IN0);
      break;
    }
    case scmEventREQ1ID: {

      readData(1, var_IN1, conn_IN1);
      break;
    }
    default: break;
  }
}

void FORTE_F_SEL_E_2::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {

      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_SEL_E_2::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN0;
    case 1: return &var_IN1;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_SEL_E_2::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_SEL_E_2::getDIO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_F_SEL_E_2::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_SEL_E_2::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN0;
    case 1: return &conn_IN1;
  }
  return nullptr;
}

CDataConnection *FORTE_F_SEL_E_2::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CInOutDataConnection **FORTE_F_SEL_E_2::getDIOInConUnchecked(TPortId) {
  return nullptr;
}

CInOutDataConnection *FORTE_F_SEL_E_2::getDIOOutConUnchecked(TPortId) {
  return nullptr;
}
