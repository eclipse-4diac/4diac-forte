/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: LINT2LINT
 *** Description:
 *** Version:
 ***     1.0: 2023-04-28/Martin Jobst -  -
 *************************************************************************/

#include "LINT2LINT_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "LINT2LINT_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_lint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_LINT2LINT, g_nStringIdLINT2LINT)

const CStringDictionary::TStringId FORTE_LINT2LINT::scmDataInputNames[] = {g_nStringIdIN};
const CStringDictionary::TStringId FORTE_LINT2LINT::scmDataInputTypeIds[] = {g_nStringIdLINT};
const CStringDictionary::TStringId FORTE_LINT2LINT::scmDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId FORTE_LINT2LINT::scmDataOutputTypeIds[] = {g_nStringIdLINT};
const TDataIOID FORTE_LINT2LINT::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_LINT2LINT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_LINT2LINT::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_LINT2LINT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_LINT2LINT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_LINT2LINT::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_LINT2LINT::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_LINT2LINT::FORTE_LINT2LINT(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_LINT2LINT::setInitialValues() {
  var_IN = 0_LINT;
  var_OUT = 0_LINT;
}

void FORTE_LINT2LINT::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_LINT2LINT::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_LINT2LINT::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_LINT2LINT::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_LINT2LINT::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_LINT2LINT::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_LINT2LINT::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_LINT2LINT::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_LINT2LINT::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_LINT2LINT::alg_REQ(void) {

  #line 2 "LINT2LINT.fbt"
  var_OUT = var_IN;
}

