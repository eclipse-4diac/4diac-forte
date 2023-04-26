/*******************************************************************************
 * Copyright (c) 2011 - 2014 AIT, ACIN, Profactor GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Alois Zoitl, Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "OUT_ANY_CONSOLE.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "OUT_ANY_CONSOLE_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_OUT_ANY_CONSOLE, g_nStringIdOUT_ANY_CONSOLE)

const CStringDictionary::TStringId FORTE_OUT_ANY_CONSOLE::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdLABEL, g_nStringIdIN};

const CStringDictionary::TStringId FORTE_OUT_ANY_CONSOLE::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdANY};

const CStringDictionary::TStringId FORTE_OUT_ANY_CONSOLE::scm_anDataOutputNames[] = {g_nStringIdQO};

const CStringDictionary::TStringId FORTE_OUT_ANY_CONSOLE::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TDataIOID FORTE_OUT_ANY_CONSOLE::scm_anEIWith[] = {0, 2, 1, 255};
const TForteInt16 FORTE_OUT_ANY_CONSOLE::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_OUT_ANY_CONSOLE::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_OUT_ANY_CONSOLE::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_OUT_ANY_CONSOLE::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_OUT_ANY_CONSOLE::scm_anEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_OUT_ANY_CONSOLE::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  3, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

FORTE_OUT_ANY_CONSOLE::FORTE_OUT_ANY_CONSOLE(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CFunctionBlock( pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId),
    var_QI(CIEC_BOOL(0)),
    var_LABEL(CIEC_STRING("")),
    var_IN(CIEC_ANY_VARIANT()),
    var_QO(CIEC_BOOL(0)),
    var_conn_QO(var_QO),
    conn_CNF(this, 0),
    conn_QI(nullptr),
    conn_LABEL(nullptr),
    conn_IN(nullptr),
    conn_QO(this, 0, &var_conn_QO) {
};

void FORTE_OUT_ANY_CONSOLE::executeEvent(int pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID:
      var_QO = var_QI;
      if (var_QI) {
        DEVLOG_INFO(" %s = ", var_LABEL.getValue());
        size_t bufferSize = var_IN.getToStringBufferSize();
        std::string buffer(bufferSize, 0);
        var_IN.toString(buffer.data(), buffer.capacity());
        DEVLOG_INFO("%s\n", buffer.c_str());
      }
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}

void FORTE_OUT_ANY_CONSOLE::readInputData(size_t pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      readData(0, &var_QI, conn_QI);
      readData(2, &var_IN, conn_IN);
      readData(1, &var_LABEL, conn_LABEL);
      break;
    }
    default:
      break;
  }
}

void FORTE_OUT_ANY_CONSOLE::writeOutputData(size_t pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCNFID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      writeData(0, &var_QO, &conn_QO);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_OUT_ANY_CONSOLE::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_LABEL;
    case 2: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_OUT_ANY_CONSOLE::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
  }
  return nullptr;
}

CEventConnection *FORTE_OUT_ANY_CONSOLE::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_OUT_ANY_CONSOLE::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_LABEL;
    case 2: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_OUT_ANY_CONSOLE::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}


