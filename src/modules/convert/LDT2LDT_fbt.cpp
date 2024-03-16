/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: LDT2LDT
 *** Description:
 *** Version:
 ***     1.0: 2023-04-28/Martin Jobst -  -
 *************************************************************************/

#include "LDT2LDT_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "LDT2LDT_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_ldate_and_time.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_LDT2LDT, g_nStringIdLDT2LDT)

const CStringDictionary::TStringId FORTE_LDT2LDT::scmDataInputNames[] = {g_nStringIdIN};
const CStringDictionary::TStringId FORTE_LDT2LDT::scmDataInputTypeIds[] = {g_nStringIdLDATE_AND_TIME};
const CStringDictionary::TStringId FORTE_LDT2LDT::scmDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId FORTE_LDT2LDT::scmDataOutputTypeIds[] = {g_nStringIdLDATE_AND_TIME};
const TDataIOID FORTE_LDT2LDT::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_LDT2LDT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_LDT2LDT::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_LDT2LDT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_LDT2LDT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_LDT2LDT::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_LDT2LDT::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_LDT2LDT::FORTE_LDT2LDT(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_LDT2LDT::setInitialValues() {
  var_IN = 0_LDATE_AND_TIME;
  var_OUT = 0_LDATE_AND_TIME;
}

void FORTE_LDT2LDT::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_LDT2LDT::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_LDT2LDT::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_LDT2LDT::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_LDT2LDT::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_LDT2LDT::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_LDT2LDT::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_LDT2LDT::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_LDT2LDT::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_LDT2LDT::alg_REQ(void) {

  #line 2 "LDT2LDT.fbt"
  var_OUT = var_IN;
}

