/*************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_TOF
 *** Description: standard timer function block (off-delay timing)
 *** Version:
 ***     1.0: 2024-03-04/Franz Hoepfinger - HR Agrartechnik GmbH -
 ***     1.1: 2024-04-23/Franz Hoepfinger - HR Agrartechnik GmbH - Add a Reset to Timer FBs
 *************************************************************************/

#pragma once

#include "cfb.h"
#include "typelib.h"
#include "forte_bool.h"
#include "forte_time.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "E_SWITCH_fbt.h"
#include "E_DELAY_fbt.h"
#include "E_RS_fbt.h"

class FORTE_E_TOF final : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_E_TOF)

  private:
    static const TEventID scmEventREQID = 0;
    static const TEventID scmEventRID = 1;
    static const TEventID scmEventCNFID = 0;


    static const SCFB_FBInstanceData scmInternalFBs[];
    static const SCFB_FBParameter scmParamters[];
    static const SCFB_FBConnectionData scmEventConnections[];
    static const SCFB_FBConnectionData scmDataConnections[];
    static const SCFB_FBNData scmFBNData;

    forte::core::CInternalFB<FORTE_E_SWITCH> fb_E_SWITCH;
    forte::core::CInternalFB<FORTE_E_DELAY> fb_E_DELAY;
    forte::core::CInternalFB<FORTE_E_RS> fb_E_RS;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;
    CDataConnection *getIf2InConUnchecked(TPortId paDIID) override;

  public:
    FORTE_E_TOF(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CEventConnection conn_CNF;

    CDataConnection *conn_IN;
    CDataConnection *conn_PT;

    COutDataConnection<CIEC_BOOL> conn_Q;

    COutDataConnection<CIEC_BOOL> conn_if2in_IN;
    COutDataConnection<CIEC_TIME> conn_if2in_PT;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;
};
