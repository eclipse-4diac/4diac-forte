/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: LTOD2LTOD
 *** Description:
 *** Version:
 ***     1.0: 2023-04-28/Martin Jobst -  -
 *************************************************************************/

#include "LTOD2LTOD_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "LTOD2LTOD_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_ltime_of_day.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_LTOD2LTOD, g_nStringIdLTOD2LTOD)

const CStringDictionary::TStringId FORTE_LTOD2LTOD::scmDataInputNames[] = {g_nStringIdIN};
const CStringDictionary::TStringId FORTE_LTOD2LTOD::scmDataInputTypeIds[] = {g_nStringIdLTIME_OF_DAY};
const CStringDictionary::TStringId FORTE_LTOD2LTOD::scmDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId FORTE_LTOD2LTOD::scmDataOutputTypeIds[] = {g_nStringIdLTIME_OF_DAY};
const TDataIOID FORTE_LTOD2LTOD::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_LTOD2LTOD::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_LTOD2LTOD::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_LTOD2LTOD::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_LTOD2LTOD::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_LTOD2LTOD::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_LTOD2LTOD::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_LTOD2LTOD::FORTE_LTOD2LTOD(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_LTOD2LTOD::setInitialValues() {
  var_IN = 0_LTIME_OF_DAY;
  var_OUT = 0_LTIME_OF_DAY;
}

void FORTE_LTOD2LTOD::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_LTOD2LTOD::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_LTOD2LTOD::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_LTOD2LTOD::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_LTOD2LTOD::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_LTOD2LTOD::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_LTOD2LTOD::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_LTOD2LTOD::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_LTOD2LTOD::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_LTOD2LTOD::alg_REQ(void) {

  #line 2 "LTOD2LTOD.fbt"
  var_OUT = var_IN;
}

