/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#include "ZephyrIOBase.h"
#include "handler/IODeviceController.h"

FORTE_ZephyrIOBase::FORTE_ZephyrIOBase(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId) :
    forte::core::io::IOConfigFBController(paContainer, paInterfaceSpec, paInstanceNameId) {
};

FORTE_ZephyrIOBase::~FORTE_ZephyrIOBase() {
  DEVLOG_INFO("FORTE_ZephyrIOBase dtor\n");
}

forte::core::io::IODeviceController* FORTE_ZephyrIOBase::createDeviceController(CDeviceExecution& paDeviceExecution) {
  return new IODeviceController(paDeviceExecution);
}

void FORTE_ZephyrIOBase::onStartup(CEventChainExecutionThread * const paECET) {
/*
  // Initialize handles
  for (size_t i = 0; i < FORTE_ZephyrIOBase::numberOfIOs; i++) {
    const auto id = std::string(*static_cast<CIEC_STRING*>(getDI(FORTE_ZephyrIOBase::initialDIOffset + i)));
    // do not use verbatim, only for reference to copy & paste in FBT instance class:
    const gpio_dt_spec* spec = nullptr;
    gpio_flags_t flags = 0;
    if (!spec) {
      DEVLOG_ERROR("FORTE_ZephyrIO::onStartup: id %s, device spec is null\n", id.c_str());
      continue;
    }
    IOHandleGPIODescriptor descr(id, IOMapper::Out, spec, flags);
    initHandle(descr);
  }
*/

  started(paECET);
}
