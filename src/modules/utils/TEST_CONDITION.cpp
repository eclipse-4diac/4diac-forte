/*******************************************************************************
 * Copyright (c) 2019, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "TEST_CONDITION.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "TEST_CONDITION_gen.cpp"
#endif

#include <devlog.h>
#include <resource.h>
#include <criticalregion.h>

unsigned int FORTE_TEST_CONDITION::smExecutedTests = 0;
unsigned int FORTE_TEST_CONDITION::smFailedTests = 0;

bool FORTE_TEST_CONDITION::smfinalReportPrinted = false;

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

FORTE_TEST_CONDITION::~FORTE_TEST_CONDITION() {
  CCriticalRegion finalReportRegion(mFinalReportMutex);
  if(!smfinalReportPrinted) {
    smfinalReportPrinted = true;
    DEVLOG_INFO(" ------------------------------------------------------------------------------\n");
    DEVLOG_INFO(" ------------------------ [TEST_CONDITION FINAL REPORT] -----------------------\n");
    if(smFailedTests) {
      DEVLOG_ERROR(" ------------------------ %u tests executed, %u failed -----------------------\n", smExecutedTests, smFailedTests);
    } else {
      DEVLOG_INFO(" ------------------------ %u tests executed, %u failed -----------------------\n", smExecutedTests, smFailedTests);
    }
    DEVLOG_INFO(" ------------------------------------------------------------------------------\n");
  }
}


void FORTE_TEST_CONDITION::executeEvent(int paEIID) {
  if(scm_nEventREQID == paEIID) {
    smExecutedTests++;
    if(check()) {
      DEVLOG_INFO(" ------------------------------ [TEST_CONDITION_PASSED] %s.%s passed\n", getResource().getInstanceName(), getInstanceName());
    } else {
      DEVLOG_ERROR("------------------------------ [TEST_CONDITION_FAILED] %s.%s failed ------------------------------\n", getResource().getInstanceName(),
        getInstanceName());
      smFailedTests++;
    }
    sendOutputEvent(scm_nEventCNFID);
  }
}

