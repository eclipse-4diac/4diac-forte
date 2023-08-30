/*******************************************************************************
 * Copyright (c) 2012, 20113, 2015 Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Gerhard Ebenhofer, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "FB_RANDOM.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FB_RANDOM_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_uint.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_FB_RANDOM, g_nStringIdFB_RANDOM)

const CStringDictionary::TStringId FORTE_FB_RANDOM::scmDataInputNames[] = {g_nStringIdSEED};

const CStringDictionary::TStringId FORTE_FB_RANDOM::scmDataInputTypeIds[] = {g_nStringIdUINT};

const CStringDictionary::TStringId FORTE_FB_RANDOM::scmDataOutputNames[] = {g_nStringIdVAL};

const CStringDictionary::TStringId FORTE_FB_RANDOM::scmDataOutputTypeIds[] = {g_nStringIdREAL};

const TDataIOID FORTE_FB_RANDOM::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_FB_RANDOM::scmEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FB_RANDOM::scmEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};

const TDataIOID FORTE_FB_RANDOM::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_FB_RANDOM::scmEOWithIndexes[] = {-1, 0};
const CStringDictionary::TStringId FORTE_FB_RANDOM::scmEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF};


const SFBInterfaceSpec FORTE_FB_RANDOM::scmFBInterfaceSpec = {
  2, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};


FORTE_FB_RANDOM::FORTE_FB_RANDOM(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CBasicFB(paSrcRes, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    mDistribution(0.0f, 1.0f),
    var_SEED(CIEC_UINT(0)),
    var_VAL(CIEC_REAL(0)),
    var_conn_VAL(var_VAL),
    conn_INITO(this, 0),
    conn_CNF(this, 1),
    conn_SEED(nullptr),
    conn_VAL(this, 0, &var_conn_VAL) {
      mRandomGenerator.seed(mRandomDevice());
}

void FORTE_FB_RANDOM::alg_INIT(){
  if (static_cast<CIEC_UINT::TValueType>(var_SEED) == 0) {
    mRandomGenerator.seed(mRandomDevice());
  } else {
    const CIEC_UINT::TValueType seedValue = static_cast<CIEC_UINT::TValueType>(var_SEED);
    mRandomGenerator.seed(seedValue);
  }
}

void FORTE_FB_RANDOM::alg_REQ(){
  var_VAL = CIEC_REAL(mDistribution(mRandomGenerator));
}


void FORTE_FB_RANDOM::executeEvent(TEventID paEIID){
  do {
    switch(mECCState) {
      case scmStateSTART:
        if(scmEventREQID == paEIID) enterStateREQ();
        else
        if(scmEventINITID == paEIID) enterStateState();
        else return; //no transition cleared
        break;
      case scmStateREQ:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      case scmStateState:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 3.", mECCState.operator TForteUInt16 ());
        mECCState = 0; // 0 is always the initial state
        return;
    }
    paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(true);
}

void FORTE_FB_RANDOM::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_SEED, conn_SEED);
      break;
    }
    case scmEventREQID: {
      RES_DATA_CON_CRITICAL_REGION();
      break;
    }
    default:
      break;
  }
}

void FORTE_FB_RANDOM::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITOID: {
      RES_DATA_CON_CRITICAL_REGION();
      break;
    }
    case scmEventCNFID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, var_VAL, conn_VAL);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_FB_RANDOM::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_SEED;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_RANDOM::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_VAL;
  }
  return nullptr;
}

CEventConnection *FORTE_FB_RANDOM::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_FB_RANDOM::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_SEED;
  }
  return nullptr;
}

CDataConnection *FORTE_FB_RANDOM::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_VAL;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_RANDOM::getVarInternal(size_t) {
  return nullptr;
}


void FORTE_FB_RANDOM::enterStateSTART(void) {
  mECCState = scmStateSTART;
}

void FORTE_FB_RANDOM::enterStateREQ(void) {
  mECCState = scmStateREQ;
  alg_REQ();
  sendOutputEvent(scmEventCNFID);
}

void FORTE_FB_RANDOM::enterStateState(void) {
  mECCState = scmStateState;
  alg_INIT();
  sendOutputEvent(scmEventINITOID);
}



