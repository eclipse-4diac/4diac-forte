/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, fortiss GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "F_SEL.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_SEL_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_SEL, g_nStringIdF_SEL)

const CStringDictionary::TStringId FORTE_F_SEL::scm_anDataInputNames[] = {g_nStringIdG, g_nStringIdIN0, g_nStringIdIN1};

const CStringDictionary::TStringId FORTE_F_SEL::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdANY, g_nStringIdANY};

const CStringDictionary::TStringId FORTE_F_SEL::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_SEL::scm_anDataOutputTypeIds[] = {g_nStringIdANY};

const TDataIOID FORTE_F_SEL::scm_anEIWith[] = {0, 2, 1, 255};
const TForteInt16 FORTE_F_SEL::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_SEL::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_SEL::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_F_SEL::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_SEL::scm_anEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_SEL::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  3, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

FORTE_F_SEL::FORTE_F_SEL(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CFunctionBlock( pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId),
    var_G(CIEC_BOOL(0)),
    var_IN0(CIEC_ANY_VARIANT()),
    var_IN1(CIEC_ANY_VARIANT()),
    var_OUT(CIEC_ANY_VARIANT()),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_G(nullptr),
    conn_IN0(nullptr),
    conn_IN1(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
};

void FORTE_F_SEL::executeEvent(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID:
      var_OUT = var_G ? var_IN1 : var_IN0;
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}

void FORTE_F_SEL::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      readData(0, &var_G, conn_G);
      readData(2, &var_IN1, conn_IN1);
      readData(1, &var_IN0, conn_IN0);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_SEL::writeOutputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCNFID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      writeData(0, &var_OUT, &conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_SEL::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_G;
    case 1: return &var_IN0;
    case 2: return &var_IN1;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_SEL::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_SEL::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_SEL::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_G;
    case 1: return &conn_IN0;
    case 2: return &conn_IN1;
  }
  return nullptr;
}

CDataConnection *FORTE_F_SEL::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


