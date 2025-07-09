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

#include "EliteBoard_fbt.h"

USE_STRING_ID(EliteBoard);
USE_STRING_ID(MAP);
USE_STRING_ID(MAPO);
USE_STRING_ID(PortA);
USE_STRING_ID(PortAdapter);
USE_STRING_ID(PortB);
USE_STRING_ID(PortC);
USE_STRING_ID(PortD);
USE_STRING_ID(PortE);
USE_STRING_ID(PortF);
USE_STRING_ID(PortG);
USE_STRING_ID(PortH);
USE_STRING_ID(PortI);
USE_STRING_ID(PortJ);
USE_STRING_ID(PortK);

#include "PortAdapter_adp.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

#include <extevhandlerhelper.h>
#include "../handle/PinHandle.h"
#include "stm32h743xx.h"
#include "stm32h7xx_hal_gpio.h"

DEFINE_FIRMWARE_FB(FORTE_EliteBoard, STRID(EliteBoard))

namespace {
  const auto cEventInputNames = std::array{STRID(MAP)};
  const auto cEventOutputNames = std::array{STRID(MAPO)};
  const auto cPlugNameIds =
      std::array{STRID(PortA), STRID(PortB), STRID(PortC), STRID(PortD), STRID(PortE), STRID(PortF),
                 STRID(PortG), STRID(PortH), STRID(PortI), STRID(PortJ), STRID(PortK)};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = {},
      .mEONames = cEventOutputNames,
      .mEOTypeNames = {},
      .mDINames = {},
      .mDONames = {},
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = cPlugNameIds,
  };
} // namespace

FORTE_EliteBoard::FORTE_EliteBoard(const CStringDictionary::TStringId paInstanceNameId,
                                   forte::core::CFBContainer &paContainer) :
    IOConfigFBController(paContainer, cFBInterfaceSpec, paInstanceNameId),
    mEventHandler{getExtEvHandler<EliteBoardDeviceController>(*this)},
    var_PortA(STRID(PortA), *this, 0),
    var_PortB(STRID(PortB), *this, 1),
    var_PortC(STRID(PortC), *this, 2),
    var_PortD(STRID(PortD), *this, 3),
    var_PortE(STRID(PortE), *this, 4),
    var_PortF(STRID(PortF), *this, 5),
    var_PortG(STRID(PortG), *this, 6),
    var_PortH(STRID(PortH), *this, 7),
    var_PortI(STRID(PortI), *this, 8),
    var_PortJ(STRID(PortJ), *this, 9),
    var_PortK(STRID(PortK), *this, 10),
    conn_MAPO(*this, 0) {};

void FORTE_EliteBoard::setInitialValues() {
}

void FORTE_EliteBoard::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  // start port configuration
  if (paEIID == scmEventMAPID) {
    mCurrentAdapterIndex = 0;
    configPorts(paECET);
    return;
  }
  // continuing port configuration
  if (paEIID == getPortAdapterByIndex(mCurrentAdapterIndex - 1).evt_MAPO()) {
    configPorts(paECET);
    sendOutputEvent(scmEventMAPOID, paECET);
    return;
  }
}

void FORTE_EliteBoard::readInputData(TEventID) {
  // no data inputs
}

void FORTE_EliteBoard::writeOutputData(TEventID) {
  // no data outputs
}

CIEC_ANY *FORTE_EliteBoard::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *FORTE_EliteBoard::getDO(size_t) {
  return nullptr;
}

forte::IPlugPin *FORTE_EliteBoard::getPlugPinUnchecked(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_PortA;
    case 1: return &var_PortB;
    case 2: return &var_PortC;
    case 3: return &var_PortD;
    case 4: return &var_PortE;
    case 5: return &var_PortF;
    case 6: return &var_PortG;
    case 7: return &var_PortH;
    case 8: return &var_PortI;
    case 9: return &var_PortJ;
    case 10: return &var_PortK;
  }
  return nullptr;
}

CEventConnection *FORTE_EliteBoard::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_MAPO;
  }
  // unknown paIndex
  return nullptr;
}

CDataConnection **FORTE_EliteBoard::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_EliteBoard::getDOConUnchecked(TPortId) {
  return nullptr;
}

// The base addresses of the MMIO GPIO ports. Provided by the vendor.
constexpr uint32_t port_addresses[] = {
    GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE, GPIOE_BASE, GPIOF_BASE,
    GPIOG_BASE, GPIOH_BASE, GPIOI_BASE, GPIOJ_BASE, GPIOK_BASE,
};

FORTE_PortAdapter &FORTE_EliteBoard::getPortAdapterByIndex(int index) {
  switch (index) {
    case scmPortAAdpNum: return *var_PortA;
    case scmPortBAdpNum: return *var_PortB;
    case scmPortCAdpNum: return *var_PortC;
    case scmPortDAdpNum: return *var_PortD;
    case scmPortEAdpNum: return *var_PortE;
    case scmPortFAdpNum: return *var_PortF;
    case scmPortGAdpNum: return *var_PortG;
    case scmPortHAdpNum: return *var_PortH;
    case scmPortIAdpNum: return *var_PortI;
    case scmPortJAdpNum: return *var_PortJ;
    case scmPortKAdpNum: return *var_PortK;
  }
}

int FORTE_EliteBoard::configPorts(CEventChainExecutionThread *const paECET) {
  for (; mCurrentAdapterIndex < mAdapterCount; mCurrentAdapterIndex++) {
    if (FORTE_EliteBoard::configurePortFB(mCurrentAdapterIndex, paECET)) {
      mCurrentAdapterIndex++;
      return mCurrentAdapterIndex;
    }
  }
  return mCurrentAdapterIndex;
}

bool FORTE_EliteBoard::configurePortFB(int index, CEventChainExecutionThread *const paECET) {
  auto &adapter = getPortAdapterByIndex(index);
  if (adapter.getPeer() == nullptr) {
    // Adapter is not connected.
    return false;
  }

  adapter.var_GPIO_Port_Addr = CIEC_DWORD(port_addresses[index]);

  sendAdapterEvent(adapter, FORTE_PortAdapter::scmEventMAPID, paECET);
  return true;
}

// IOConfigFBController
IODeviceController *FORTE_EliteBoard::createDeviceController(CDeviceExecution &paDeviceExecution) {
  return &mEventHandler;
}
