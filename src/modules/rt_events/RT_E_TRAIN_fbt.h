/*******************************************************************************
 * Copyright (c) 2006 - 2014 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

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

#include "RT_E_CYCLE_fbt.h"
#include "E_CTU_fbt.h"
#include "E_SWITCH_fbt.h"

class FORTE_RT_E_TRAIN final : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_RT_E_TRAIN)

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
    static const CStringDictionary::TStringId scmEventInputTypeIds[];
    static const TEventID scmEventEOID = 0;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];
    static const CStringDictionary::TStringId scmEventOutputTypeIds[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    static const SCFB_FBInstanceData scmInternalFBs[];
    static const SCFB_FBParameter scmParamters[];
    static const SCFB_FBConnectionData scmEventConnections[];
    static const SCFB_FBConnectionData scmDataConnections[];
    static const SCFB_FBNData scmFBNData;

    forte::core::CInternalFB<FORTE_RT_E_CYCLE> fb_RT_E_CYCLE;
    forte::core::CInternalFB<FORTE_E_CTU> fb_E_CTU;
    forte::core::CInternalFB<FORTE_E_SWITCH> fb_E_SWITCH;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;
    CDataConnection *getIf2InConUnchecked(TPortId paDIID) override;

  public:
    FORTE_RT_E_TRAIN(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CEventConnection conn_EO;
    CDataConnection *conn_DT;
    CDataConnection *conn_N;
    CDataConnection *conn_Deadline;
    CDataConnection *conn_WCET;
    COutDataConnection<CIEC_UINT> conn_CV;
    COutDataConnection<CIEC_TIME> conn_if2in_DT;
    COutDataConnection<CIEC_UINT> conn_if2in_N;
    COutDataConnection<CIEC_TIME> conn_if2in_Deadline;
    COutDataConnection<CIEC_TIME> conn_if2in_WCET;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;
};
