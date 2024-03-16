/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: DT2DT
 *** Description:
 *** Version:
 ***     1.0: 2023-04-28/Martin Jobst -  -
 *************************************************************************/

#include "DT2DT_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "DT2DT_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_date_and_time.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_DT2DT, g_nStringIdDT2DT)

const CStringDictionary::TStringId FORTE_DT2DT::scmDataInputNames[] = {g_nStringIdIN};
const CStringDictionary::TStringId FORTE_DT2DT::scmDataInputTypeIds[] = {g_nStringIdDATE_AND_TIME};
const CStringDictionary::TStringId FORTE_DT2DT::scmDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId FORTE_DT2DT::scmDataOutputTypeIds[] = {g_nStringIdDATE_AND_TIME};
const TDataIOID FORTE_DT2DT::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_DT2DT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_DT2DT::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_DT2DT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_DT2DT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_DT2DT::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_DT2DT::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_DT2DT::FORTE_DT2DT(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_DT2DT::setInitialValues() {
  var_IN = 0_DATE_AND_TIME;
  var_OUT = 0_DATE_AND_TIME;
}

void FORTE_DT2DT::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_DT2DT::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_DT2DT::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_DT2DT::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_DT2DT::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_DT2DT::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_DT2DT::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_DT2DT::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_DT2DT::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_DT2DT::alg_REQ(void) {

  #line 2 "DT2DT.fbt"
  var_OUT = var_IN;
}

