/*******************************************************************************
 * Copyright (c) 2021, 2022 Jonathan Lainer (kontakt@lainer.co.at)
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *******************************************************************************/

#include "Port_fbt.h"

USE_STRING_ID(Pin0);
USE_STRING_ID(Pin1);
USE_STRING_ID(Pin10);
USE_STRING_ID(Pin11);
USE_STRING_ID(Pin12);
USE_STRING_ID(Pin13);
USE_STRING_ID(Pin14);
USE_STRING_ID(Pin15);
USE_STRING_ID(Pin2);
USE_STRING_ID(Pin3);
USE_STRING_ID(Pin4);
USE_STRING_ID(Pin5);
USE_STRING_ID(Pin6);
USE_STRING_ID(Pin7);
USE_STRING_ID(Pin8);
USE_STRING_ID(Pin9);
USE_STRING_ID(Port);
USE_STRING_ID(PortAdapter);
USE_STRING_ID(PortInAdapter);
USE_STRING_ID(WSTRING);


#include "PortAdapter_adp.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

#include "core/io/mapper/io_mapper.h"
#include <handler/EliteBoardDeviceController.h>
#include <handle/PinHandle.h>
#include <extevhandlerhelper.h>

using namespace forte::core::io;

DEFINE_FIRMWARE_FB(FORTE_Port, STRID(Port))

const CStringDictionary::TStringId FORTE_Port::scmDataInputNames[] = {STRID(Pin0), STRID(Pin1), STRID(Pin2), STRID(Pin3), STRID(Pin4), STRID(Pin5), STRID(Pin6), STRID(Pin7), STRID(Pin8), STRID(Pin9), STRID(Pin10), STRID(Pin11), STRID(Pin12), STRID(Pin13), STRID(Pin14), STRID(Pin15)};
const CStringDictionary::TStringId FORTE_Port::scmDataInputTypeIds[] = {STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING)};
const SAdapterInstanceDef FORTE_Port::scmAdapterInstances[] = {
  {STRID(PortAdapter), STRID(PortInAdapter), false}
};
const SFBInterfaceSpec FORTE_Port::scmFBInterfaceSpec = {
  0, nullptr, nullptr, nullptr, nullptr,
  0, nullptr, nullptr, nullptr, nullptr,
  16, scmDataInputNames, scmDataInputTypeIds,
  0, nullptr, nullptr,
  0, nullptr,
  1, scmAdapterInstances
};

FORTE_Port::FORTE_Port(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_Pin0(""_STRING),
    var_Pin1(""_STRING),
    var_Pin2(""_STRING),
    var_Pin3(""_STRING),
    var_Pin4(""_STRING),
    var_Pin5(""_STRING),
    var_Pin6(""_STRING),
    var_Pin7(""_STRING),
    var_Pin8(""_STRING),
    var_Pin9(""_STRING),
    var_Pin10(""_STRING),
    var_Pin11(""_STRING),
    var_Pin12(""_STRING),
    var_Pin13(""_STRING),
    var_Pin14(""_STRING),
    var_Pin15(""_STRING),
    var_PortInAdapter(STRID(PortInAdapter), *this, false),
    conn_Pin0(nullptr),
    conn_Pin1(nullptr),
    conn_Pin2(nullptr),
    conn_Pin3(nullptr),
    conn_Pin4(nullptr),
    conn_Pin5(nullptr),
    conn_Pin6(nullptr),
    conn_Pin7(nullptr),
    conn_Pin8(nullptr),
    conn_Pin9(nullptr),
    conn_Pin10(nullptr),
    conn_Pin11(nullptr),
    conn_Pin12(nullptr),
    conn_Pin13(nullptr),
    conn_Pin14(nullptr),
    conn_Pin15(nullptr) {
};

bool FORTE_Port::initialize() {
  if(!var_PortInAdapter.initialize()) { return false; }
  var_PortInAdapter.setParentFB(this, 0);
  for (int i = 0; i < pin_cnt; i++)
    mRegistered[i] = nullptr;
  return CFunctionBlock::initialize();
}

void FORTE_Port::setInitialValues() {
  var_Pin0 = ""_STRING;
  var_Pin1 = ""_STRING;
  var_Pin2 = ""_STRING;
  var_Pin3 = ""_STRING;
  var_Pin4 = ""_STRING;
  var_Pin5 = ""_STRING;
  var_Pin6 = ""_STRING;
  var_Pin7 = ""_STRING;
  var_Pin8 = ""_STRING;
  var_Pin9 = ""_STRING;
  var_Pin10 = ""_STRING;
  var_Pin11 = ""_STRING;
  var_Pin12 = ""_STRING;
  var_Pin13 = ""_STRING;
  var_Pin14 = ""_STRING;
  var_Pin15 = ""_STRING;
}

void FORTE_Port::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  if (paEIID == var_PortInAdapter.evt_MAP()) {
    deregister_handles();
    register_handles();

    sendAdapterEvent(0, FORTE_PortAdapter::scmEventMAPOID, paECET);
  }
}

