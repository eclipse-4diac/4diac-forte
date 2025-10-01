/*******************************************************************************
 * Copyright (c) 2022, 2024 Markus Meingast, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Markus Meingast - initial implementation
 *******************************************************************************/

#include "OPCUA_DEV.h"
#include "forte/devicefactory.h"

using namespace forte::literals;

namespace forte::iec61499::system {
  namespace {
    [[maybe_unused]] const DeviceFactory::EntryImpl<OPCUA_DEV> entry("OPCUA_DEV"_STRID);
  } // namespace

  OPCUA_DEV::OPCUA_DEV(const std::string_view paMGRID) : RMT_DEV(paMGRID), mOPCUAMgr(*this) {
    RMT_DEV::changeExecutionState(EMGMCommandType::Reset);
  }

  int OPCUA_DEV::startDevice() {
    RMT_DEV::startDevice();
    if (mOPCUAMgr.initialize() != EMGMResponse::Ready) {
      return -1;
    }
    return 0;
  }
} // namespace forte::iec61499::system
