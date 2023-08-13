/*******************************************************************************
 * Copyright (c) 2019, 2023 fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *   Martin Jobst - add readInputData and writeOutputData
 *******************************************************************************/

#include "TEST_CONDITION.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "TEST_CONDITION_gen.cpp"
#endif

#include "devlog.h"
#include "resource.h"
#include "criticalregion.h"

unsigned int FORTE_TEST_CONDITION::smExecutedTests = 0;
unsigned int FORTE_TEST_CONDITION::smFailedTests = 0;

bool FORTE_TEST_CONDITION::smfinalReportPrinted = false;

DEFINE_FIRMWARE_FB(FORTE_TEST_CONDITION, g_nStringIdTEST_CONDITION)

const CStringDictionary::TStringId FORTE_TEST_CONDITION::scmDataInputNames[] = {g_nStringIdcheck};

const CStringDictionary::TStringId FORTE_TEST_CONDITION::scmDataInputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FORTE_TEST_CONDITION::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_TEST_CONDITION::scmEIWith[] = {0, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_TEST_CONDITION::scmEventInputNames[] = {g_nStringIdREQ};

const CStringDictionary::TStringId FORTE_TEST_CONDITION::scmEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_TEST_CONDITION::scmFBInterfaceSpec = {
  1,  scmEventInputNames,  scmEIWith,  scmEIWithIndexes,
  1,  scmEventOutputNames,  nullptr, nullptr,  1,  scmDataInputNames, scmDataInputTypeIds,
  0,  nullptr, nullptr,
  0, nullptr,
  0, nullptr
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


void FORTE_TEST_CONDITION::executeEvent(TEventID paEIID) {
  if(scmEventREQID == paEIID) {
    smExecutedTests++;
    if(check()) {
      DEVLOG_INFO(" ------------------------------ [TEST_CONDITION_PASSED] %s.%s passed\n", getResource().getInstanceName(), getInstanceName());
    } else {
      DEVLOG_ERROR("------------------------------ [TEST_CONDITION_FAILED] %s.%s failed ------------------------------\n", getResource().getInstanceName(),
        getInstanceName());
      smFailedTests++;
    }
    sendOutputEvent(scmEventCNFID);
  }
}

void FORTE_TEST_CONDITION::readInputData(TEventID) {
  RES_DATA_CON_CRITICAL_REGION();
  readData(0, *mDIs[0], mDIConns[0]);
}

void FORTE_TEST_CONDITION::writeOutputData(TEventID) {
}

