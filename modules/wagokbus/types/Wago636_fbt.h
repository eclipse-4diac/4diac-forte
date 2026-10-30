/*************************************************************************
 * Copyright (c) 2025, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#pragma once

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"
#include "../WagoDeviceController.h"
#include "WagoRegComDevice.h"

namespace forte::eclipse4diac::io::wago {

  class FORTE_Wago636 final : public WagoRegComDevice {
      DECLARE_FIRMWARE_FB(FORTE_Wago636)

    private:
      static const TEventID scmEventMAPID = 0;
      static const TEventID scmEventMAPOID = 0;
      static const TEventID scmEventINDID = 1;
      static const int scmBusAdapterInAdpNum = 0;
      static const int scmBusAdapterOutAdpNum = 1;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    protected:
      void initHandlesBase(size_t paNumberOfBoolInputs,
                           size_t paNumberOfBoolOutputs,
                           size_t paNumberOfAnalogInputs,
                           size_t paNumberOfAnalogOutputs);
      INIT_HANDLES(7, 8, 1, 1)

    public:
      FORTE_Wago636(const forte::StringId paInstanceNameId, CFBContainer &paContainer);
      ~FORTE_Wago636() override = default;

      CIEC_BOOL var_QI;
      CIEC_STRING var_Busy;
      CIEC_STRING var_LimitSwitchN;
      CIEC_STRING var_LimitSwitchP;
      CIEC_STRING var_PresetInput;
      CIEC_STRING var_OptimizeOnZInput;
      CIEC_STRING var_OnTarget;
      CIEC_STRING var_ReferenceOk;
      CIEC_STRING var_CurrentPosition;
      CIEC_STRING var_TargetPosition;
      CIEC_STRING var_MotorN;
      CIEC_STRING var_MotorP;
      CIEC_STRING var_Positioning;
      CIEC_STRING var_OptimizeOn;
      CIEC_STRING var_Preset;
      CIEC_STRING var_CurrentControlOn;
      CIEC_STRING var_PresetInputEnable;
      CIEC_STRING var_QuitErrors;

      CIEC_BOOL var_QO;
      CIEC_WSTRING var_STATUS;

      CEventConnection conn_MAPO;
      CEventConnection conn_IND;

      CDataConnection *conn_QI;
      CDataConnection *conn_Busy;
      CDataConnection *conn_LimitSwitchN;
      CDataConnection *conn_LimitSwitchP;
      CDataConnection *conn_PresetInput;
      CDataConnection *conn_OptimizeOnZInput;
      CDataConnection *conn_OnTarget;
      CDataConnection *conn_ReferenceOk;
      CDataConnection *conn_CurrentPosition;
      CDataConnection *conn_TargetPosition;
      CDataConnection *conn_MotorN;
      CDataConnection *conn_MotorP;
      CDataConnection *conn_Positioning;
      CDataConnection *conn_OptimizeOn;
      CDataConnection *conn_Preset;
      CDataConnection *conn_CurrentControlOn;
      CDataConnection *conn_PresetInputEnable;
      CDataConnection *conn_QuitErrors;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_WSTRING> conn_STATUS;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;

      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };

} // namespace forte::eclipse4diac::io::wago
