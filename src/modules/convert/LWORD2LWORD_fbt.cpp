/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: LWORD2LWORD
 *** Description:
 *** Version:
 ***     1.0: 2023-04-28/Martin Jobst -  -
 *************************************************************************/

#include "LWORD2LWORD_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "LWORD2LWORD_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_lword.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_LWORD2LWORD, g_nStringIdLWORD2LWORD)

const CStringDictionary::TStringId FORTE_LWORD2LWORD::scmDataInputNames[] = {g_nStringIdIN};
const CStringDictionary::TStringId FORTE_LWORD2LWORD::scmDataInputTypeIds[] = {g_nStringIdLWORD};
const CStringDictionary::TStringId FORTE_LWORD2LWORD::scmDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId FORTE_LWORD2LWORD::scmDataOutputTypeIds[] = {g_nStringIdLWORD};
const TDataIOID FORTE_LWORD2LWORD::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_LWORD2LWORD::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_LWORD2LWORD::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_LWORD2LWORD::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_LWORD2LWORD::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_LWORD2LWORD::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_LWORD2LWORD::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_LWORD2LWORD::FORTE_LWORD2LWORD(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_LWORD2LWORD::setInitialValues() {
  var_IN = 0_LWORD;
  var_OUT = 0_LWORD;
}

void FORTE_LWORD2LWORD::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_LWORD2LWORD::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_LWORD2LWORD::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_LWORD2LWORD::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_LWORD2LWORD::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_LWORD2LWORD::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_LWORD2LWORD::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_LWORD2LWORD::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_LWORD2LWORD::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_LWORD2LWORD::alg_REQ(void) {

  #line 2 "LWORD2LWORD.fbt"
  var_OUT = var_IN;
}

