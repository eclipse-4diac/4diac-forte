/*************************************************************************
 *** Copyright (c) 2022 Peirlberger Juergen
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
 *** Name: PLCnextAXLSEDI16
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2022-04-07/Peirlberger Juergen -  - initial API and implementation and/or initial documentation
 *************************************************************************/

#pragma once

#include "../slaveHandler.h"
#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "PLCnextBusAdapter.h"

class FORTE_PLCnextAXLSEDI16 final : public PLCnextSlaveHandler {
    DECLARE_FIRMWARE_FB(FORTE_PLCnextAXLSEDI16)

  private:
    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventINDID = 1;
    static const int scmBusAdapterInAdpNum = 0;
    static const int scmBusAdapterOutAdpNum = 1;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

    const char *init() override;
    void initHandles() override;

  public:
    FORTE_PLCnextAXLSEDI16(forte::StringId paInstanceNameId, CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_STRING var_DI_1;
    CIEC_STRING var_DI_2;
    CIEC_STRING var_DI_3;
    CIEC_STRING var_DI_4;
    CIEC_STRING var_DI_5;
    CIEC_STRING var_DI_6;
    CIEC_STRING var_DI_7;
    CIEC_STRING var_DI_8;
    CIEC_STRING var_DI_9;
    CIEC_STRING var_DI_10;
    CIEC_STRING var_DI_11;
    CIEC_STRING var_DI_12;
    CIEC_STRING var_DI_13;
    CIEC_STRING var_DI_14;
    CIEC_STRING var_DI_15;
    CIEC_STRING var_DI_16;

    CIEC_BOOL var_QO;
    CIEC_WSTRING var_STATUS;

    CEventConnection conn_INITO;
    CEventConnection conn_IND;

    CDataConnection *conn_QI;
    CDataConnection *conn_DI_1;
    CDataConnection *conn_DI_2;
    CDataConnection *conn_DI_3;
    CDataConnection *conn_DI_4;
    CDataConnection *conn_DI_5;
    CDataConnection *conn_DI_6;
    CDataConnection *conn_DI_7;
    CDataConnection *conn_DI_8;
    CDataConnection *conn_DI_9;
    CDataConnection *conn_DI_10;
    CDataConnection *conn_DI_11;
    CDataConnection *conn_DI_12;
    CDataConnection *conn_DI_13;
    CDataConnection *conn_DI_14;
    CDataConnection *conn_DI_15;
    CDataConnection *conn_DI_16;

    COutDataConnection<CIEC_BOOL> conn_QO;
    COutDataConnection<CIEC_WSTRING> conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;

    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI,
                  const CIEC_STRING &paDI_1,
                  const CIEC_STRING &paDI_2,
                  const CIEC_STRING &paDI_3,
                  const CIEC_STRING &paDI_4,
                  const CIEC_STRING &paDI_5,
                  const CIEC_STRING &paDI_6,
                  const CIEC_STRING &paDI_7,
                  const CIEC_STRING &paDI_8,
                  const CIEC_STRING &paDI_9,
                  const CIEC_STRING &paDI_10,
                  const CIEC_STRING &paDI_11,
                  const CIEC_STRING &paDI_12,
                  const CIEC_STRING &paDI_13,
                  const CIEC_STRING &paDI_14,
                  const CIEC_STRING &paDI_15,
                  const CIEC_STRING &paDI_16,
                  CAnyBitOutputParameter<CIEC_BOOL> paQO,
                  COutputParameter<CIEC_WSTRING> paSTATUS) {
      COutputGuard guard_paQO(paQO);
      COutputGuard guard_paSTATUS(paSTATUS);
      var_QI = paQI;
      var_DI_1 = paDI_1;
      var_DI_2 = paDI_2;
      var_DI_3 = paDI_3;
      var_DI_4 = paDI_4;
      var_DI_5 = paDI_5;
      var_DI_6 = paDI_6;
      var_DI_7 = paDI_7;
      var_DI_8 = paDI_8;
      var_DI_9 = paDI_9;
      var_DI_10 = paDI_10;
      var_DI_11 = paDI_11;
      var_DI_12 = paDI_12;
      var_DI_13 = paDI_13;
      var_DI_14 = paDI_14;
      var_DI_15 = paDI_15;
      var_DI_16 = paDI_16;
      receiveInputEvent(scmEventINITID, nullptr);
      *paQO = var_QO;
      *paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI,
                    const CIEC_STRING &paDI_1,
                    const CIEC_STRING &paDI_2,
                    const CIEC_STRING &paDI_3,
                    const CIEC_STRING &paDI_4,
                    const CIEC_STRING &paDI_5,
                    const CIEC_STRING &paDI_6,
                    const CIEC_STRING &paDI_7,
                    const CIEC_STRING &paDI_8,
                    const CIEC_STRING &paDI_9,
                    const CIEC_STRING &paDI_10,
                    const CIEC_STRING &paDI_11,
                    const CIEC_STRING &paDI_12,
                    const CIEC_STRING &paDI_13,
                    const CIEC_STRING &paDI_14,
                    const CIEC_STRING &paDI_15,
                    const CIEC_STRING &paDI_16,
                    CAnyBitOutputParameter<CIEC_BOOL> paQO,
                    COutputParameter<CIEC_WSTRING> paSTATUS) {
      evt_INIT(paQI, paDI_1, paDI_2, paDI_3, paDI_4, paDI_5, paDI_6, paDI_7, paDI_8, paDI_9, paDI_10, paDI_11, paDI_12,
               paDI_13, paDI_14, paDI_15, paDI_16, paQO, paSTATUS);
    }
};
