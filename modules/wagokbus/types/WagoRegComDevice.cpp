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
#include "WagoRegComCmd_dtp.h"
#include "forte/iec61131_functions/func_ADD.h"
#include "forte/iec61131_functions/func_GT.h"
#include "forte/datatypes/forte_wstring.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::wago {

  const CIEC_WagoRegComCmd WagoRegComDevice::mSetPsw{CIEC_USINT(31), CIEC_BYTE(0x35), CIEC_BYTE(0x12)};
  const CIEC_WagoRegComCmd WagoRegComDevice::mResetPsw{CIEC_USINT(31), CIEC_BYTE(0x00), CIEC_BYTE(0x00)};

  const CIEC_WSTRING WagoRegComDevice::scmRegNrOutOfRange("Register number out of range (0-31).");
  const CIEC_WSTRING WagoRegComDevice::scmOK("OK.");
  const CIEC_WSTRING WagoRegComDevice::scmEnabled("Register communication enabled.");
  const CIEC_WSTRING WagoRegComDevice::scmDisabled("Register communication disabled.");

  const CIEC_WSTRING WagoRegComDevice::scmRegComOpenFail("Failed to open register communication.");
  const CIEC_WSTRING WagoRegComDevice::scmRegComReqFail("Failed request of register communication.");

  WagoRegComDevice::WagoRegComDevice(int paType, CFBContainer &paContainer, const SFBInterfaceSpec &paInterfaceSpec, const forte::StringId paInstanceNameId)
      : WagoSlaveBase(paType, paContainer, paInterfaceSpec, paInstanceNameId),
        var_RegCom("RegCom"_STRID, *this, 2){
  }

  forte::ISocketPin *WagoRegComDevice::getSocketPinUnchecked(size_t paIndex) {
    return (paIndex == 1) ? &var_RegCom : WagoSlaveBase::getSocketPinUnchecked(paIndex);
  }

  void WagoRegComDevice::writeRegCom() {
    if (checkRegNr() && mRegComState == RegComStatus::Idle) {
      if(getController().writeRegComRequest(RegCom().var_cmd)) {
        mRegComState = RegComStatus::Write;
      } else {
        mRegComState = RegComStatus::Idle;
        RegCom().var_STATUS = scmRegComReqFail;
        sendAdapterEvent(RegCom(), FORTE_WagoRegCom::scmEventErrorID, getEventChainExecutor());
      }
    }
  }

  bool WagoRegComDevice::checkRegNr() {
    bool retVal = true;
    if (func_GT(RegCom().var_cmd.var_RegNr, 63_USINT)) { // USINT = 8Bit but regNr only 6Bit
      RegCom().var_STATUS = scmRegNrOutOfRange;
      sendAdapterEvent(RegCom(), FORTE_WagoRegCom::scmEventErrorID, getEventChainExecutor());
      retVal = false;
    }
    return retVal;
  }

  void WagoRegComDevice::readRegCom() {
    if (checkRegNr() && mRegComState == RegComStatus::Idle) {
      if(getController().readRegComRequest(RegCom().var_cmd)) {
        mRegComState = RegComStatus::ReadReq;
      } else {
        mRegComState = RegComStatus::Idle;
        RegCom().var_STATUS = scmRegComReqFail;
        sendAdapterEvent(RegCom(), FORTE_WagoRegCom::scmEventErrorID, getEventChainExecutor());
      }
    }
  }

  void WagoRegComDevice::closeRegCom() {
    if (mRegComState == RegComStatus::Idle) {
      if(!RegCom().var_autoPsw) {
        disableRegCom();
        return;
      }
      if(getController().writeRegComRequest(mResetPsw)) {
        mRegComState = RegComStatus::ResetPassword;
      } else {
        RegCom().var_STATUS = scmRegComReqFail;
        sendAdapterEvent(RegCom(), FORTE_WagoRegCom::scmEventErrorID, getEventChainExecutor());
      }
    }
  }

  void WagoRegComDevice::openRegCom(CEventChainExecutionThread* const paECET) {
    if (mRegComState == RegComStatus::Init) {
      setEventChainExecutor(paECET);
      if (!getController().enableRegCom(this)) {
        mRegComState = RegComStatus::Init;
        RegCom().var_STATUS = scmRegComOpenFail;
        sendAdapterEvent(RegCom(), FORTE_WagoRegCom::scmEventErrorID, paECET);
      }
    }
  }

  void WagoRegComDevice::disableRegCom() {
    if (getController().disableRegCom()){
      mRegComState = RegComStatus::Deinit;
    }else{
      mRegComState = RegComStatus::Idle;
      RegCom().var_STATUS = scmRegComReqFail;
      sendAdapterEvent(RegCom(), FORTE_WagoRegCom::scmEventErrorID, getEventChainExecutor());
    }
  }

  void WagoRegComDevice::enabled(CEventChainExecutionThread* paECET) {
      mRegComState = RegComStatus::Idle;
      RegCom().var_STATUS = scmEnabled;
      sendAdapterEvent(RegCom(), FORTE_WagoRegCom::scmEventOpenedID, paECET);
  }

  void WagoRegComDevice::handleExternalEvent() {
    CEventChainExecutionThread* paECET = getEventChainExecutor();
    switch(mRegComState) {
    case RegComStatus::Read:
    case RegComStatus::Write:
      RegCom().var_counter = func_ADD(RegCom().var_counter, CIEC_USINT(1));
      mRegComState = RegComStatus::Idle;
      RegCom().var_STATUS = scmOK;
      sendAdapterEvent(RegCom(), FORTE_WagoRegCom::scmEventCNFID, paECET);
      break;
    case RegComStatus::ReadReq:
      if (getController().readRegComResult(RegCom().var_REG_D0, RegCom().var_REG_D1)) {
        mRegComState = RegComStatus::Read;
      } else {
        mRegComState = RegComStatus::Idle;
        RegCom().var_STATUS = scmRegComReqFail;
        sendAdapterEvent(RegCom(), FORTE_WagoRegCom::scmEventErrorID, paECET);
      }
      break;
    case RegComStatus::Init:
      if(!RegCom().var_autoPsw) {
        enabled(paECET);
        break;
      }
      if (getController().writeRegComRequest(mSetPsw)) {
        mRegComState = RegComStatus::SetPassword;
      } else {
        RegCom().var_STATUS = scmRegComReqFail;
        sendAdapterEvent(RegCom(), FORTE_WagoRegCom::scmEventErrorID, paECET);
      }
      break;
    case RegComStatus::SetPassword:
      enabled(paECET);
      break;
    case RegComStatus::ResetPassword:
      disableRegCom();
      break;
    case RegComStatus::Deinit:
      mRegComState = RegComStatus::Init;
      RegCom().var_counter = CIEC_USINT(0);
      RegCom().var_STATUS = scmDisabled;
      sendAdapterEvent(RegCom(), FORTE_WagoRegCom::scmEventClosedID, paECET);
      break;
    }
  }

  void WagoRegComDevice::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    IOConfigFBMultiSlave::executeEvent(paEIID, paECET);
    if (cgExternalEventID == paEIID) {
      handleExternalEvent();
    } else if (RegCom().evt_Write() == paEIID) {
      writeRegCom();
    } else if (RegCom().evt_Read() == paEIID){
      readRegCom();
    } else if (RegCom().evt_Open() == paEIID){
      openRegCom(paECET);
    } else if (RegCom().evt_Close() == paEIID){
      closeRegCom();
    } else if (BusAdapterIn().INIT() == paEIID) {
      getController().initRegComOffsets(this);
    }
  }

} // namespace forte::eclipse4diac::io::wago
