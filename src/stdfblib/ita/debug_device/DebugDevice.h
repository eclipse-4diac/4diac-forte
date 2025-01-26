/*******************************************************************************
 * Copyright (c) 2025 Jose Cabral
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial implementation
 *******************************************************************************/

#pragma once

#include "RMT_DEV.h"
#include "DebugMGR.h"

/**
 * @brief Device that adds debug commands to the device. The commands are defined in DebugMGR
 * 
 */
class DebugDevice : public RMT_DEV {
public:

  DebugDevice(const std::string &paMGRID = "localhost:61499");
  ~DebugDevice() override = default;

  int startDevice() override;

  DebugMGR mDebugMgr;
};
