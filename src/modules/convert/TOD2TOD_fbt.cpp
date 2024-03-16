/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: TOD2TOD
 *** Description:
 *** Version:
 ***     1.0: 2023-04-28/Martin Jobst -  -
 *************************************************************************/

#include "TOD2TOD_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "TOD2TOD_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_time_of_day.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_TOD2TOD, g_nStringIdTOD2TOD)

const CStringDictionary::TStringId FORTE_TOD2TOD::scmDataInputNames[] = {g_nStringIdIN};
const CStringDictionary::TStringId FORTE_TOD2TOD::scmDataInputTypeIds[] = {g_nStringIdTIME_OF_DAY};
const CStringDictionary::TStringId FORTE_TOD2TOD::scmDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId FORTE_TOD2TOD::scmDataOutputTypeIds[] = {g_nStringIdTIME_OF_DAY};
const TDataIOID FORTE_TOD2TOD::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_TOD2TOD::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_TOD2TOD::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_TOD2TOD::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_TOD2TOD::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_TOD2TOD::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_TOD2TOD::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_TOD2TOD::FORTE_TOD2TOD(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_TOD2TOD::setInitialValues() {
  var_IN = 0_TIME_OF_DAY;
  var_OUT = 0_TIME_OF_DAY;
}

void FORTE_TOD2TOD::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_TOD2TOD::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_TOD2TOD::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_TOD2TOD::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_TOD2TOD::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_TOD2TOD::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_TOD2TOD::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_TOD2TOD::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_TOD2TOD::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_TOD2TOD::alg_REQ(void) {

  #line 2 "TOD2TOD.fbt"
  var_OUT = var_IN;
}

