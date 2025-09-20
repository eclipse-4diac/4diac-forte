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

#include "forte/eclipse4diac/utils/STEST_END_fbt.h"
#include "forte/device.h"

using namespace forte::literals;

namespace forte::eclipse4diac::utils {
  namespace {
    const auto cEventInputNames = std::array{"REQ"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_STEST_END, "eclipse4diac::utils::STEST_END"_STRID)

  FORTE_STEST_END::FORTE_STEST_END(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId) {};

  void FORTE_STEST_END::executeEvent(TEventID paEIID, CEventChainExecutionThread *const) {
    if (scmEventREQID == paEIID) {
    arch::CThread::sleepThread(250); // avoid killing the device before it was properly started (DEV_MGR still was
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

} // namespace forte::eclipse4diac::utils
