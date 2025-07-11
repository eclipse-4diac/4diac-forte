/*******************************************************************************
 * Copyright (c) 2013 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Michael Hofmann, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "STEST_END_fbt.h"
#include "core/device.h"

USE_STRING_ID(Event);
USE_STRING_ID(REQ);
USE_STRING_ID(STEST_END);

DEFINE_FIRMWARE_FB(FORTE_STEST_END, STRID(STEST_END))

namespace {
  const auto cEventInputNames = std::array{STRID(REQ)};
  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = {},
      .mEONames = {},
      .mEOTypeNames = {},
      .mDINames = {},
      .mDONames = {},
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };
} // namespace

FORTE_STEST_END::FORTE_STEST_END(const CStringDictionary::TStringId paInstanceNameId,
                                 forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId) {};

void FORTE_STEST_END::executeEvent(TEventID paEIID, CEventChainExecutionThread *const) {
  if (scmEventREQID == paEIID) {
    CThread::sleepThread(250); // avoid killing the device before it was properly started (DEV_MGR still was
                               // initializing the ipCommlayer when it was killed)
    getDevice()->changeExecutionState(EMGMCommandType::Kill);
  }
}

void FORTE_STEST_END::readInputData(TEventID) {
  // nothing to do
}

void FORTE_STEST_END::writeOutputData(TEventID) {
  // nothing to do
}

CIEC_ANY *FORTE_STEST_END::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *FORTE_STEST_END::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_STEST_END::getEOConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection **FORTE_STEST_END::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_STEST_END::getDOConUnchecked(TPortId) {
  return nullptr;
}
