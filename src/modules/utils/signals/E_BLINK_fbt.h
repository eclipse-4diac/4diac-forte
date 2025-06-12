/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_BLINK
 *** Description: Simulates a blinking signal (turning on and off for specific durations)
 *** Version:
 ***     1.0: 2025-02-15/Franz Höpfinger - HR Agrartechnik GmbH   - initial API and implementation and/or initial
 *documentation
 *************************************************************************/

#pragma once

#include "cfb.h"
#include "typelib.h"
#include "forte_any_magnitude_variant.h"
#include "forte_bool.h"
#include "forte_time.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "E_CYCLE_fbt.h"
#include "E_PULSE_fbt.h"
#include "GEN_ADD_fbt.h"

class FORTE_E_BLINK final : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_E_BLINK)

  private:
    static const TEventID scmEventSTARTID = 0;
    static const TEventID scmEventSTOPID = 1;
    static const TEventID scmEventCNFID = 0;


    static const SCFB_FBInstanceData scmInternalFBs[];
    static const SCFB_FBConnectionData scmEventConnections[];
    static const SCFB_FBConnectionData scmDataConnections[];
    static const SCFB_FBNData scmFBNData;

    forte::core::CInternalFB<FORTE_E_PULSE> fb_E_TP;
    forte::core::CInternalFB<FORTE_E_CYCLE> fb_E_CYCLE;
    forte::core::CInternalFB<GEN_ADD> fb_ADD_2;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;
    CDataConnection *getIf2InConUnchecked(TPortId paDIID) override;

  public:
    FORTE_E_BLINK(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CEventConnection conn_CNF;

    CDataConnection *conn_TIMELOW;
    CDataConnection *conn_TIMEHIGH;

    COutDataConnection<CIEC_BOOL> conn_OUT;

    COutDataConnection<CIEC_TIME> conn_if2in_TIMELOW;
    COutDataConnection<CIEC_TIME> conn_if2in_TIMEHIGH;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;
};
