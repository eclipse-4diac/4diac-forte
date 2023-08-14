/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny, Monika Wenger,
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "RT_E_PERMIT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RT_E_PERMIT_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_RT_E_PERMIT, g_nStringIdRT_E_PERMIT)

const CStringDictionary::TStringId FORTE_RT_E_PERMIT::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdPERMIT, g_nStringIdTmin, g_nStringIdDeadline, g_nStringIdWCET};
const CStringDictionary::TStringId FORTE_RT_E_PERMIT::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME};
const CStringDictionary::TStringId FORTE_RT_E_PERMIT::scm_anDataOutputNames[] = {g_nStringIdQO};
const CStringDictionary::TStringId FORTE_RT_E_PERMIT::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};
const TDataIOID FORTE_RT_E_PERMIT::scm_anEIWith[] = {0, 2, 3, 4, scmWithListDelimiter, 1, scmWithListDelimiter};
const TForteInt16 FORTE_RT_E_PERMIT::scm_anEIWithIndexes[] = {0, 5};
const CStringDictionary::TStringId FORTE_RT_E_PERMIT::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdEI};
const TDataIOID FORTE_RT_E_PERMIT::scm_anEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_RT_E_PERMIT::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_RT_E_PERMIT::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdEO};
const SFBInterfaceSpec FORTE_RT_E_PERMIT::scm_stFBInterfaceSpec = {
  2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  2, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  5, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_RT_E_PERMIT::FORTE_RT_E_PERMIT(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
        CRTEventSingle( pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId),
    var_conn_QO(var_QO),
    conn_INITO(this, 0),
    conn_EO(this, 1),
    conn_QI(nullptr),
    conn_PERMIT(nullptr),
    conn_Tmin(nullptr),
    conn_Deadline(nullptr),
    conn_WCET(nullptr),
    conn_QO(this, 0, &var_conn_QO) {
};

void FORTE_RT_E_PERMIT::setInitialValues() {
  var_QI = 0_BOOL;
  var_PERMIT = 0_BOOL;
  var_Tmin = 0_TIME;
  var_Deadline = 0_TIME;
  var_WCET = 0_TIME;
  var_QO = 0_BOOL;
}

bool FORTE_RT_E_PERMIT::checkActivation(TEventID){
  return var_PERMIT;
}

void FORTE_RT_E_PERMIT::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scm_nEventINITID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_QI, conn_QI);
      readData(2, var_Tmin, conn_Tmin);
      readData(3, var_Deadline, conn_Deadline);
      readData(4, var_WCET, conn_WCET);
      break;
    }
    case scm_nEventEIID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(1, var_PERMIT, conn_PERMIT);
      break;
    }
    default:
      break;
  }
}

void FORTE_RT_E_PERMIT::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scm_nEventINITOID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, var_QO, conn_QO);
      break;
    }
    case scm_nEventEOID: {
      RES_DATA_CON_CRITICAL_REGION();
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_RT_E_PERMIT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_PERMIT;
    case 2: return &var_Tmin;
    case 3: return &var_Deadline;
    case 4: return &var_WCET;
  }
  return nullptr;
}

CIEC_ANY *FORTE_RT_E_PERMIT::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
  }
  return nullptr;
}

CIEC_ANY *FORTE_RT_E_PERMIT::getDIO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_RT_E_PERMIT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_RT_E_PERMIT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_PERMIT;
    case 2: return &conn_Tmin;
    case 3: return &conn_Deadline;
    case 4: return &conn_WCET;
  }
  return nullptr;
}

CDataConnection *FORTE_RT_E_PERMIT::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}

CInOutDataConnection **FORTE_RT_E_PERMIT::getDIOInConUnchecked(TPortId) {
  return nullptr;
}

CInOutDataConnection *FORTE_RT_E_PERMIT::getDIOOutConUnchecked(TPortId) {
  return nullptr;
}


