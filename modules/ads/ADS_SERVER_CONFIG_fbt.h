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
 *** Name: ADS_SERVER_CONFIG
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2018-08-28/Martin Melik Merkumians - TU Vienna/ACIN - initial contribution
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

class FORTE_ADS_SERVER_CONFIG final : public CFunctionBlock {
    DECLARE_FIRMWARE_FB(FORTE_ADS_SERVER_CONFIG)

  private:
    CIEC_BOOL &QI() {
      return *static_cast<CIEC_BOOL *>(getDI(0));
    };

    CIEC_STRING &FRIENDLY_NAME() {
      return *static_cast<CIEC_STRING *>(getDI(1));
    };

    CIEC_STRING &SERVER_ADS_ADDRESS() {
      return *static_cast<CIEC_STRING *>(getDI(2));
    };

    CIEC_UINT &ADS_PORT() {
      return *static_cast<CIEC_UINT *>(getDI(3));
    };

    CIEC_STRING &SERVER_IPV4_OR_HOSTNAME() {
      return *static_cast<CIEC_STRING *>(getDI(4));
    };

    CIEC_BOOL &QO() {
      return *static_cast<CIEC_BOOL *>(getDO(0));
    };

    CIEC_WSTRING &STATUS() {
      return *static_cast<CIEC_WSTRING *>(getDO(1));
    };

    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventINITOID = 0;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    ~FORTE_ADS_SERVER_CONFIG() override = default;

    FORTE_ADS_SERVER_CONFIG(forte::StringId paInstanceNameId, CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_WSTRING var_FRIENDLY_NAME;
    CIEC_WSTRING var_SERVER_ADS_ADDRESS;
    CIEC_UINT var_ADS_PORT;
    CIEC_WSTRING var_SERVER_IPV4_OR_HOSTNAME;

    CIEC_BOOL var_QO;
    CIEC_WSTRING var_STATUS;

    CEventConnection conn_INITO;

    CDataConnection *conn_QI;
    CDataConnection *conn_FRIENDLY_NAME;
    CDataConnection *conn_SERVER_ADS_ADDRESS;
    CDataConnection *conn_ADS_PORT;
    CDataConnection *conn_SERVER_IPV4_OR_HOSTNAME;

    COutDataConnection<CIEC_BOOL> conn_QO;
    COutDataConnection<CIEC_WSTRING> conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI,
                  const CIEC_WSTRING &paFRIENDLY_NAME,
                  const CIEC_WSTRING &paSERVER_ADS_ADDRESS,
                  const CIEC_UINT &paADS_PORT,
                  const CIEC_WSTRING &paSERVER_IPV4_OR_HOSTNAME,
                  CAnyBitOutputParameter<CIEC_BOOL> paQO,
                  COutputParameter<CIEC_WSTRING> paSTATUS) {
      COutputGuard guard_paQO(paQO);
      COutputGuard guard_paSTATUS(paSTATUS);
      var_QI = paQI;
      var_FRIENDLY_NAME = paFRIENDLY_NAME;
      var_SERVER_ADS_ADDRESS = paSERVER_ADS_ADDRESS;
      var_ADS_PORT = paADS_PORT;
      var_SERVER_IPV4_OR_HOSTNAME = paSERVER_IPV4_OR_HOSTNAME;
      executeEvent(scmEventINITID, nullptr);
      *paQO = var_QO;
      *paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI,
                    const CIEC_WSTRING &paFRIENDLY_NAME,
                    const CIEC_WSTRING &paSERVER_ADS_ADDRESS,
                    const CIEC_UINT &paADS_PORT,
                    const CIEC_WSTRING &paSERVER_IPV4_OR_HOSTNAME,
                    CAnyBitOutputParameter<CIEC_BOOL> paQO,
                    COutputParameter<CIEC_WSTRING> paSTATUS) {
      evt_INIT(paQI, paFRIENDLY_NAME, paSERVER_ADS_ADDRESS, paADS_PORT, paSERVER_IPV4_OR_HOSTNAME, paQO, paSTATUS);
    }
};
