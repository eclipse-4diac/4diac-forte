/*******************************************************************************
 * Copyright (c) 2025 Maximilian Scharf
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Maximilian Scharf - Initial implementation.
 *******************************************************************************/

#pragma once

#include "core/funcbloc.h"
#include "core/datatypes/forte_bool.h"
#include "core/datatypes/forte_wstring.h"
#include "eGenAdapter_adp.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

#include "eIX_fbt.h"
#include "eConfigFB.h"

class FORTE_eIXconfig final : public CeConfigFB {
  DECLARE_FIRMWARE_FB(FORTE_eIXconfig)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventCONFID = 0;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventCNFID = 0;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];
    static const int scmeIXAdpNum = 0;
    static const SAdapterInstanceDef scmAdapterInstances[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_eIXconfig(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);
    bool initialize() override;

    CIEC_BOOL var_FE;
    CIEC_BOOL var_RE;

    CIEC_WSTRING var_STATUS;

    FORTE_eGenAdapter var_eIX;

    CEventConnection conn_CNF;

    CDataConnection *conn_FE;
    CDataConnection *conn_RE;

    COutDataConnection<CIEC_WSTRING> conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CAdapter *getAdapterUnchecked(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_CONF(const CIEC_BOOL &paFE, const CIEC_BOOL &paRE, CIEC_WSTRING &paSTATUS) {
      var_FE = paFE;
      var_RE = paRE;
      executeEvent(scmEventCONFID, nullptr);
      paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paFE, const CIEC_BOOL &paRE, CIEC_WSTRING &paSTATUS) {
      evt_CONF(paFE, paRE, paSTATUS);
    }

    bool eventGen() override;
};

