/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "IORevPiCore.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "IORevPiCore_gen.cpp"
#endif

#include "../RevPiController.h"

DEFINE_FIRMWARE_FB(FORTE_IORevPiCore, g_nStringIdIORevPiCore)

const CStringDictionary::TStringId FORTE_IORevPiCore::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdUpdateInterval};

const CStringDictionary::TStringId FORTE_IORevPiCore::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUINT};

const CStringDictionary::TStringId FORTE_IORevPiCore::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_IORevPiCore::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_IORevPiCore::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_IORevPiCore::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId FORTE_IORevPiCore::scm_anEventInputNames[] = {g_nStringIdINIT};

const TDataIOID FORTE_IORevPiCore::scm_anEOWith[] = {0, 1, 255, 0, 1, 255};
const TForteInt16 FORTE_IORevPiCore::scm_anEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId FORTE_IORevPiCore::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdIND};

const SAdapterInstanceDef FORTE_IORevPiCore::scm_astAdapterInstances[] = {
{g_nStringIdIORevPiBusAdapter, g_nStringIdBusAdapterOut, true }};

const SFBInterfaceSpec FORTE_IORevPiCore::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  1,scm_astAdapterInstances};


void FORTE_IORevPiCore::setInitialValues(){
  UpdateInterval() = 25;
}

void FORTE_IORevPiCore::setConfig() {
  RevPiController::Config config;
  config.UpdateInterval = UpdateInterval();
  controller->setConfig(&config);
}

forte::core::IO::IODeviceController* FORTE_IORevPiCore::createDeviceController(CDeviceExecution& paDeviceExecution) {
  return new RevPiController(paDeviceExecution);
}



