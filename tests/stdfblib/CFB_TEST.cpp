/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "CFB_TEST.h"

using namespace forte::literals;

namespace forte::test {
  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID};
    const auto cEventInputNames = std::array{"SET"_STRID, "RESET"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID, "CHANGED"_STRID};
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

    const auto cEventConnections = std::to_array<SCFB_FBConnectionData>({
        {{}, "SET"_STRID, "E_MUX_2"_STRID, "EI1"_STRID},
        {{}, "RESET"_STRID, "E_MUX_2"_STRID, "EI2"_STRID},
        {"E_MUX_2"_STRID, "EO"_STRID, "PERMIT_OP"_STRID, "EI"_STRID},
        {"SET_CHANGED"_STRID, "EO1"_STRID, {}, "CNF"_STRID},
        {"SET_CHANGED"_STRID, "EO0"_STRID, "E_SR"_STRID, "S"_STRID},
        {"RESET_CHANGED"_STRID, "EO0"_STRID, {}, "CNF"_STRID},
        {"RESET_CHANGED"_STRID, "EO1"_STRID, "E_SR"_STRID, "R"_STRID},
        {"PERMIT_OP"_STRID, "EO"_STRID, "E_DEMUX_2"_STRID, "EI"_STRID},
        {"E_DEMUX_2"_STRID, "EO1"_STRID, "SET_CHANGED"_STRID, "EI"_STRID},
        {"E_DEMUX_2"_STRID, "EO2"_STRID, "RESET_CHANGED"_STRID, "EI"_STRID},
        {"E_SR"_STRID, "EO"_STRID, {}, "CNF"_STRID},
        {"E_SR"_STRID, "EO"_STRID, {}, "CHANGED"_STRID},
    });

    const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
        {{}, "QI"_STRID, "PERMIT_OP"_STRID, "PERMIT"_STRID},
        {"E_SR"_STRID, "Q"_STRID, {}, "QO"_STRID},
        {"E_SR"_STRID, "Q"_STRID, "SET_CHANGED"_STRID, "G"_STRID},
        {"E_SR"_STRID, "Q"_STRID, "RESET_CHANGED"_STRID, "G"_STRID},
        {"E_MUX_2"_STRID, "K"_STRID, "E_DEMUX_2"_STRID, "K"_STRID},
    });

    const SCFB_FBNData cFBNData = {
        .mEventConnections = cEventConnections,
        .mDataConnections = cDataConnections,
        .mAdapterConnections = {},
    };

  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_CFB_TEST, "CFB_TEST"_STRID)

  FORTE_CFB_TEST::FORTE_CFB_TEST(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_PERMIT_OP("PERMIT_OP"_STRID, *this),
      fb_E_SR("E_SR"_STRID, *this),
      fb_SET_CHANGED("SET_CHANGED"_STRID, *this),
      fb_E_DEMUX_2("E_DEMUX_2"_STRID, "E_DEMUX_2", *this),
      fb_E_MUX_2("E_MUX_2"_STRID, "E_MUX_2", *this),
      fb_RESET_CHANGED("RESET_CHANGED"_STRID, *this),
      conn_CNF(*this, 0),
      conn_CHANGED(*this, 1),
      conn_QI(nullptr),
      conn_QO(*this, 0, 0_BOOL),
      conn_if2in_QI(*this, 0, 0_BOOL) {};

  void FORTE_CFB_TEST::setInitialValues() {
    conn_if2in_QI.getValue() = 0_BOOL;
    fb_E_SR->conn_Q.getValue() = 0_BOOL;
  }

  void FORTE_CFB_TEST::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventSETID: {
        readData(0, conn_if2in_QI.getValue(), conn_QI);
        break;
      }
      case scmEventRESETID: {
        readData(0, conn_if2in_QI.getValue(), conn_QI);
        break;
      }
      default: break;
    }
  }

  void FORTE_CFB_TEST::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_E_SR->conn_Q.getValue(), conn_QO);
        break;
      }
      case scmEventCHANGEDID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_E_SR->conn_Q.getValue(), conn_QO);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_CFB_TEST::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_QI.getValue();
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_CFB_TEST::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &fb_E_SR->conn_Q.getValue();
    }
    return nullptr;
  }

  CEventConnection *FORTE_CFB_TEST::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
      case 1: return &conn_CHANGED;
    }
    return nullptr;
  }

  CDataConnection **FORTE_CFB_TEST::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
    }
    return nullptr;
  }

  CDataConnection *FORTE_CFB_TEST::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
    }
    return nullptr;
  }

  CDataConnection *FORTE_CFB_TEST::getIf2InConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_QI;
    }
    return nullptr;
  }
} // namespace forte::test
