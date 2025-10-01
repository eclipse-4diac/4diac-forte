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

#pragma once

#include "OPCUA_MGR.h"
#include "RMT_DEV.h"

namespace forte::iec61499::system {
  class OPCUA_DEV : public RMT_DEV {
    public:
      OPCUA_MGR mOPCUAMgr;

      explicit OPCUA_DEV(std::string_view paMGRID = "localhost:61499");
      ~OPCUA_DEV() override = default;

      int startDevice() override;
  };
} // namespace forte::iec61499::system
