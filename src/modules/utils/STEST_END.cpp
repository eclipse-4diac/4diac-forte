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
#include "STEST_END.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "STEST_END_gen.cpp"
#endif
#include "../../stdfblib/ita/RMT_DEV.h"
#include <forte_thread.h>

DEFINE_FIRMWARE_FB(FORTE_STEST_END, g_nStringIdSTEST_END)

const TForteInt16 FORTE_STEST_END::scmEIWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_STEST_END::scmEventInputNames[] = {g_nStringIdREQ};

const SFBInterfaceSpec FORTE_STEST_END::scmFBInterfaceSpec = {
  1,  scmEventInputNames,  nullptr,  scmEIWithIndexes,
  0,  nullptr,  nullptr, nullptr,  0,  nullptr, nullptr,
  0,  nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

void FORTE_STEST_END::executeEvent(TEventID paEIID){
  if(scmEventREQID == paEIID){
    CThread::sleepThread(250); //avoid killing the device before it was properly started (DEV_MGR still was initializing the ipCommlayer when it was killed)
    getResource().getDevice().changeFBExecutionState(EMGMCommandType::Kill);
  }
}



