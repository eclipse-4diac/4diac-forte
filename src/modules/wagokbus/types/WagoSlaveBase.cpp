/*************************************************************************
 * Copyright (c) 2016, 2024 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#include "WagoSlaveBase.h"

#include "../WagoDeviceController.h"

using namespace forte::core::io;

const TForteUInt8 WagoSlaveBase::scmSlaveConfigurationIO[] = { };
const TForteUInt8 WagoSlaveBase::scmSlaveConfigurationIONum = 0;

WagoSlaveBase::WagoSlaveBase(int paType, forte::core::CFBContainer &paContainer, const SFBInterfaceSpec& paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId) :
        IOConfigFBMultiSlave(scmSlaveConfigurationIO, scmSlaveConfigurationIONum, paType, paContainer, paInterfaceSpec, paInstanceNameId) {
}

void WagoSlaveBase::initWagoHandle (int paDIIndex, int paIOIndex, CIEC_ANY::EDataTypeID paType, forte::core::io::IOMapper::Direction paDirection) {
  WagoDeviceController::WagoHandleDescriptor desc(
      static_cast<CIEC_STRING*>(getDI(paDIIndex))->getStorage(), paDirection, mIndex,
      paType, static_cast<TForteUInt32>(paIOIndex));
  initHandle(desc);
}

void WagoSlaveBase::initHandlesBase(size_t paNumberOfBoolInputs, size_t paNumberOfBoolOutputs, size_t paNumberOfAnalogInputs, size_t paNumberOfAnalogOutputs) {
  size_t offset = 1; //skip QI

  for(size_t i = 0; i < paNumberOfBoolInputs; i++) {
    initWagoHandle(offset + i, i, CIEC_ANY::e_BOOL, IOMapper::In);
  }

  offset += paNumberOfBoolInputs;

  for(size_t i = 0; i < paNumberOfBoolOutputs; i++) {
    initWagoHandle(offset + i, i, CIEC_ANY::e_BOOL, IOMapper::Out);
  }

  offset += paNumberOfBoolOutputs;

  for(size_t i = 0; i < paNumberOfAnalogInputs; i++) {
    initWagoHandle(offset + i, i, CIEC_ANY::e_WORD, IOMapper::In);
  }

  offset += paNumberOfAnalogInputs;

  for(size_t i = 0; i < paNumberOfAnalogOutputs; i++) {
    initWagoHandle(offset + i, i, CIEC_ANY::e_WORD, IOMapper::Out);
  }
}
