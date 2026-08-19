/*************************************************************************
 * Copyright (c) 2026 Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 *************************************************************************/

#include "WagoRegComDevice.h"
#include "forte/iec61131_functions/func_ADD.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::wago {

  WagoRegComDevice::WagoRegComDevice(int paType, CFBContainer &paContainer, const SFBInterfaceSpec &paInterfaceSpec, const forte::StringId paInstanceNameId)
      : WagoSlaveBase(paType, paContainer, paInterfaceSpec, paInstanceNameId),
        var_RegCom("RegCom"_STRID, *this, 2){
  }

  forte::ISocketPin *WagoRegComDevice::getSocketPinUnchecked(size_t paIndex) {
    return (paIndex == 1) ? &var_RegCom : WagoSlaveBase::getSocketPinUnchecked(paIndex);
  }

  void WagoRegComDevice::writeRegCom() {
    if (regComState == RegComStatus::Idle) {
      regComState = RegComStatus::Write;
      getController().writeRegComRequest(RegCom().var_cmd);
    } else {
      DEVLOG_DEBUG("[WagoRegComDevice::writeRegCom] Register communication state not Idle.\n");
    }
  }

  void WagoRegComDevice::readRegCom() {
    if (regComState == RegComStatus::Idle) {
      regComState = RegComStatus::ReadReq;
      getController().readRegComRequest(RegCom().var_cmd);
    } else {
      DEVLOG_DEBUG("[WagoRegComDevice::readRegCom] Register communication state not Idle.\n");
    }
  }

  void WagoRegComDevice::closeRegCom() {
    if (regComState == RegComStatus::Idle) {
      regComState = RegComStatus::ResetPassword;
      getController().writeRegComRequest(mResetPsw);
    } else {
      DEVLOG_DEBUG("[WagoRegComDevice::closeRegCom] Register communication state not Idle.\n");
    }
  }

  void WagoRegComDevice::openRegCom(CEventChainExecutionThread* const paECET) {
    if (regComState == RegComStatus::Init) {
      setEventChainExecutor(paECET);
      getController().enableRegCom(this);
    } else {
      DEVLOG_DEBUG("[WagoRegComDevice::openRegCom] Register communication state not Init.\n");
    }
  }

  void WagoRegComDevice::handleExternalEvent() {
    CEventChainExecutionThread* paECET = getEventChainExecutor();
    switch(regComState) {
    case RegComStatus::Read:
    case RegComStatus::Write:
      RegCom().var_counter = func_ADD(RegCom().var_counter, CIEC_USINT(1));
      regComState = RegComStatus::Idle;
      sendAdapterEvent(RegCom(), FORTE_WagoRegCom::scmEventCNFID, paECET);
      break;
    case RegComStatus::ReadReq:
      regComState = RegComStatus::Read;
      getController().readRegComResult(RegCom().var_rREG_D0, RegCom().var_rREG_D1);
      break;
    case RegComStatus::Init:
      regComState = RegComStatus::SetPassword;
      getController().writeRegComRequest(mSetPsw);
      break;
    case RegComStatus::SetPassword:
      regComState = RegComStatus::Idle;
      sendAdapterEvent(RegCom(), FORTE_WagoRegCom::scmEventOpenedID, paECET);
      break;
    case RegComStatus::ResetPassword:
      RegCom().var_counter = CIEC_USINT(0);
      regComState = RegComStatus::Deinit;
      getController().disableRegCom();
      break;
    case RegComStatus::Deinit:
      regComState = RegComStatus::Init;
      sendAdapterEvent(RegCom(), FORTE_WagoRegCom::scmEventClosedID, paECET);
      break;
    }
  }

} // namespace forte::eclipse4diac::io::wago
