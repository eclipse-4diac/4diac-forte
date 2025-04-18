/*******************************************************************************
 * Copyright (c) 2017, 2024 fortiss GmbH, Monika Wenger, Johannes Kepler University Linz
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v20.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Monika Wenger, Alois Zoitl - migration to current runtime
 *******************************************************************************/

#include "RevPiConfig.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(DigitalInput_1);
USE_STRING_ID(DigitalInput_10);
USE_STRING_ID(DigitalInput_11);
USE_STRING_ID(DigitalInput_12);
USE_STRING_ID(DigitalInput_13);
USE_STRING_ID(DigitalInput_14);
USE_STRING_ID(DigitalInput_2);
USE_STRING_ID(DigitalInput_3);
USE_STRING_ID(DigitalInput_4);
USE_STRING_ID(DigitalInput_5);
USE_STRING_ID(DigitalInput_6);
USE_STRING_ID(DigitalInput_7);
USE_STRING_ID(DigitalInput_8);
USE_STRING_ID(DigitalInput_9);
USE_STRING_ID(DigitalOutput_1);
USE_STRING_ID(DigitalOutput_10);
USE_STRING_ID(DigitalOutput_11);
USE_STRING_ID(DigitalOutput_12);
USE_STRING_ID(DigitalOutput_13);
USE_STRING_ID(DigitalOutput_14);
USE_STRING_ID(DigitalOutput_2);
USE_STRING_ID(DigitalOutput_3);
USE_STRING_ID(DigitalOutput_4);
USE_STRING_ID(DigitalOutput_5);
USE_STRING_ID(DigitalOutput_6);
USE_STRING_ID(DigitalOutput_7);
USE_STRING_ID(DigitalOutput_8);
USE_STRING_ID(DigitalOutput_9);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(IORevPi);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(STATUS);
USE_STRING_ID(UINT);
USE_STRING_ID(UpdateInterval);
USE_STRING_ID(WSTRING);

#include "RevPiController.h"

using namespace forte::core::io;

DEFINE_FIRMWARE_FB(RevPiConfig, STRID(IORevPi))

const CStringDictionary::TStringId RevPiConfig::scm_anDataInputNames[] = {STRID(QI),
                                                                          STRID(DigitalInput_1),
                                                                          STRID(DigitalInput_2),
                                                                          STRID(DigitalInput_3),
                                                                          STRID(DigitalInput_4),
                                                                          STRID(DigitalInput_5),
                                                                          STRID(DigitalInput_6),
                                                                          STRID(DigitalInput_7),
                                                                          STRID(DigitalInput_8),
                                                                          STRID(DigitalInput_9),
                                                                          STRID(DigitalInput_10),
                                                                          STRID(DigitalInput_11),
                                                                          STRID(DigitalInput_12),
                                                                          STRID(DigitalInput_13),
                                                                          STRID(DigitalInput_14),
                                                                          STRID(DigitalOutput_1),
                                                                          STRID(DigitalOutput_2),
                                                                          STRID(DigitalOutput_3),
                                                                          STRID(DigitalOutput_4),
                                                                          STRID(DigitalOutput_5),
                                                                          STRID(DigitalOutput_6),
                                                                          STRID(DigitalOutput_7),
                                                                          STRID(DigitalOutput_8),
                                                                          STRID(DigitalOutput_9),
                                                                          STRID(DigitalOutput_10),
                                                                          STRID(DigitalOutput_11),
                                                                          STRID(DigitalOutput_12),
                                                                          STRID(DigitalOutput_13),
                                                                          STRID(DigitalOutput_14),
                                                                          STRID(UpdateInterval)};

const CStringDictionary::TStringId RevPiConfig::scm_anDataInputTypeIds[] = {
    STRID(BOOL),    STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING),
    STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING),
    STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING),
    STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING),
    STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(UINT)};

const CStringDictionary::TStringId RevPiConfig::scm_anDataOutputNames[] = {STRID(QO), STRID(STATUS)};

const CStringDictionary::TStringId RevPiConfig::scm_anDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};

const TForteInt16 RevPiConfig::scm_anEIWithIndexes[] = {0};
const TDataIOID RevPiConfig::scm_anEIWith[] = {0,  29, 1,  2,  3,  4,  5,  6,  9,  8,  7,  10, 11, 12, 13, 14,
                                               15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 255};
const CStringDictionary::TStringId RevPiConfig::scm_anEventInputNames[] = {STRID(INIT)};
const CStringDictionary::TStringId RevPiConfig::scmEventInputTypeIds[] = {STRID(EInit)};

const TDataIOID RevPiConfig::scm_anEOWith[] = {0, 1, 255, 0, 1, 255};
const TForteInt16 RevPiConfig::scm_anEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId RevPiConfig::scm_anEventOutputNames[] = {STRID(INITO), STRID(IND)};
const CStringDictionary::TStringId RevPiConfig::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};

const SFBInterfaceSpec RevPiConfig::scm_stFBInterfaceSpec = {1,
                                                             scm_anEventInputNames,
                                                             scmEventInputTypeIds,
                                                             scm_anEIWith,
                                                             scm_anEIWithIndexes,
                                                             2,
                                                             scm_anEventOutputNames,
                                                             scmEventOutputTypeIds,
                                                             scm_anEOWith,
                                                             scm_anEOWithIndexes,
                                                             30,
                                                             scm_anDataInputNames,
                                                             scm_anDataInputTypeIds,
                                                             2,
                                                             scm_anDataOutputNames,
                                                             scm_anDataOutputTypeIds,
                                                             0,
                                                             0};

void RevPiConfig::setInitialValues() {
  UpdateInterval() = 25;
}

void RevPiConfig::setConfig() {
  RevPiController::Config config;
  config.updateInterval = UpdateInterval();
  controller->setConfig(&config);
}

IODeviceController *RevPiConfig::createDeviceController(CDeviceExecution &paDeviceExecution) {
  return new RevPiController(paDeviceExecution);
}
