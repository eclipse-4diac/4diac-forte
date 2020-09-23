/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "plc01a1_config_fb.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "plc01a1_config_fb_gen.cpp"
#endif

#include "plc01a1_controller.h"

DEFINE_FIRMWARE_FB(PLC01A1ConfigFB, g_nStringIdPLC01A1)



const CStringDictionary::TStringId PLC01A1ConfigFB::scm_anDataInputNames[] = { g_nStringIdQI, g_nStringIdIN1, g_nStringIdIN2, g_nStringIdIN3, g_nStringIdIN4,
  g_nStringIdIN5, g_nStringIdIN6, g_nStringIdIN7, g_nStringIdIN8, g_nStringIdOUT1, g_nStringIdOUT2, g_nStringIdOUT3, g_nStringIdOUT4, g_nStringIdOUT5,
  g_nStringIdOUT6, g_nStringIdOUT7, g_nStringIdOUT8, g_nStringIdUpdateInterval };

const CStringDictionary::TStringId PLC01A1ConfigFB::scm_anDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
  g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
  g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdUINT };

const CStringDictionary::TStringId PLC01A1ConfigFB::scm_anDataOutputNames[] = { g_nStringIdQO, g_nStringIdSTATUS };

const CStringDictionary::TStringId PLC01A1ConfigFB::scm_anDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdWSTRING };

const TForteInt16 PLC01A1ConfigFB::scm_anEIWithIndexes[] = { 0 };
const TDataIOID PLC01A1ConfigFB::scm_anEIWith[] = { 0, 17, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 255 };
const CStringDictionary::TStringId PLC01A1ConfigFB::scm_anEventInputNames[] = { g_nStringIdINIT };
const TDataIOID PLC01A1ConfigFB::scm_anEOWith[] = { 0, 1, 255, 0, 1, 255 };
const TForteInt16 PLC01A1ConfigFB::scm_anEOWithIndexes[] = { 0, 3, -1 };
const CStringDictionary::TStringId PLC01A1ConfigFB::scm_anEventOutputNames[] = { g_nStringIdINITO, g_nStringIdIND };
const SFBInterfaceSpec PLC01A1ConfigFB::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  18,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void PLC01A1ConfigFB::setInitialValues() {
  UpdateInterval() = 25;
}

forte::core::io::IODeviceController* PLC01A1ConfigFB::createDeviceController(CDeviceExecution &paDeviceExecution) {
  return new PLC01A1Controller(paDeviceExecution);
}

void PLC01A1ConfigFB::setConfig() {
  PLC01A1Controller::Config config;
  config.mUpdateInterval = UpdateInterval();
  getDeviceController()->setConfig(&config);
}

void PLC01A1ConfigFB::onStartup() {
  // Initialize handles
  size_t initialDIOffset = 1;
  size_t numberOfInputs = 8;
  size_t numberOfOutputs = 8;

  for(size_t i = 0; i < numberOfInputs; i++) {
    PLC01A1Controller::HandleDescriptor desc = PLC01A1Controller::HandleDescriptor(*static_cast<CIEC_WSTRING*>(getDI(initialDIOffset + i)),
      forte::core::io::IOMapper::In,
      0 /*offset is always 0 */,
      static_cast<uint8_t>(i));
    initHandle(&desc);
  }

  for(size_t i = 0; i < numberOfOutputs; i++) {
    PLC01A1Controller::HandleDescriptor
    desc = PLC01A1Controller::HandleDescriptor(*static_cast<CIEC_WSTRING*>(getDI(initialDIOffset + numberOfInputs + i)),
      forte::core::io::IOMapper::Out, 0 /*offset is always 0 */, static_cast<uint8_t>(numberOfOutputs - i - 1));
    initHandle(&desc);
  }

  started();
}



