/*******************************************************************************
 * Copyright (c) 2017, 2025 fortiss GmbH, Monika Wenger, Johannes Kepler University Linz
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

using namespace forte::literals;

#include "RevPiController.h"

namespace forte::eclipse4diac::io::revpi {

  DEFINE_FIRMWARE_FB(RevPiConfig, "IORevPi"_STRID)

  const forte::StringId RevPiConfig::scm_anDataInputNames[] = {"QI"_STRID,
                                                               "DigitalInput_1"_STRID,
                                                               "DigitalInput_2"_STRID,
                                                               "DigitalInput_3"_STRID,
                                                               "DigitalInput_4"_STRID,
                                                               "DigitalInput_5"_STRID,
                                                               "DigitalInput_6"_STRID,
                                                               "DigitalInput_7"_STRID,
                                                               "DigitalInput_8"_STRID,
                                                               "DigitalInput_9"_STRID,
                                                               "DigitalInput_10"_STRID,
                                                               "DigitalInput_11"_STRID,
                                                               "DigitalInput_12"_STRID,
                                                               "DigitalInput_13"_STRID,
                                                               "DigitalInput_14"_STRID,
                                                               "DigitalOutput_1"_STRID,
                                                               "DigitalOutput_2"_STRID,
                                                               "DigitalOutput_3"_STRID,
                                                               "DigitalOutput_4"_STRID,
                                                               "DigitalOutput_5"_STRID,
                                                               "DigitalOutput_6"_STRID,
                                                               "DigitalOutput_7"_STRID,
                                                               "DigitalOutput_8"_STRID,
                                                               "DigitalOutput_9"_STRID,
                                                               "DigitalOutput_10"_STRID,
                                                               "DigitalOutput_11"_STRID,
                                                               "DigitalOutput_12"_STRID,
                                                               "DigitalOutput_13"_STRID,
                                                               "DigitalOutput_14"_STRID,
                                                               "UpdateInterval"_STRID};

  const forte::StringId RevPiConfig::scm_anDataInputTypeIds[] = {
      "BOOL"_STRID,    "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID,
      "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID,
      "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID,
      "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID,
      "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "UINT"_STRID};

  const forte::StringId RevPiConfig::scm_anDataOutputNames[] = {"QO"_STRID, "STATUS"_STRID};

  const forte::StringId RevPiConfig::scm_anDataOutputTypeIds[] = {"BOOL"_STRID, "WSTRING"_STRID};

  const TForteInt16 RevPiConfig::scm_anEIWithIndexes[] = {0};
  const TDataIOID RevPiConfig::scm_anEIWith[] = {0,  29, 1,  2,  3,  4,  5,  6,  9,  8,  7,  10, 11, 12, 13, 14,
                                                 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 255};
  const forte::StringId RevPiConfig::scm_anEventInputNames[] = {"INIT"_STRID};
  const auto cEventInputTypeIds = std::array{"EInit"_STRID};

  const TDataIOID RevPiConfig::scm_anEOWith[] = {0, 1, 255, 0, 1, 255};
  const TForteInt16 RevPiConfig::scm_anEOWithIndexes[] = {0, 3, -1};
  const forte::StringId RevPiConfig::scm_anEventOutputNames[] = {"INITO"_STRID, "IND"_STRID};
  const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID};

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

} // namespace forte::eclipse4diac::io::revpi
