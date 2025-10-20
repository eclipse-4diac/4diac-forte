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
 *** Name: PLCnextAXLSESC
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2022-04-07/Peirlberger Juergen -  - initial API and implementation and/or initial documentation
 *************************************************************************/

#pragma once

#include "../slaveHandler.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_wstring.h"
#include "PLCnextBusAdapter.h"

class FORTE_PLCnextAXLSESC final : public PLCnextSlaveHandler {
    DECLARE_FIRMWARE_FB(FORTE_PLCnextAXLSESC)

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
    FORTE_PLCnextAXLSESC(forte::StringId paInstanceNameId, CFBContainer &paContainer);

    CIEC_BOOL var_QI;

    CIEC_BOOL var_QO;
    CIEC_WSTRING var_STATUS;

    CEventConnection conn_INITO;
    CEventConnection conn_IND;

    CDataConnection *conn_QI;

    COutDataConnection<CIEC_BOOL> conn_QO;
    COutDataConnection<CIEC_WSTRING> conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;

    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void
    evt_INIT(const CIEC_BOOL &paQI, CAnyBitOutputParameter<CIEC_BOOL> paQO, COutputParameter<CIEC_WSTRING> paSTATUS) {
      COutputGuard guard_paQO(paQO);
      COutputGuard guard_paSTATUS(paSTATUS);
      var_QI = paQI;
      receiveInputEvent(scmEventINITID, nullptr);
      *paQO = var_QO;
      *paSTATUS = var_STATUS;
    }

    void
    operator()(const CIEC_BOOL &paQI, CAnyBitOutputParameter<CIEC_BOOL> paQO, COutputParameter<CIEC_WSTRING> paSTATUS) {
      evt_INIT(paQI, paQO, paSTATUS);
    }
};
