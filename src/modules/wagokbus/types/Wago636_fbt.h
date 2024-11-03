/*************************************************************************
 * Copyright (c) 2024
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#pragma once

#include "funcbloc.h"
#include "forte_bool.h"
#include "forte_string.h"
#include "forte_uint.h"
#include "forte_wstring.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "WagoBusAdapter.h"
#include "wagoSlaveBase.h"

class FORTE_Wago636 final : public WagoSlaveBase {
  DECLARE_FIRMWARE_FB(FORTE_Wago636)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventMAPID = 0;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventMAPOID = 0;
    static const TEventID scmEventINDID = 1;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];
    static const int scmBusAdapterInAdpNum = 0;
    static const int scmBusAdapterOutAdpNum = 1;
    static const SAdapterInstanceDef scmAdapterInstances[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  protected:
      void initHandlesBase(size_t paNumberOfBoolInputs, size_t paNumberOfBoolOutputs, size_t paNumberOfAnalogInputs, size_t paNumberOfAnalogOutputs);
      INIT_HANDLES(3, 5, 1, 1)

  public:
    FORTE_Wago636(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);
    ~FORTE_Wago636() override = default;

    CIEC_BOOL var_QI;
    CIEC_STRING var_Busy;
    CIEC_STRING var_LimitSwitchN;
    CIEC_STRING var_LimitSwitchP;
    CIEC_STRING var_CurrentPosition;
    CIEC_STRING var_TargetPosition;
    CIEC_STRING var_MotorN;
    CIEC_STRING var_MotorP;
    CIEC_STRING var_Positioning;
    CIEC_STRING var_Preset;
    CIEC_STRING var_QuitErrors;

    CIEC_BOOL var_QO;
    CIEC_WSTRING var_STATUS;

    CIEC_BOOL var_conn_QO;
    CIEC_WSTRING var_conn_STATUS;

    CEventConnection conn_MAPO;
    CEventConnection conn_IND;

    CDataConnection *conn_QI;
    CDataConnection *conn_Busy;
    CDataConnection *conn_LimitSwitchN;
    CDataConnection *conn_LimitSwitchP;
    CDataConnection *conn_CurrentPosition;
    CDataConnection *conn_TargetPosition;
    CDataConnection *conn_MotorN;
    CDataConnection *conn_MotorP;
    CDataConnection *conn_Positioning;
    CDataConnection *conn_Preset;
    CDataConnection *conn_QuitErrors;

    CDataConnection conn_QO;
    CDataConnection conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    FORTE_WagoBusAdapter &var_BusAdapterIn() {
      return *static_cast<FORTE_WagoBusAdapter*>(mAdapters[0]);
    };

    FORTE_WagoBusAdapter &var_BusAdapterOut() {
      return *static_cast<FORTE_WagoBusAdapter*>(mAdapters[1]);
    };
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_MAP(const CIEC_BOOL &paQI, const CIEC_STRING &paBusy, const CIEC_STRING &paLimitSwitchN, const CIEC_STRING &paLimitSwitchP, const CIEC_STRING &paCurrentPosition, const CIEC_STRING &paTargetPosition, const CIEC_STRING &paMotorN, const CIEC_STRING &paMotorP, const CIEC_STRING &paPositioning, const CIEC_STRING &paPreset, const CIEC_STRING &paQuitErrors, CIEC_BOOL &paQO, CIEC_WSTRING &paSTATUS) {
      var_QI = paQI;
      var_Busy = paBusy;
      var_LimitSwitchN = paLimitSwitchN;
      var_LimitSwitchP = paLimitSwitchP;
      var_CurrentPosition = paCurrentPosition;
      var_TargetPosition = paTargetPosition;
      var_MotorN = paMotorN;
      var_MotorP = paMotorP;
      var_Positioning = paPositioning;
      var_Preset = paPreset;
      var_QuitErrors = paQuitErrors;
      executeEvent(scmEventMAPID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_STRING &paBusy, const CIEC_STRING &paLimitSwitchN, const CIEC_STRING &paLimitSwitchP, const CIEC_STRING &paCurrentPosition, const CIEC_STRING &paTargetPosition, const CIEC_STRING &paMotorN, const CIEC_STRING &paMotorP, const CIEC_STRING &paPositioning, const CIEC_STRING &paPreset, const CIEC_STRING &paQuitErrors, CIEC_BOOL &paQO, CIEC_WSTRING &paSTATUS) {
      evt_MAP(paQI, paBusy, paLimitSwitchN, paLimitSwitchP, paCurrentPosition, paTargetPosition, paMotorN, paMotorP, paPositioning, paPreset, paQuitErrors, paQO, paSTATUS);
    }
};

