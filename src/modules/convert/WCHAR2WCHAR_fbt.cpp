/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: WCHAR2WCHAR
 *** Description:
 *** Version:
 ***     1.0: 2023-04-28/Martin Jobst -  -
 *************************************************************************/

#include "WCHAR2WCHAR_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "WCHAR2WCHAR_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_wchar.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_WCHAR2WCHAR, g_nStringIdWCHAR2WCHAR)

const CStringDictionary::TStringId FORTE_WCHAR2WCHAR::scmDataInputNames[] = {g_nStringIdIN};
const CStringDictionary::TStringId FORTE_WCHAR2WCHAR::scmDataInputTypeIds[] = {g_nStringIdWCHAR};
const CStringDictionary::TStringId FORTE_WCHAR2WCHAR::scmDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId FORTE_WCHAR2WCHAR::scmDataOutputTypeIds[] = {g_nStringIdWCHAR};
const TDataIOID FORTE_WCHAR2WCHAR::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_WCHAR2WCHAR::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_WCHAR2WCHAR::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_WCHAR2WCHAR::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_WCHAR2WCHAR::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_WCHAR2WCHAR::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_WCHAR2WCHAR::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_WCHAR2WCHAR::FORTE_WCHAR2WCHAR(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_WCHAR2WCHAR::setInitialValues() {
  var_IN = u'\0'_WCHAR;
  var_OUT = u'\0'_WCHAR;
}

void FORTE_WCHAR2WCHAR::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_WCHAR2WCHAR::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_WCHAR2WCHAR::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_WCHAR2WCHAR::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_WCHAR2WCHAR::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_WCHAR2WCHAR::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_WCHAR2WCHAR::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_WCHAR2WCHAR::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_WCHAR2WCHAR::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_WCHAR2WCHAR::alg_REQ(void) {

  #line 2 "WCHAR2WCHAR.fbt"
  var_OUT = var_IN;
}

