/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_TABLE
 *** Description: Composite Function Block Type
 *** Version:
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 *************************************************************************/

#pragma once

#include "cfb.h"
#include "typelib.h"
#include "forte_time.h"
#include "forte_uint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "E_DELAY_fbt.h"
#include "E_TABLE_CTRL_fbt.h"

class FORTE_E_TABLE final : public CCompositeFB {
  DECLARE_FIRMWARE_FB(FORTE_E_TABLE)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventSTARTID = 0;
    static const TEventID scmEventSTOPID = 1;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventEOID = 0;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    static const SCFB_FBInstanceData scmInternalFBs[];
    static const SCFB_FBConnectionData scmEventConnections[];
    static const SCFB_FBFannedOutConnectionData scmFannedOutEventConnections[];
    static const SCFB_FBConnectionData scmDataConnections[];
    static const SCFB_FBFannedOutConnectionData scmFannedOutDataConnections[];
    static const SCFB_FBNData scmFBNData;

    forte::core::CInternalFB<FORTE_E_TABLE_CTRL> fb_E_TABLE_CTRL;
    forte::core::CInternalFB<FORTE_E_DELAY> fb_E_DELAY;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void readInternal2InterfaceOutputData(TEventID paEOID) override;
    void setInitialValues() override;

  public:
    FORTE_E_TABLE(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3> var_DT;
    CIEC_UINT var_N;

    CIEC_UINT var_CV;

    CIEC_UINT var_conn_CV;

    CEventConnection conn_EO;

    CDataConnection *conn_DT;
    CDataConnection *conn_N;

    CDataConnection conn_CV;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_START(const CIEC_ARRAY_COMMON<CIEC_TIME> &paDT, const CIEC_UINT &paN, CIEC_UINT &paCV) {
      var_DT = paDT;
      var_N = paN;
      executeEvent(scmEventSTARTID, nullptr);
      paCV = var_CV;
    }

    void evt_STOP(const CIEC_ARRAY_COMMON<CIEC_TIME> &paDT, const CIEC_UINT &paN, CIEC_UINT &paCV) {
      var_DT = paDT;
      var_N = paN;
      executeEvent(scmEventSTOPID, nullptr);
      paCV = var_CV;
    }

    void operator()(const CIEC_ARRAY_COMMON<CIEC_TIME> &paDT, const CIEC_UINT &paN, CIEC_UINT &paCV) {
      evt_START(paDT, paN, paCV);
    }
};

