/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: CHAR2CHAR
 *** Description:
 *** Version:
 ***     1.0: 2023-04-28/Martin Jobst -  -
 *************************************************************************/

#include "CHAR2CHAR_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "CHAR2CHAR_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_char.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_CHAR2CHAR, g_nStringIdCHAR2CHAR)

const CStringDictionary::TStringId FORTE_CHAR2CHAR::scmDataInputNames[] = {g_nStringIdIN};
const CStringDictionary::TStringId FORTE_CHAR2CHAR::scmDataInputTypeIds[] = {g_nStringIdCHAR};
const CStringDictionary::TStringId FORTE_CHAR2CHAR::scmDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId FORTE_CHAR2CHAR::scmDataOutputTypeIds[] = {g_nStringIdCHAR};
const TDataIOID FORTE_CHAR2CHAR::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_CHAR2CHAR::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_CHAR2CHAR::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_CHAR2CHAR::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_CHAR2CHAR::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_CHAR2CHAR::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_CHAR2CHAR::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_CHAR2CHAR::FORTE_CHAR2CHAR(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_CHAR2CHAR::setInitialValues() {
  var_IN = '\0'_CHAR;
  var_OUT = '\0'_CHAR;
}

void FORTE_CHAR2CHAR::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_CHAR2CHAR::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_CHAR2CHAR::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_CHAR2CHAR::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_CHAR2CHAR::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_CHAR2CHAR::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_CHAR2CHAR::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_CHAR2CHAR::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_CHAR2CHAR::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_CHAR2CHAR::alg_REQ(void) {

  #line 2 "CHAR2CHAR.fbt"
  var_OUT = var_IN;
}

