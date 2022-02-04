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

#include <handle/PinHandle.h>
#include <extevhandlerhelper.h>
#include <stm32h743xx.h>
#include <stm32h7xx_hal_gpio.h>

DEFINE_FIRMWARE_FB(FORTE_EliteBoard, g_nStringIdEliteBoard)

const TForteInt16 FORTE_EliteBoard::scm_anEIWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_EliteBoard::scm_anEventInputNames[] = {
    g_nStringIdMAP};

const TForteInt16 FORTE_EliteBoard::scm_anEOWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_EliteBoard::scm_anEventOutputNames[] =
    {g_nStringIdMAPO};

const SAdapterInstanceDef FORTE_EliteBoard::scm_astAdapterInstances[] = {
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
    {g_nStringIdPortAdapter, g_nStringIdPortK, true}};

const SFBInterfaceSpec FORTE_EliteBoard::scm_stFBInterfaceSpec = {
    1,       scm_anEventInputNames,
    nullptr, scm_anEIWithIndexes,
    1,       scm_anEventOutputNames,
    nullptr, scm_anEOWithIndexes,
    0,       nullptr,
    nullptr, 0,
    nullptr, nullptr,
    11,      scm_astAdapterInstances};

FORTE_EliteBoard::FORTE_EliteBoard(const CStringDictionary::TStringId pa_nInstanceNameId,
                 CResource *pa_poSrcRes)
    : forte::core::io::IOConfigFBController(pa_poSrcRes, &scm_stFBInterfaceSpec,
                                            pa_nInstanceNameId, m_anFBConnData,
                                            m_anFBVarsData),
      mEventHandler{getExtEvHandler<EliteBoardDeviceController>(*this)} {};

forte::core::io::IODeviceController *
FORTE_EliteBoard::createDeviceController(CDeviceExecution &paDeviceExecution) {
  return &mEventHandler;
}

// The base addresses of the MMIO GPIO ports. Provided by the vendor.
constexpr uint32_t port_addresses[] = {
    GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE, GPIOE_BASE, GPIOF_BASE,
    GPIOG_BASE, GPIOH_BASE, GPIOI_BASE, GPIOJ_BASE, GPIOK_BASE,
};

FORTE_PortAdapter &FORTE_EliteBoard::getPortAdapterByIndex(int index) {
  return (*static_cast<FORTE_PortAdapter *>(m_apoAdapters[index]));
}

bool FORTE_EliteBoard::configurePortFB(int index) {
  auto &adapter = getPortAdapterByIndex(index);
  if (adapter.getPeer() == nullptr) {
    // Adapter is not connected.
    return false;
  }

  adapter.GPIO_Port_Addr() = port_addresses[index];

  sendAdapterEvent(index, FORTE_PortAdapter::scm_nEventMAPID);
  return true;
}

void FORTE_EliteBoard::executeEvent(int pa_nEIID) {

  if (pa_nEIID == scm_nEventMAPID) {
    // start initialisation chain
    mCurrentAdapterIndex = 0;

    while (mCurrentAdapterIndex < mAdapterCount &&
           !configurePortFB(mCurrentAdapterIndex)) {
      mCurrentAdapterIndex++;
    }

    mCurrentAdapterIndex++;
  } else if (pa_nEIID == getPortAdapterByIndex(mCurrentAdapterIndex).MAPO()) {
    // previous adapter finished initialisation
    if (mCurrentAdapterIndex < mAdapterCount) {
      // not completed yet

      while (mCurrentAdapterIndex < mAdapterCount &&
             !configurePortFB(mCurrentAdapterIndex)) {
        mCurrentAdapterIndex++;
      }

      if (mCurrentAdapterIndex >= mAdapterCount) {
        // initialisation complete
        sendOutputEvent(scm_nEventMAPOID);
      } else {
        // not completed yet
        mCurrentAdapterIndex++;
      }
    } else {
      // initialisation complete
      sendOutputEvent(scm_nEventMAPOID);
    }
  }
}
