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

const CStringDictionary::TStringId FORTE_WagoMaster::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdUpdateInterval};

const CStringDictionary::TStringId FORTE_WagoMaster::scm_anDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUINT };

const CStringDictionary::TStringId FORTE_WagoMaster::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_WagoMaster::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_WagoMaster::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_WagoMaster::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId FORTE_WagoMaster::scm_anEventInputNames[] = {g_nStringIdINIT};

const TDataIOID FORTE_WagoMaster::scm_anEOWith[] = {0, 1, 255, 1, 0, 255};
const TForteInt16 FORTE_WagoMaster::scm_anEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId FORTE_WagoMaster::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdIND};

const SAdapterInstanceDef FORTE_WagoMaster::scm_astAdapterInstances[] = {
{g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterOut, true }};

const SFBInterfaceSpec FORTE_WagoMaster::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  1,scm_astAdapterInstances};

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
