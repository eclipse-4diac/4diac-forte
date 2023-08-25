/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "E_PERMIT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_PERMIT_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_PERMIT, g_nStringIdE_PERMIT)

const CStringDictionary::TStringId FORTE_E_PERMIT::scmDataInputNames[] = {g_nStringIdPERMIT};
const CStringDictionary::TStringId FORTE_E_PERMIT::scmDataInputTypeIds[] = {g_nStringIdBOOL};
const TDataIOID FORTE_E_PERMIT::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_PERMIT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_PERMIT::scmEventInputNames[] = {g_nStringIdEI};
const TForteInt16 FORTE_E_PERMIT::scmEOWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_E_PERMIT::scmEventOutputNames[] = {g_nStringIdEO};
const SFBInterfaceSpec FORTE_E_PERMIT::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

FORTE_E_PERMIT::FORTE_E_PERMIT(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CBasicFB(paSrcRes, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_PERMIT(CIEC_BOOL(0)),
    conn_EO(this, 0),
    conn_PERMIT(nullptr) {
}



void FORTE_E_PERMIT::executeEvent(TEventID paEIID){
  do {
    switch(mECCState) {
      case scmStateSTART:
        if((scmEventEIID == paEIID) && (var_PERMIT)) enterStateEO();
        else return; //no transition cleared
        break;
      case scmStateEO:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 2.", mECCState.operator TForteUInt16 ());
        mECCState = 0; // 0 is always the initial state
        return;
    }
    paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(true);
}

void FORTE_E_PERMIT::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventEIID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_PERMIT, conn_PERMIT);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_PERMIT::writeOutputData(TEventID) {
}

CIEC_ANY *FORTE_E_PERMIT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_PERMIT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_PERMIT::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_PERMIT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_PERMIT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_PERMIT;
  }
  return nullptr;
}

CDataConnection *FORTE_E_PERMIT::getDOConUnchecked(TPortId) {
  return nullptr;
}

CIEC_ANY *FORTE_E_PERMIT::getVarInternal(size_t) {
  return nullptr;
}


void FORTE_E_PERMIT::enterStateSTART(void) {
  mECCState = scmStateSTART;
}

void FORTE_E_PERMIT::enterStateEO(void) {
  mECCState = scmStateEO;
  sendOutputEvent(scmEventEOID);
}



