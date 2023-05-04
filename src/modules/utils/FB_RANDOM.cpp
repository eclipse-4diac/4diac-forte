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

#include <time.h>
#include <stdlib.h>

DEFINE_FIRMWARE_FB(FORTE_FB_RANDOM, g_nStringIdFB_RANDOM)

const CStringDictionary::TStringId FORTE_FB_RANDOM::scm_anDataInputNames[] = {g_nStringIdSEED};

const CStringDictionary::TStringId FORTE_FB_RANDOM::scm_anDataInputTypeIds[] = {g_nStringIdUINT};

const CStringDictionary::TStringId FORTE_FB_RANDOM::scm_anDataOutputNames[] = {g_nStringIdVAL};

const CStringDictionary::TStringId FORTE_FB_RANDOM::scm_anDataOutputTypeIds[] = {g_nStringIdREAL};

const TDataIOID FORTE_FB_RANDOM::scm_anEIWith[] = {0, 255};
const TForteInt16 FORTE_FB_RANDOM::scm_anEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FB_RANDOM::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};

const TDataIOID FORTE_FB_RANDOM::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_FB_RANDOM::scm_anEOWithIndexes[] = {-1, 0};
const CStringDictionary::TStringId FORTE_FB_RANDOM::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF};


const SFBInterfaceSpec FORTE_FB_RANDOM::scm_stFBInterfaceSpec = {
  2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  2, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  1, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};


FORTE_FB_RANDOM::FORTE_FB_RANDOM(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    var_SEED(CIEC_UINT(0)),
    var_VAL(CIEC_REAL(0)),
    var_conn_VAL(var_VAL),
    conn_INITO(this, 0),
    conn_CNF(this, 1),
    conn_SEED(nullptr),
    conn_VAL(this, 0, &var_conn_VAL) {
}

void FORTE_FB_RANDOM::alg_INIT(){
// WARNING - Don't forget to add #include <time.h>
  if (static_cast<CIEC_UINT::TValueType>(var_SEED) == 0) {
    srand((unsigned int) time(nullptr) );
  } else {
    srand(static_cast<CIEC_UINT::TValueType>(var_SEED));
  }
}

void FORTE_FB_RANDOM::alg_REQ(){
  var_VAL = CIEC_REAL(static_cast<TForteFloat>(rand())/static_cast<TForteFloat>(RAND_MAX));
}


void FORTE_FB_RANDOM::executeEvent(int pa_nEIID){
  do {
    switch(m_nECCState) {
      case scm_nStateSTART:
        if(scm_nEventREQID == pa_nEIID) enterStateREQ();
        else
        if(scm_nEventINITID == pa_nEIID) enterStateState();
        else return; //no transition cleared
        break;
      case scm_nStateREQ:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      case scm_nStateState:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 3.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; // 0 is always the initial state
        return;
    }
    pa_nEIID = cg_nInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(true);
}

void FORTE_FB_RANDOM::readInputData(size_t pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventINITID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      readData(0, &var_SEED, conn_SEED);
      break;
    }
    case scm_nEventREQID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      break;
    }
    default:
      break;
  }
}

void FORTE_FB_RANDOM::writeOutputData(size_t pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventINITOID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      break;
    }
    case scm_nEventCNFID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      writeData(0, &var_VAL, &conn_VAL);
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
  m_nECCState = scm_nStateSTART;
}

void FORTE_FB_RANDOM::enterStateREQ(void) {
  m_nECCState = scm_nStateREQ;
  alg_REQ();
  sendOutputEvent(scm_nEventCNFID);
}

void FORTE_FB_RANDOM::enterStateState(void) {
  m_nECCState = scm_nStateState;
  alg_INIT();
  sendOutputEvent(scm_nEventINITOID);
}