void FORTE_Port::readInputData(TEventID) {
  // nothing to do
}

void FORTE_Port::writeOutputData(TEventID) {
  // nothing to do
}

CIEC_ANY *FORTE_Port::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_Pin0;
    case 1: return &var_Pin1;
    case 2: return &var_Pin2;
    case 3: return &var_Pin3;
    case 4: return &var_Pin4;
    case 5: return &var_Pin5;
    case 6: return &var_Pin6;
    case 7: return &var_Pin7;
    case 8: return &var_Pin8;
    case 9: return &var_Pin9;
    case 10: return &var_Pin10;
    case 11: return &var_Pin11;
    case 12: return &var_Pin12;
    case 13: return &var_Pin13;
    case 14: return &var_Pin14;
    case 15: return &var_Pin15;
  }
  return nullptr;
}

CIEC_ANY *FORTE_Port::getDO(size_t) {
  return nullptr;
}

CAdapter *FORTE_Port::getAdapterUnchecked(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_PortInAdapter;
  }
  return nullptr;
}

CEventConnection *FORTE_Port::getEOConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection **FORTE_Port::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_Pin0;
    case 1: return &conn_Pin1;
    case 2: return &conn_Pin2;
    case 3: return &conn_Pin3;
    case 4: return &conn_Pin4;
    case 5: return &conn_Pin5;
    case 6: return &conn_Pin6;
    case 7: return &conn_Pin7;
    case 8: return &conn_Pin8;
    case 9: return &conn_Pin9;
    case 10: return &conn_Pin10;
    case 11: return &conn_Pin11;
    case 12: return &conn_Pin12;
    case 13: return &conn_Pin13;
    case 14: return &conn_Pin14;
    case 15: return &conn_Pin15;
  }
  return nullptr;
}

CDataConnection *FORTE_Port::getDOConUnchecked(TPortId) {
  return nullptr;
}

void FORTE_Port::deregister_handles() {
  for (int i = 0; i < pin_cnt; i++) {
    if (mRegistered[i] != nullptr) {
      IOMapper::getInstance().deregisterHandle(std::string(*mRegistered[i]));
    }
  }
}

void FORTE_Port::register_handles() {
  uint32_t base = var_PortInAdapter.var_GPIO_Port_Addr();
  auto port = reinterpret_cast<GPIO_TypeDef *>(base);

  for (int i = 0; i < pin_cnt; i++) {
    auto *id = static_cast<CIEC_STRING *>(getDI(i));

    // Only initialize pins that have an id.
    if (id->empty())
      continue;

    mRegistered[i] = id;

    // Create a GPIO pin handle using the port struct to identify the MMIO port and
    // a bit mask to identify the pin.
    EliteBoardDeviceController::EliteBoardHandleDescriptor descr(*id, IOMapper::UnknownDirection, port, uint16_t(1 << i));
    EliteBoardDeviceController &ctrl = getExtEvHandler<EliteBoardDeviceController>(*this);

    IOMapper::getInstance().registerHandle(std::string(*id), ctrl.createIOHandle(descr));
  }
}
