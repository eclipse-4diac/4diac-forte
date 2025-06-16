/*******************************************************************************
 * Copyright (c) 2005, 2015 ACIN, Profactor GmbH, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Rene Smodic, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "RMT_DEV.h"

USE_STRING_ID(MGR);
USE_STRING_ID(MGR_ID);
USE_STRING_ID(WSTRING);

#include <stringdict.h>

namespace {
  const auto cDataInputNames = std::array{STRID(MGR_ID)};

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

RMT_DEV::RMT_DEV(const std::string &paMGR_ID) :
    CDevice(cFBInterfaceSpec, CStringDictionary::scmInvalidStringId),
    conn_MGR_ID_int(*this, 0, u""_WSTRING),
    conn_MGR_ID(nullptr),
    MGR(STRID(MGR), *this) {
  setMGR_ID(paMGR_ID);
}

bool RMT_DEV::initialize() {
  if (!CDevice::initialize()) {
    return false;
  }

  if (!MGR.initialize()) {
    return false;
  }

  // we need to manually create this connection as the MGR is not managed by device
  conn_MGR_ID_int.connect(MGR, std::array{STRID(MGR_ID)});
  return true;
}

RMT_DEV::~RMT_DEV() = default;

int RMT_DEV::startDevice() {
  CDevice::startDevice();
  MGR.changeExecutionState(EMGMCommandType::Start);
  return 0;
}

void RMT_DEV::awaitShutdown() {
  MGR.joinResourceThread();
}

EMGMResponse RMT_DEV::changeExecutionState(EMGMCommandType paCommand) {
  EMGMResponse eRetVal = CDevice::changeExecutionState(paCommand);
  if ((EMGMResponse::Ready == eRetVal) && (EMGMCommandType::Kill == paCommand)) {
    MGR.changeExecutionState(EMGMCommandType::Kill);
  }
  return eRetVal;
}

void RMT_DEV::setMGR_ID(const std::string &paVal) {
  conn_MGR_ID_int.getValue().fromString(paVal.c_str());
}

CIEC_ANY *RMT_DEV::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &conn_MGR_ID_int.getValue();
  }
  return nullptr;
}

CDataConnection **RMT_DEV::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_MGR_ID;
  }
  return nullptr;
}

CConnection *RMT_DEV::getResIf2InConnectionUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_MGR_ID_int;
  }
  return nullptr;
}
