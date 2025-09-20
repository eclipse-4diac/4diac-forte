/*************************************************************************
 * Copyright (c) 2016, 2025 fortiss GmbH, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#include "WagoSlaveBase.h"
#include "../WagoDeviceController.h"

using namespace forte::literals;

using namespace forte::io;

const TForteUInt8 WagoSlaveBase::scmSlaveConfigurationIO[] = {};
const TForteUInt8 WagoSlaveBase::scmSlaveConfigurationIONum = 0;

WagoSlaveBase::WagoSlaveBase(int paType,
                             CFBContainer &paContainer,
                             const SFBInterfaceSpec &paInterfaceSpec,
                             const forte::StringId paInstanceNameId) :
    IOConfigFBMultiSlave(
        scmSlaveConfigurationIO, scmSlaveConfigurationIONum, paType, paContainer, paInterfaceSpec, paInstanceNameId),
    var_BusAdapterOut("BusAdapterOut"_STRID, *this, 0),
    var_BusAdapterIn("BusAdapterIn"_STRID, *this, 0) {
}

void WagoSlaveBase::initWagoHandle(int paDIIndex,
                                   int paIOIndex,
                                   CIEC_ANY::EDataTypeID paType,
                                   IOMapper::Direction paDirection) {
  WagoDeviceController::WagoHandleDescriptor desc(static_cast<CIEC_STRING *>(getDI(paDIIndex))->getStorage(),
                                                  paDirection, mIndex, paType, static_cast<TForteUInt32>(paIOIndex));
  initHandle(desc);
}

void WagoSlaveBase::initHandlesBase(size_t paNumberOfBoolInputs,
                                    size_t paNumberOfBoolOutputs,
                                    size_t paNumberOfAnalogInputs,
                                    size_t paNumberOfAnalogOutputs) {
  size_t offset = 1; // skip QI

  for (size_t i = 0; i < paNumberOfBoolInputs; i++) {
    initWagoHandle(offset + i, i, CIEC_ANY::e_BOOL, IOMapper::In);
  }

  offset += paNumberOfBoolInputs;

  for (size_t i = 0; i < paNumberOfBoolOutputs; i++) {
    initWagoHandle(offset + i, i, CIEC_ANY::e_BOOL, IOMapper::Out);
  }

  offset += paNumberOfBoolOutputs;

  for (size_t i = 0; i < paNumberOfAnalogInputs; i++) {
    initWagoHandle(offset + i, i, CIEC_ANY::e_WORD, IOMapper::In);
  }

  offset += paNumberOfAnalogInputs;

  for (size_t i = 0; i < paNumberOfAnalogOutputs; i++) {
    initWagoHandle(offset + i, i, CIEC_ANY::e_WORD, IOMapper::Out);
  }
}

forte::IPlugPin *WagoSlaveBase::getPlugPinUnchecked(size_t paIndex) {
  return (paIndex == 0) ? &var_BusAdapterOut : nullptr;
}

forte::ISocketPin *WagoSlaveBase::getSocketPinUnchecked(size_t paIndex) {
  return (paIndex == 0) ? &var_BusAdapterIn : nullptr;
}
