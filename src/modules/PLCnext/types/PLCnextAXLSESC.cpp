/*************************************************************************
 *** Copyright (c) 2022 Peirlberger Juergen
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: PLCnextAXLSESC
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2022-04-07/Peirlberger Juergen -  - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "PLCnextAXLSESC.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(BusAdapterIn);
USE_STRING_ID(BusAdapterOut);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(PLCnextAXLSESC);
USE_STRING_ID(PLCnextBusAdapter);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(STATUS);
USE_STRING_ID(WSTRING);


#include "PLCnextBusAdapter.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_PLCnextAXLSESC, STRID(PLCnextAXLSESC))

const CStringDictionary::TStringId FORTE_PLCnextAXLSESC::scmDataInputNames[] = {STRID(QI)};
const CStringDictionary::TStringId FORTE_PLCnextAXLSESC::scmDataInputTypeIds[] = {STRID(BOOL)};
const CStringDictionary::TStringId FORTE_PLCnextAXLSESC::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_PLCnextAXLSESC::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};
const TDataIOID FORTE_PLCnextAXLSESC::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_PLCnextAXLSESC::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_PLCnextAXLSESC::scmEventInputNames[] = {STRID(INIT)};
const CStringDictionary::TStringId FORTE_PLCnextAXLSESC::scmEventInputTypeIds[] = {STRID(EInit)};
const TDataIOID FORTE_PLCnextAXLSESC::scmEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_PLCnextAXLSESC::scmEOWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_PLCnextAXLSESC::scmEventOutputNames[] = {STRID(INITO), STRID(IND)};
const CStringDictionary::TStringId FORTE_PLCnextAXLSESC::scmEventOutputTypeIds[] = {STRID(EInit), STRID(Event)};
const SAdapterInstanceDef FORTE_PLCnextAXLSESC::scmAdapterInstances[] = {
  {STRID(PLCnextBusAdapter), STRID(BusAdapterIn), false},
  {STRID(PLCnextBusAdapter), STRID(BusAdapterOut), true}
};
const SFBInterfaceSpec FORTE_PLCnextAXLSESC::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  2, scmAdapterInstances
};

FORTE_PLCnextAXLSESC::FORTE_PLCnextAXLSESC(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    PLCnextSlaveHandler (PLCnextSlaveHandler::NoUsage, pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData),
    var_QI(0_BOOL),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
    var_BusAdapterIn(STRID(BusAdapterIn), *this, false),
    var_BusAdapterOut(STRID(BusAdapterOut), *this, true),
    var_conn_QO(var_QO),
    var_conn_STATUS(var_STATUS),
    conn_INITO(this, 0),
    conn_IND(this, 1),
    conn_QI(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_STATUS(this, 1, &var_conn_STATUS) {
};

bool FORTE_PLCnextAXLSESC::initialize() {
  if(!var_BusAdapterIn.initialize()) { return false; }
  if(!var_BusAdapterOut.initialize()) { return false; }
  var_BusAdapterIn.setParentFB(this, 0);
  var_BusAdapterOut.setParentFB(this, 1);
  return CFunctionBlock::initialize();
}

void FORTE_PLCnextAXLSESC::setInitialValues() {
  var_QI = 0_BOOL;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_PLCnextAXLSESC::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      break;
    }
    default:
      break;
  }
}

void FORTE_PLCnextAXLSESC::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      break;
    }
    case scmEventINDID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_PLCnextAXLSESC::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
  }
  return nullptr;
}

CIEC_ANY *FORTE_PLCnextAXLSESC::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CAdapter *FORTE_PLCnextAXLSESC::getAdapterUnchecked(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_BusAdapterIn;
    case 1: return &var_BusAdapterOut;
  }
  return nullptr;
}

CEventConnection *FORTE_PLCnextAXLSESC::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_PLCnextAXLSESC::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
  }
  return nullptr;
}

CDataConnection *FORTE_PLCnextAXLSESC::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}

void FORTE_PLCnextAXLSESC::initHandles() {

}

const char* FORTE_PLCnextAXLSESC::init(){
    PLCnextDeviceController &controller = *static_cast<PLCnextDeviceController*>(&getController());
    controller.registerSlaveHandler(this);
    return 0;
}
