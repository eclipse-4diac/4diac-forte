/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: Wago1506
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     0.0: 2016-11-30/4DIAC-IDE - 4DIAC-Consortium -
 *************************************************************************/

#include "wagoSlaveBase.h"
#include "../wagoDeviceController.h"

using namespace forte::core::io;

const TForteUInt8 WagoSlaveBase::scmSlaveConfigurationIO[] = { };
const TForteUInt8 WagoSlaveBase::scmSlaveConfigurationIONum = 0;

WagoSlaveBase::WagoSlaveBase(int paType, forte::core::CFBContainer &paContainer, const SFBInterfaceSpec* paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId) :
        IOConfigFBMultiSlave(scmSlaveConfigurationIO, scmSlaveConfigurationIONum, paType, paContainer, paInterfaceSpec, paInstanceNameId) {
}

void WagoSlaveBase::initHandlesBase(size_t paNumberOfBoolInputs, size_t paNumberOfBoolOutputs, size_t paNumberOfAnalogInputs, size_t paNumberOfAnalogOutputs) {
  size_t offset = 1; //skip QI

  for(size_t i = 0; i < paNumberOfBoolInputs; i++) {
    WagoDeviceController::WagoHandleDescriptor desc(static_cast<CIEC_STRING*>(getDI(offset + i))->getStorage(),
      IOMapper::In, mIndex, CIEC_ANY::e_BOOL, static_cast<TForteUInt32>(i));
    initHandle(desc);
  }

  offset += paNumberOfBoolInputs;

  for(size_t i = 0; i < paNumberOfBoolOutputs; i++) {
    WagoDeviceController::WagoHandleDescriptor desc(static_cast<CIEC_STRING*>(getDI(offset + i))->getStorage(),
      IOMapper::Out, mIndex, CIEC_ANY::e_BOOL, static_cast<TForteUInt32>(i));
    initHandle(desc);
  }

  offset += paNumberOfBoolOutputs;

  for(size_t i = 0; i < paNumberOfAnalogInputs; i++) {
    WagoDeviceController::WagoHandleDescriptor desc(static_cast<CIEC_STRING*>(getDI(offset + i))->getStorage(),
      IOMapper::In, mIndex, CIEC_ANY::e_WORD, static_cast<TForteUInt32>(i));
    initHandle(desc);
  }

  offset += paNumberOfAnalogInputs;

  for(size_t i = 0; i < paNumberOfAnalogOutputs; i++) {
    WagoDeviceController::WagoHandleDescriptor desc(static_cast<CIEC_STRING*>(getDI(offset + i))->getStorage(),
      IOMapper::Out, mIndex, CIEC_ANY::e_WORD, static_cast<TForteUInt32>(i));
    initHandle(desc);
  }
}
