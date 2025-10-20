/*************************************************************************
 * Copyright (c) 2025, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#pragma once

#include "WagoSlaveBase.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"

class FORTE_Wago636 final : public WagoSlaveBase {
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
    INIT_HANDLES(7, 7, 1, 1)

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
    CDataConnection *conn_PresetInputEnable;
    CDataConnection *conn_QuitErrors;

    COutDataConnection<CIEC_BOOL> conn_QO;
    COutDataConnection<CIEC_WSTRING> conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;

    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_MAP(const CIEC_BOOL &paQI,
                 const CIEC_STRING &paBusy,
                 const CIEC_STRING &paLimitSwitchN,
                 const CIEC_STRING &paLimitSwitchP,
                 const CIEC_STRING &paPresetInput,
                 const CIEC_STRING &paOptimizeOnZInput,
                 const CIEC_STRING &paOnTarget,
                 const CIEC_STRING &paReferenceOk,
                 const CIEC_STRING &paCurrentPosition,
                 const CIEC_STRING &paTargetPosition,
                 const CIEC_STRING &paMotorN,
                 const CIEC_STRING &paMotorP,
                 const CIEC_STRING &paPositioning,
                 const CIEC_STRING &paOptimizeOn,
                 const CIEC_STRING &paPreset,
                 const CIEC_STRING &paPresetInputEnable,
                 const CIEC_STRING &paQuitErrors,
                 CAnyBitOutputParameter<CIEC_BOOL> paQO,
                 COutputParameter<CIEC_WSTRING> paSTATUS) {
      COutputGuard guard_paQO(paQO);
      COutputGuard guard_paSTATUS(paSTATUS);
      var_QI = paQI;
      var_Busy = paBusy;
      var_LimitSwitchN = paLimitSwitchN;
      var_LimitSwitchP = paLimitSwitchP;
      var_PresetInput = paPresetInput;
      var_OptimizeOnZInput = paOptimizeOnZInput;
      var_OnTarget = paOnTarget;
      var_ReferenceOk = paReferenceOk;
      var_CurrentPosition = paCurrentPosition;
      var_TargetPosition = paTargetPosition;
      var_MotorN = paMotorN;
      var_MotorP = paMotorP;
      var_Positioning = paPositioning;
      var_OptimizeOn = paOptimizeOn;
      var_Preset = paPreset;
      var_PresetInputEnable = paPresetInputEnable;
      var_QuitErrors = paQuitErrors;
      executeEvent(scmEventMAPID, nullptr);
      *paQO = var_QO;
      *paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI,
                    const CIEC_STRING &paBusy,
                    const CIEC_STRING &paLimitSwitchN,
                    const CIEC_STRING &paLimitSwitchP,
                    const CIEC_STRING &paPresetInput,
                    const CIEC_STRING &paOptimizeOnZInput,
                    const CIEC_STRING &paOnTarget,
                    const CIEC_STRING &paReferenceOk,
                    const CIEC_STRING &paCurrentPosition,
                    const CIEC_STRING &paTargetPosition,
                    const CIEC_STRING &paMotorN,
                    const CIEC_STRING &paMotorP,
                    const CIEC_STRING &paPositioning,
                    const CIEC_STRING &paOptimizeOn,
                    const CIEC_STRING &paPreset,
                    const CIEC_STRING &paPresetInputEnable,
                    const CIEC_STRING &paQuitErrors,
                    CAnyBitOutputParameter<CIEC_BOOL> paQO,
                    COutputParameter<CIEC_WSTRING> paSTATUS) {
      evt_MAP(paQI, paBusy, paLimitSwitchN, paLimitSwitchP, paPresetInput, paOptimizeOnZInput, paOnTarget,
              paReferenceOk, paCurrentPosition, paTargetPosition, paMotorN, paMotorP, paPositioning, paOptimizeOn,
              paPreset, paPresetInputEnable, paQuitErrors, paQO, paSTATUS);
    }
};
