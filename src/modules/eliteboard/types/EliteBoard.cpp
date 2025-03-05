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

#include "EliteBoard.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "EliteBoard_gen.cpp"
#endif

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

DEFINE_FIRMWARE_FB(FORTE_EliteBoard, g_nStringIdEliteBoard)

const TForteInt16 FORTE_EliteBoard::scmEIWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_EliteBoard::scmEventInputNames[] = {g_nStringIdMAP};
const TForteInt16 FORTE_EliteBoard::scmEOWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_EliteBoard::scmEventOutputNames[] = {g_nStringIdMAPO};
const SAdapterInstanceDef FORTE_EliteBoard::scmAdapterInstances[] = {
  {g_nStringIdPortAdapter, g_nStringIdPortA, true},
  {g_nStringIdPortAdapter, g_nStringIdPortB, true},
  {g_nStringIdPortAdapter, g_nStringIdPortC, true},
  {g_nStringIdPortAdapter, g_nStringIdPortD, true},
  {g_nStringIdPortAdapter, g_nStringIdPortE, true},
  {g_nStringIdPortAdapter, g_nStringIdPortF, true},
  {g_nStringIdPortAdapter, g_nStringIdPortG, true},
  {g_nStringIdPortAdapter, g_nStringIdPortH, true},
  {g_nStringIdPortAdapter, g_nStringIdPortI, true},
  {g_nStringIdPortAdapter, g_nStringIdPortJ, true},
  {g_nStringIdPortAdapter, g_nStringIdPortK, true}
};
const SFBInterfaceSpec FORTE_EliteBoard::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, nullptr, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, nullptr, scmEOWithIndexes,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr,
  11, scmAdapterInstances
};

FORTE_EliteBoard::FORTE_EliteBoard(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_PortA(g_nStringIdPortA, *this, true),
    var_PortB(g_nStringIdPortB, *this, true),
    var_PortC(g_nStringIdPortC, *this, true),
    var_PortD(g_nStringIdPortD, *this, true),
    var_PortE(g_nStringIdPortE, *this, true),
    var_PortF(g_nStringIdPortF, *this, true),
    var_PortG(g_nStringIdPortG, *this, true),
    var_PortH(g_nStringIdPortH, *this, true),
    var_PortI(g_nStringIdPortI, *this, true),
    var_PortJ(g_nStringIdPortJ, *this, true),
    var_PortK(g_nStringIdPortK, *this, true),
    conn_MAPO(this, 0) {
};

bool FORTE_EliteBoard::initialize() {
  if(!var_PortA.initialize()) { return false; }
  if(!var_PortB.initialize()) { return false; }
  if(!var_PortC.initialize()) { return false; }
  if(!var_PortD.initialize()) { return false; }
  if(!var_PortE.initialize()) { return false; }
  if(!var_PortF.initialize()) { return false; }
  if(!var_PortG.initialize()) { return false; }
  if(!var_PortH.initialize()) { return false; }
  if(!var_PortI.initialize()) { return false; }
  if(!var_PortJ.initialize()) { return false; }
  if(!var_PortK.initialize()) { return false; }
  var_PortA.setParentFB(this, 0);
  var_PortB.setParentFB(this, 1);
  var_PortC.setParentFB(this, 2);
  var_PortD.setParentFB(this, 3);
  var_PortE.setParentFB(this, 4);
  var_PortF.setParentFB(this, 5);
  var_PortG.setParentFB(this, 6);
  var_PortH.setParentFB(this, 7);
  var_PortI.setParentFB(this, 8);
  var_PortJ.setParentFB(this, 9);
  var_PortK.setParentFB(this, 10);
  return CFunctionBlock::initialize();
}


void FORTE_EliteBoard::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  // start port configuration
  if (paEIID == scmEventMAPID) {
      mCurrentAdapterIndex = 0;
      configPorts(paECET);
      return;
  }
  // continuing port configuration
  if (paEIID == getPortAdapterByIndex(mCurrentAdapterIndex-1).evt_MAPO()) {
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

CAdapter *FORTE_EliteBoard::getAdapterUnchecked(const size_t paIndex) {
  switch(paIndex) {
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
  switch(paIndex) {
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
    case scmPortAAdpNum:
      return var_PortA;
    case scmPortBAdpNum:
      return var_PortB;
    case scmPortCAdpNum:
      return var_PortC;
    case scmPortDAdpNum:
      return var_PortD;
    case scmPortEAdpNum:
      return var_PortE;
    case scmPortFAdpNum:
      return var_PortF;
    case scmPortGAdpNum:
      return var_PortG;
    case scmPortHAdpNum:
      return var_PortH;
    case scmPortIAdpNum:
      return var_PortI;
    case scmPortJAdpNum:
      return var_PortJ;
    case scmPortKAdpNum:
      return var_PortK;
  }
}

int FORTE_EliteBoard::configPorts(CEventChainExecutionThread *const paECET) {
  for (;mCurrentAdapterIndex < mAdapterCount; mCurrentAdapterIndex++) {
    if(FORTE_EliteBoard::configurePortFB(mCurrentAdapterIndex, paECET)) {
      mCurrentAdapterIndex++;
      return mCurrentAdapterIndex;
    }
  }
  return mCurrentAdapterIndex;
}

bool FORTE_EliteBoard::configurePortFB(int index, CEventChainExecutionThread * const paECET) {
  auto &adapter = getPortAdapterByIndex(index);
  if (adapter.getPeer() == nullptr) {
    // Adapter is not connected.
    return false;
  }

  adapter.var_GPIO_Port_Addr() = static_cast<CIEC_DWORD>(port_addresses[index]);

  sendAdapterEvent(index, FORTE_PortAdapter::scmEventMAPID, paECET);
  return true;
}
