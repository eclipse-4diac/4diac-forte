/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: LDATE2LDATE
 *** Description:
 *** Version:
 ***     1.0: 2023-04-28/Martin Jobst -  -
 *************************************************************************/

#include "LDATE2LDATE_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "LDATE2LDATE_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_ldate.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_LDATE2LDATE, g_nStringIdLDATE2LDATE)

const CStringDictionary::TStringId FORTE_LDATE2LDATE::scmDataInputNames[] = {g_nStringIdIN};
const CStringDictionary::TStringId FORTE_LDATE2LDATE::scmDataInputTypeIds[] = {g_nStringIdLDATE};
const CStringDictionary::TStringId FORTE_LDATE2LDATE::scmDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId FORTE_LDATE2LDATE::scmDataOutputTypeIds[] = {g_nStringIdLDATE};
const TDataIOID FORTE_LDATE2LDATE::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_LDATE2LDATE::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_LDATE2LDATE::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_LDATE2LDATE::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_LDATE2LDATE::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_LDATE2LDATE::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_LDATE2LDATE::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_LDATE2LDATE::FORTE_LDATE2LDATE(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_LDATE2LDATE::setInitialValues() {
  var_IN = 0_LDATE;
  var_OUT = 0_LDATE;
}

void FORTE_LDATE2LDATE::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_LDATE2LDATE::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_LDATE2LDATE::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_LDATE2LDATE::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_LDATE2LDATE::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_LDATE2LDATE::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_LDATE2LDATE::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_LDATE2LDATE::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_LDATE2LDATE::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_LDATE2LDATE::alg_REQ(void) {

  #line 2 "LDATE2LDATE.fbt"
  var_OUT = var_IN;
}

