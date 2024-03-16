/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: LTIME2LTIME
 *** Description:
 *** Version:
 ***     1.0: 2023-04-28/Martin Jobst -  -
 *************************************************************************/

#include "LTIME2LTIME_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "LTIME2LTIME_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_ltime.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_LTIME2LTIME, g_nStringIdLTIME2LTIME)

const CStringDictionary::TStringId FORTE_LTIME2LTIME::scmDataInputNames[] = {g_nStringIdIN};
const CStringDictionary::TStringId FORTE_LTIME2LTIME::scmDataInputTypeIds[] = {g_nStringIdLTIME};
const CStringDictionary::TStringId FORTE_LTIME2LTIME::scmDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId FORTE_LTIME2LTIME::scmDataOutputTypeIds[] = {g_nStringIdLTIME};
const TDataIOID FORTE_LTIME2LTIME::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_LTIME2LTIME::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_LTIME2LTIME::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_LTIME2LTIME::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_LTIME2LTIME::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_LTIME2LTIME::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_LTIME2LTIME::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_LTIME2LTIME::FORTE_LTIME2LTIME(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_LTIME2LTIME::setInitialValues() {
  var_IN = 0_LTIME;
  var_OUT = 0_LTIME;
}

void FORTE_LTIME2LTIME::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_LTIME2LTIME::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_LTIME2LTIME::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_LTIME2LTIME::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_LTIME2LTIME::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_LTIME2LTIME::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_LTIME2LTIME::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_LTIME2LTIME::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_LTIME2LTIME::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_LTIME2LTIME::alg_REQ(void) {

  #line 2 "LTIME2LTIME.fbt"
  var_OUT = var_IN;
}

