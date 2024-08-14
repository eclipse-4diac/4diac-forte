/*******************************************************************************
* Copyright (c) 2012 - 2024 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *   Michael Gafert - moved common functions to super class, changed to new FB style
 *******************************************************************************/

#pragma once

#include "PowerlinkFunctionBlockDI.h"
#include "funcbloc.h"
#include "forte_bool.h"
#include "forte_usint.h"
#include "forte_uint.h"
#include "forte_string.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_X20DI4653 final : public PowerlinkFunctionBlockDI {
  DECLARE_FIRMWARE_FB(FORTE_X20DI4653)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventREQID = 1;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventCNFID = 1;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_X20DI4653(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_USINT var_CNID;
    CIEC_UINT var_MODID;

    CIEC_BOOL var_QO;
    CIEC_USINT var_CNIDO;
    CIEC_STRING var_STATUS;
    CIEC_BOOL var_DI01;
    CIEC_BOOL var_DI02;
    CIEC_BOOL var_DI03;
    CIEC_BOOL var_DI04;

    CIEC_BOOL var_conn_QO;
    CIEC_USINT var_conn_CNIDO;
    CIEC_STRING var_conn_STATUS;
    CIEC_BOOL var_conn_DI01;
    CIEC_BOOL var_conn_DI02;
    CIEC_BOOL var_conn_DI03;
    CIEC_BOOL var_conn_DI04;

    CEventConnection conn_INITO;
    CEventConnection conn_CNF;

    CDataConnection *conn_QI;
    CDataConnection *conn_CNID;
    CDataConnection *conn_MODID;

    CDataConnection conn_QO;
    CDataConnection conn_CNIDO;
    CDataConnection conn_STATUS;
    CDataConnection conn_DI01;
    CDataConnection conn_DI02;
    CDataConnection conn_DI03;
    CDataConnection conn_DI04;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI, const CIEC_USINT &paCNID, const CIEC_UINT &paMODID, CIEC_BOOL &paQO, CIEC_USINT &paCNIDO, CIEC_STRING &paSTATUS, CIEC_BOOL &paDI01, CIEC_BOOL &paDI02, CIEC_BOOL &paDI03, CIEC_BOOL &paDI04) {
      var_QI = paQI;
      var_CNID = paCNID;
      var_MODID = paMODID;
      executeEvent(scmEventINITID, nullptr);
      paQO = var_QO;
      paCNIDO = var_CNIDO;
      paSTATUS = var_STATUS;
      paDI01 = var_DI01;
      paDI02 = var_DI02;
      paDI03 = var_DI03;
      paDI04 = var_DI04;
    }

    void evt_REQ(const CIEC_BOOL &paQI, const CIEC_USINT &paCNID, const CIEC_UINT &paMODID, CIEC_BOOL &paQO, CIEC_USINT &paCNIDO, CIEC_STRING &paSTATUS, CIEC_BOOL &paDI01, CIEC_BOOL &paDI02, CIEC_BOOL &paDI03, CIEC_BOOL &paDI04) {
      var_QI = paQI;
      var_CNID = paCNID;
      var_MODID = paMODID;
      executeEvent(scmEventREQID, nullptr);
      paQO = var_QO;
      paCNIDO = var_CNIDO;
      paSTATUS = var_STATUS;
      paDI01 = var_DI01;
      paDI02 = var_DI02;
      paDI03 = var_DI03;
      paDI04 = var_DI04;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_USINT &paCNID, const CIEC_UINT &paMODID, CIEC_BOOL &paQO, CIEC_USINT &paCNIDO, CIEC_STRING &paSTATUS, CIEC_BOOL &paDI01, CIEC_BOOL &paDI02, CIEC_BOOL &paDI03, CIEC_BOOL &paDI04) {
      evt_INIT(paQI, paCNID, paMODID, paQO, paCNIDO, paSTATUS, paDI01, paDI02, paDI03, paDI04);
    }
};


