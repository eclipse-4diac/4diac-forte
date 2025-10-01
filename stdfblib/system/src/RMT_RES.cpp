/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Rene Smodic, Thomas Strasser,
 *   Martin Melik Merkumians, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "RMT_RES.h"
#include "forte/ecet.h"

using namespace forte::literals;

namespace forte::iec61499::system {
  namespace {
    const auto cDataInputNames = std::array{"MGR_ID"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = {},
        .mEITypeNames = {},
        .mEONames = {},
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(RMT_RES, "iec61499::system::RMT_RES"_STRID);

  RMT_RES::RMT_RES(StringId paInstanceNameId, CFBContainer &paDevice) :
      CResource(paDevice, cFBInterfaceSpec, paInstanceNameId),
      conn_MGR_ID(nullptr),
      conn_MGR_ID_int(*this, 0, u""_WSTRING),
      fb_START("START"_STRID, *this),
      fb_MGR_FF("MGR_FF"_STRID, *this),
      fb_MGR("MGR"_STRID, *this) {
  }

  bool RMT_RES::initialize() {
    if (!CResource::initialize()) {
      return false;
    }

    SManagementCMD command;

    command.mFirstParam.push_back("START"_STRID);
    command.mFirstParam.push_back("COLD"_STRID);
    command.mSecondParam.push_back("MGR_FF"_STRID);
    command.mSecondParam.push_back("S"_STRID);
    createConnection(command);

    command.mFirstParam.clear();
    command.mFirstParam.push_back("START"_STRID);
    command.mFirstParam.push_back("WARM"_STRID);
    command.mSecondParam.clear();
    command.mSecondParam.push_back("MGR_FF"_STRID);
    command.mSecondParam.push_back("S"_STRID);
    createConnection(command);

    command.mFirstParam.clear();
    command.mFirstParam.push_back("START"_STRID);
    command.mFirstParam.push_back("STOP"_STRID);
    command.mSecondParam.clear();
    command.mSecondParam.push_back("MGR_FF"_STRID);
    command.mSecondParam.push_back("R"_STRID);
    createConnection(command);

    command.mFirstParam.clear();
    command.mFirstParam.push_back("MGR_FF"_STRID);
    command.mFirstParam.push_back("EO"_STRID);
    command.mSecondParam.clear();
    command.mSecondParam.push_back("MGR"_STRID);
    command.mSecondParam.push_back("INIT"_STRID);
    createConnection(command);

    command.mFirstParam.clear();
    command.mFirstParam.push_back("MGR_FF"_STRID);
    command.mFirstParam.push_back("Q"_STRID);
    command.mSecondParam.clear();
    command.mSecondParam.push_back("MGR"_STRID);
    command.mSecondParam.push_back("QI"_STRID);
    createConnection(command);

    command.mFirstParam.clear();
    command.mFirstParam.push_back("MGR_ID"_STRID);
    command.mSecondParam.clear();
    command.mSecondParam.push_back("MGR"_STRID);
    command.mSecondParam.push_back("ID"_STRID);
    createConnection(command);
    return true;
  }

  RMT_RES::~RMT_RES() = default;

  void RMT_RES::joinResourceThread() const {
    getResourceEventExecution()->joinEventChainExecutionThread();
  }

  CIEC_ANY *RMT_RES::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_MGR_ID_int.getValue();
    }
    return nullptr;
  }

  CDataConnection **RMT_RES::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_MGR_ID;
    }
    return nullptr;
  }

  CConnection *RMT_RES::getResIf2InConnectionUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_MGR_ID_int;
    }
    return nullptr;
  }
} // namespace forte::iec61499::system
