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

#include "E_SELECT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SELECT_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_SELECT, g_nStringIdE_SELECT)

const CStringDictionary::TStringId FORTE_E_SELECT::scmDataInputNames[] = {g_nStringIdG};
const CStringDictionary::TStringId FORTE_E_SELECT::scmDataInputTypeIds[] = {g_nStringIdBOOL};
const TDataIOID FORTE_E_SELECT::scmEIWith[] = {0, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_E_SELECT::scmEIWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_E_SELECT::scmEventInputNames[] = {g_nStringIdEI0, g_nStringIdEI1};
const TForteInt16 FORTE_E_SELECT::scmEOWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_E_SELECT::scmEventOutputNames[] = {g_nStringIdEO};
const SFBInterfaceSpec FORTE_E_SELECT::scmFBInterfaceSpec = {
  2, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

FORTE_E_SELECT::FORTE_E_SELECT(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CBasicFB(paSrcRes, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_G(CIEC_BOOL(0)),
    conn_EO(this, 0),
    conn_G(nullptr) {
}



void FORTE_E_SELECT::executeEvent(TEventID paEIID){
  do {
    switch(mECCState) {
      case scmStateSTART:
        if((scmEventEI0ID == paEIID) && (func_NOT<CIEC_BOOL>(var_G))) enterStateEO();
        else
        if((scmEventEI1ID == paEIID) && (var_G)) enterStateEO();
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

void FORTE_E_SELECT::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventEI0ID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_G, conn_G);
      break;
    }
    case scmEventEI1ID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_G, conn_G);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_SELECT::writeOutputData(TEventID) {
}

CIEC_ANY *FORTE_E_SELECT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_G;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_SELECT::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_SELECT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_SELECT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_G;
  }
  return nullptr;
}

CDataConnection *FORTE_E_SELECT::getDOConUnchecked(TPortId) {
  return nullptr;
}

CIEC_ANY *FORTE_E_SELECT::getVarInternal(size_t) {
  return nullptr;
}


void FORTE_E_SELECT::enterStateSTART(void) {
  mECCState = scmStateSTART;
}

void FORTE_E_SELECT::enterStateEO(void) {
  mECCState = scmStateEO;
  sendOutputEvent(scmEventEOID);
}



