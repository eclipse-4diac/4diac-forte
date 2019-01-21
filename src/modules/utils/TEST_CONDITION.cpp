/*******************************************************************************
 * Copyright (c) 2019, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "TEST_CONDITION.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "TEST_CONDITION_gen.cpp"
#endif

#include <devlog.h>

DEFINE_FIRMWARE_FB(FORTE_TEST_CONDITION, g_nStringIdTEST_CONDITION)

const CStringDictionary::TStringId FORTE_TEST_CONDITION::scm_anDataInputNames[] = {g_nStringIdcheck};

const CStringDictionary::TStringId FORTE_TEST_CONDITION::scm_anDataInputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FORTE_TEST_CONDITION::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_TEST_CONDITION::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId FORTE_TEST_CONDITION::scm_anEventInputNames[] = {g_nStringIdREQ};

const TForteInt16 FORTE_TEST_CONDITION::scm_anEOWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId FORTE_TEST_CONDITION::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_TEST_CONDITION::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  0, 0,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  0, 0
};


void FORTE_TEST_CONDITION::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventREQID:
      if(check()) {
        DEVLOG_INFO(" ------------------------------ [TEST CONDITION] %s passed\n", getInstanceName());
      } else {
        DEVLOG_ERROR("------------------------------ [TEST CONDITION] %s failed ------------------------------\n", getInstanceName());
      }
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}

