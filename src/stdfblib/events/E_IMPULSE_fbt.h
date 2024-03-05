/*************************************************************************  
 *** Copyright (c) 2012, 2023 TU Wien ACIN, HR Agrartechnik GmbH  
 *** This program and the accompanying materials are made available under the  
 *** terms of the Eclipse Public License 2.0 which is available at  
 *** http://www.eclipse.org/legal/epl-2.0.  
 ***  
 *** SPDX-License-Identifier: EPL-2.0  
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_IMPULSE
 *** Description: standard timer function block (pulse)
 *** Version:
 ***     1.0: 2023-08-21/Franz Hoepfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#pragma once

#include "cfb.h"
#include "typelib.h"
#include "forte_time.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_E_IMPULSE final : public CCompositeFB {
  DECLARE_FIRMWARE_FB(FORTE_E_IMPULSE)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventREQID = 0;
    static const TEventID scmEventRID = 1;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventCNFID = 0;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    static const SCFB_FBInstanceData scmInternalFBs[];
    static const SCFB_FBParameter scmParamters[];
    static const SCFB_FBConnectionData scmEventConnections[];
    static const SCFB_FBFannedOutConnectionData scmFannedOutEventConnections[];
    static const SCFB_FBConnectionData scmDataConnections[];
    static const SCFB_FBFannedOutConnectionData scmFannedOutDataConnections[];
    static const SCFB_FBNData scmFBNData;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void readInternal2InterfaceOutputData(TEventID paEOID) override;
    void setInitialValues() override;

  public:
    FORTE_E_IMPULSE(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_TIME var_PT;

    CIEC_BOOL var_Q;

    CIEC_BOOL var_conn_Q;

    CEventConnection conn_CNF;

    CDataConnection *conn_PT;

    CDataConnection conn_Q;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_REQ(const CIEC_TIME &paPT, CIEC_BOOL &paQ) {
      var_PT = paPT;
      executeEvent(scmEventREQID, nullptr);
      paQ = var_Q;
    }

    void evt_R(const CIEC_TIME &paPT, CIEC_BOOL &paQ) {
      var_PT = paPT;
      executeEvent(scmEventRID, nullptr);
      paQ = var_Q;
    }

    void operator()(const CIEC_TIME &paPT, CIEC_BOOL &paQ) {
      evt_REQ(paPT, paQ);
    }
};


