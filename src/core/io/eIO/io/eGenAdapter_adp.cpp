/*******************************************************************************
 * Copyright (c) 2025 Maximilian Scharf
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Maximilian Scharf - Initial implementation.
 *******************************************************************************/

#include "eGenAdapter_adp.h"

#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(eGenAdapter);

DEFINE_ADAPTER_TYPE(FORTE_eGenAdapter, STRID(eGenAdapter))


const SFBInterfaceSpec FORTE_eGenAdapter::scmFBInterfaceSpecSocket = {
  0, nullptr, nullptr, nullptr, nullptr,
  0, nullptr, nullptr, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

const SFBInterfaceSpec FORTE_eGenAdapter::scmFBInterfaceSpecPlug = {
  0, nullptr, nullptr, nullptr, nullptr,
  0, nullptr, nullptr, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

void FORTE_eGenAdapter::readInputData(TEventID) {
  if(isSocket()) {
    // nothing to do
  } else {
    // nothing to do
  }
}

void FORTE_eGenAdapter::writeOutputData(TEventID) {
  if(isSocket()) {
    // nothing to do
  } else {
    // nothing to do
  }
}