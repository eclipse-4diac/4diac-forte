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

DEFINE_FIRMWARE_FB(FORTE_WagoMaster, g_nStringIdWagoMaster)

const CStringDictionary::TStringId FORTE_WagoMaster::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdUpdateInterval};

const CStringDictionary::TStringId FORTE_WagoMaster::scmDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUINT };

const CStringDictionary::TStringId FORTE_WagoMaster::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_WagoMaster::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_WagoMaster::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_WagoMaster::scmEIWith[] = {0, 1, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_WagoMaster::scmEventInputNames[] = {g_nStringIdINIT};

const TDataIOID FORTE_WagoMaster::scmEOWith[] = {0, 1, scmWithListDelimiter, 1, 0, scmWithListDelimiter};
const TForteInt16 FORTE_WagoMaster::scmEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId FORTE_WagoMaster::scmEventOutputNames[] = {g_nStringIdINITO, g_nStringIdIND};

const SAdapterInstanceDef FORTE_WagoMaster::scmAdapterInstances[] = {
{g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterOut, true }};

const SFBInterfaceSpec FORTE_WagoMaster::scmFBInterfaceSpec = {
  1,  scmEventInputNames,  scmEIWith,  scmEIWithIndexes,
  2,  scmEventOutputNames,  scmEOWith, scmEOWithIndexes,  2,  scmDataInputNames, scmDataInputTypeIds,
  2,  scmDataOutputNames, scmDataOutputTypeIds,
  1,scmAdapterInstances};

void FORTE_WagoMaster::setInitialValues(){
  UpdateInterval() = 25;
}

void FORTE_WagoMaster::setConfig() {
  WagoDeviceController::WagoConfig config;
  config.updateInterval = UpdateInterval();
  getDeviceController()->setConfig(&config);
}

forte::core::io::IODeviceController* FORTE_WagoMaster::createDeviceController(CDeviceExecution& paDeviceExecution) {
  return new WagoDeviceController(paDeviceExecution);
}
