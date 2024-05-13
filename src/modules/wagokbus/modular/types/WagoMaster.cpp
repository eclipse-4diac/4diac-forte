/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: WagoMaster
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2018-09-12/cabral - null -
 *************************************************************************/

#include "WagoMaster.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "WagoMaster_gen.cpp"
#endif

#include "../wagoDeviceController.h"

using namespace forte::core::io;

DEFINE_FIRMWARE_FB(FORTE_WagoMaster, g_nStringIdWagoMaster)

const CStringDictionary::TStringId FORTE_WagoMaster::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdUpdateInterval};
const CStringDictionary::TStringId FORTE_WagoMaster::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUINT};
const CStringDictionary::TStringId FORTE_WagoMaster::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};
const CStringDictionary::TStringId FORTE_WagoMaster::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};
const TDataIOID FORTE_WagoMaster::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_WagoMaster::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_WagoMaster::scmEventInputNames[] = {g_nStringIdINIT};
const TDataIOID FORTE_WagoMaster::scmEOWith[] = {0, 1, scmWithListDelimiter, 1, 0, scmWithListDelimiter};
const TForteInt16 FORTE_WagoMaster::scmEOWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_WagoMaster::scmEventOutputNames[] = {g_nStringIdINITO, g_nStringIdIND};
const SAdapterInstanceDef FORTE_WagoMaster::scmAdapterInstances[] = {
  {g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterOut, true}
};
const SFBInterfaceSpec FORTE_WagoMaster::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  1, scmAdapterInstances
};

FORTE_WagoMaster::FORTE_WagoMaster(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    IOConfigFBMultiMaster(paContainer, &scmFBInterfaceSpec, paInstanceNameId),
    var_UpdateInterval(25_UINT),
    var_conn_QO(var_QO),
    var_conn_STATUS(var_STATUS),
    conn_INITO(this, 0),
    conn_IND(this, 1),
    conn_QI(nullptr),
    conn_UpdateInterval(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_STATUS(this, 1, &var_conn_STATUS) {
};

void FORTE_WagoMaster::setInitialValues() {
  var_QI = 0_BOOL;
  var_UpdateInterval = 25_UINT;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_WagoMaster::setConfig() {
  WagoDeviceController::WagoConfig config;
  config.updateInterval = static_cast<CIEC_UINT::TValueType>(var_UpdateInterval);
  getDeviceController()->setConfig(&config);
}

forte::core::io::IODeviceController* FORTE_WagoMaster::createDeviceController(CDeviceExecution& paDeviceExecution) {
  return new WagoDeviceController(paDeviceExecution);
}

void FORTE_WagoMaster::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_UpdateInterval, conn_UpdateInterval);
      break;
    }
    default:
      break;
  }
}

void FORTE_WagoMaster::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      break;
    }
    case scmEventINDID: {
      writeData(1, var_STATUS, conn_STATUS);
      writeData(0, var_QO, conn_QO);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_WagoMaster::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_UpdateInterval;
  }
  return nullptr;
}

CIEC_ANY *FORTE_WagoMaster::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_WagoMaster::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_WagoMaster::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_UpdateInterval;
  }
  return nullptr;
}

CDataConnection *FORTE_WagoMaster::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
