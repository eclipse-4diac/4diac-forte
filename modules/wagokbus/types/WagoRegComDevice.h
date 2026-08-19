/*************************************************************************
 * Copyright (c) 2026 Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 *************************************************************************/

#pragma once

#include "WagoSlaveBase.h"
#include "WagoRegCom_adp.h"
#include "RegComCmd_dtp.h"
#include "../WagoDeviceController.h"

namespace forte::eclipse4diac::io::wago {

  class WagoRegComDevice : public WagoSlaveBase {
    public:
      WagoRegComDevice(int paType, CFBContainer &paContainer, const SFBInterfaceSpec &paInterfaceSpec, const forte::StringId paInstanceNameId);
      virtual ~WagoRegComDevice() = default;

      forte::CSocketPin<FORTE_WagoRegCom_Socket> var_RegCom;

      size_t getSlaveIndex() {
        return mIndex;
      }

      void setOffset_REG_S0(TForteUInt32 paOffset) { mOffset_REG_S0 = paOffset; }
      void setOffset_REG_S7(TForteUInt32 paOffset) { mOffset_REG_S7 = paOffset; }
      void setOffset_REG_C0(TForteUInt32 paOffset) { mOffset_REG_C0 = paOffset; }
      void setOffset_REG_C7(TForteUInt32 paOffset) { mOffset_REG_C7 = paOffset; }
      void setOffset_wREG_D0(TForteUInt32 paOffset) { mOffset_wREG_D0 = paOffset; }
      void setOffset_rREG_D0(TForteUInt32 paOffset) { mOffset_rREG_D0 = paOffset; }

      TForteUInt32 getOffset_REG_S0() { return mOffset_REG_S0; }
      TForteUInt32 getOffset_REG_S7() { return mOffset_REG_S7; }
      TForteUInt32 getOffset_REG_C0() { return mOffset_REG_C0; }
      TForteUInt32 getOffset_REG_C7() { return mOffset_REG_C7; }
      TForteUInt32 getOffset_wREG_D0() { return mOffset_wREG_D0; }
      TForteUInt32 getOffset_rREG_D0() { return mOffset_rREG_D0; }

      void handleExternalEvent();

    protected:
      void writeRegCom();
      void readRegCom();
      void closeRegCom();
      void openRegCom(CEventChainExecutionThread* const paECET);

      forte::ISocketPin *getSocketPinUnchecked(size_t) override;

      WagoDeviceController &getController() {
        return static_cast<WagoDeviceController &>(IOConfigFBMultiSlave::getController());
      }

      FORTE_WagoRegCom &RegCom() {
        return (*static_cast<FORTE_WagoRegCom *>(getSocketPinUnchecked(1)->getAdapterBlock()));
      }

    private:
      enum class RegComStatus : std::uint8_t {
          Init = 0,
          SetPassword = 1,
          Idle = 2,
          Write = 3,
          Read = 4,
          ReadReq = 5,
          ResetPassword = 6,
          Deinit = 7
      };

      RegComStatus regComState = RegComStatus::Init;

      TForteUInt32 mOffset_REG_S0;
      TForteUInt32 mOffset_REG_S7;
      TForteUInt32 mOffset_REG_C0;
      TForteUInt32 mOffset_REG_C7;
      TForteUInt32 mOffset_wREG_D0;
      TForteUInt32 mOffset_rREG_D0;

      CIEC_RegComCmd const mSetPsw{CIEC_USINT(31), CIEC_BYTE(0x35), CIEC_BYTE(0x12)};
      CIEC_RegComCmd const mResetPsw{CIEC_USINT(31), CIEC_BYTE(0x00), CIEC_BYTE(0x00)};
  };

} // namespace forte::eclipse4diac::io::wago
