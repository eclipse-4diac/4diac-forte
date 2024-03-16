/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: DATE2DATE
 *** Description:
 *** Version:
 ***     1.0: 2023-04-28/Martin Jobst -  -
 *************************************************************************/

#include "DATE2DATE_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "DATE2DATE_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_date.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_DATE2DATE, g_nStringIdDATE2DATE)

const CStringDictionary::TStringId FORTE_DATE2DATE::scmDataInputNames[] = {g_nStringIdIN};
const CStringDictionary::TStringId FORTE_DATE2DATE::scmDataInputTypeIds[] = {g_nStringIdDATE};
const CStringDictionary::TStringId FORTE_DATE2DATE::scmDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId FORTE_DATE2DATE::scmDataOutputTypeIds[] = {g_nStringIdDATE};
const TDataIOID FORTE_DATE2DATE::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_DATE2DATE::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_DATE2DATE::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_DATE2DATE::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_DATE2DATE::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_DATE2DATE::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_DATE2DATE::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_DATE2DATE::FORTE_DATE2DATE(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_DATE2DATE::setInitialValues() {
  var_IN = 0_DATE;
  var_OUT = 0_DATE;
}

void FORTE_DATE2DATE::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_DATE2DATE::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_DATE2DATE::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_DATE2DATE::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_DATE2DATE::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_DATE2DATE::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_DATE2DATE::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_DATE2DATE::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_DATE2DATE::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_DATE2DATE::alg_REQ(void) {

  #line 2 "DATE2DATE.fbt"
  var_OUT = var_IN;
}

