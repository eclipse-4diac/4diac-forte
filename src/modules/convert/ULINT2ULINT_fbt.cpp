/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: ULINT2ULINT
 *** Description:
 *** Version:
 ***     1.0: 2023-04-28/Martin Jobst -  -
 *************************************************************************/

#include "ULINT2ULINT_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "ULINT2ULINT_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_ulint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_ULINT2ULINT, g_nStringIdULINT2ULINT)

const CStringDictionary::TStringId FORTE_ULINT2ULINT::scmDataInputNames[] = {g_nStringIdIN};
const CStringDictionary::TStringId FORTE_ULINT2ULINT::scmDataInputTypeIds[] = {g_nStringIdULINT};
const CStringDictionary::TStringId FORTE_ULINT2ULINT::scmDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId FORTE_ULINT2ULINT::scmDataOutputTypeIds[] = {g_nStringIdULINT};
const TDataIOID FORTE_ULINT2ULINT::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_ULINT2ULINT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_ULINT2ULINT::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_ULINT2ULINT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_ULINT2ULINT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_ULINT2ULINT::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_ULINT2ULINT::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_ULINT2ULINT::FORTE_ULINT2ULINT(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_ULINT2ULINT::setInitialValues() {
  var_IN = 0_ULINT;
  var_OUT = 0_ULINT;
}

void FORTE_ULINT2ULINT::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_ULINT2ULINT::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_ULINT2ULINT::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_ULINT2ULINT::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_ULINT2ULINT::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_ULINT2ULINT::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_ULINT2ULINT::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_ULINT2ULINT::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_ULINT2ULINT::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_ULINT2ULINT::alg_REQ(void) {

  #line 2 "ULINT2ULINT.fbt"
  var_OUT = var_IN;
}

