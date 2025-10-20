/*************************************************************************
 *** Copyright (c) 2018 TU Vienna/ACIN
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: SET_LOCAL_ADS_ADDRESS
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2018-08-31/Martin Melik Merkumians - TU Vienna/ACIN - initial contribution
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

class FORTE_SET_LOCAL_ADS_ADDRESS final : public CFunctionBlock {
    DECLARE_FIRMWARE_FB(FORTE_SET_LOCAL_ADS_ADDRESS)

  private:
    CIEC_BOOL &QI() {
      return *static_cast<CIEC_BOOL *>(getDI(0));
    };

    CIEC_STRING &PARAMS() {
      return *static_cast<CIEC_STRING *>(getDI(1));
    };

    CIEC_BOOL &QO() {
      return *static_cast<CIEC_BOOL *>(getDO(0));
    };

    CIEC_WSTRING &STATUS() {
      return *static_cast<CIEC_WSTRING *>(getDO(1));
    };

    CIEC_STRING &LOCAL_ADS_ADDRESS() {
      return *static_cast<CIEC_STRING *>(getDO(2));
    };

    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventINITOID = 0;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_SET_LOCAL_ADS_ADDRESS(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
        ~FORTE_SET_LOCAL_ADS_ADDRESS() override = default;

    FORTE_SET_LOCAL_ADS_ADDRESS(forte::StringId paInstanceNameId, CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_STRING var_PARAMS;

    CIEC_BOOL var_QO;
    CIEC_WSTRING var_STATUS;
    CIEC_STRING var_LOCAL_ADS_ADDRESS;

    CEventConnection conn_INITO;

    CDataConnection *conn_QI;
    CDataConnection *conn_PARAMS;

    COutDataConnection<CIEC_BOOL> conn_QO;
    COutDataConnection<CIEC_WSTRING> conn_STATUS;
    COutDataConnection<CIEC_STRING> conn_LOCAL_ADS_ADDRESS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI,
                  const CIEC_STRING &paPARAMS,
                  CAnyBitOutputParameter<CIEC_BOOL> paQO,
                  COutputParameter<CIEC_WSTRING> paSTATUS,
                  COutputParameter<CIEC_STRING> paLOCAL_ADS_ADDRESS) {
      COutputGuard guard_paQO(paQO);
      COutputGuard guard_paSTATUS(paSTATUS);
      COutputGuard guard_paLOCAL_ADS_ADDRESS(paLOCAL_ADS_ADDRESS);
      var_QI = paQI;
      var_PARAMS = paPARAMS;
      executeEvent(scmEventINITID, nullptr);
      *paQO = var_QO;
      *paSTATUS = var_STATUS;
      *paLOCAL_ADS_ADDRESS = var_LOCAL_ADS_ADDRESS;
    }

    void operator()(const CIEC_BOOL &paQI,
                    const CIEC_STRING &paPARAMS,
                    CAnyBitOutputParameter<CIEC_BOOL> paQO,
                    COutputParameter<CIEC_WSTRING> paSTATUS,
                    COutputParameter<CIEC_STRING> paLOCAL_ADS_ADDRESS) {
      evt_INIT(paQI, paPARAMS, paQO, paSTATUS, paLOCAL_ADS_ADDRESS);
    }
};
