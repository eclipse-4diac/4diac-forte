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

OPCUA_DEV::OPCUA_DEV(const std::string &paMGRID) :
  RMT_DEV(paMGRID), mOPCUAMgr(*this){
  changeExecutionState(EMGMCommandType::Reset);
}

OPCUA_DEV::~OPCUA_DEV() {
}

int OPCUA_DEV::startDevice(void) {
  RMT_DEV::startDevice();
  if (mOPCUAMgr.initialize() != EMGMResponse::Ready) {
    return -1;
  }
  return 0;
}
