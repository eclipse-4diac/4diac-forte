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
 *   Michael Gafert - converted to new FB style
 *******************************************************************************/

#pragma once

#include "EplWrapper.h"

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_string.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

class FORTE_POWERLINK_MN final : public CFunctionBlock {
    DECLARE_FIRMWARE_FB(FORTE_POWERLINK_MN)

  private:
    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventINITOID = 0;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_POWERLINK_MN(forte::StringId paInstanceNameId, forte::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_STRING var_CDC_CFG;
    CIEC_STRING var_APP_CFG;
    CIEC_STRING var_DEV_NAME;

    CIEC_BOOL var_QO;
    CIEC_STRING var_STATUS;

    CEventConnection conn_INITO;

    CDataConnection *conn_QI;
    CDataConnection *conn_CDC_CFG;
    CDataConnection *conn_APP_CFG;
    CDataConnection *conn_DEV_NAME;

    COutDataConnection<CIEC_BOOL> conn_QO;
    COutDataConnection<CIEC_STRING> conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI,
                  const CIEC_STRING &paCDC_CFG,
                  const CIEC_STRING &paAPP_CFG,
                  const CIEC_STRING &paDEV_NAME,
                  CAnyBitOutputParameter<CIEC_BOOL> paQO,
                  COutputParameter<CIEC_STRING> paSTATUS) {
      COutputGuard guard_paQO(paQO);
      COutputGuard guard_paSTATUS(paSTATUS);
      var_QI = paQI;
      var_CDC_CFG = paCDC_CFG;
      var_APP_CFG = paAPP_CFG;
      var_DEV_NAME = paDEV_NAME;
      executeEvent(scmEventINITID, nullptr);
      *paQO = var_QO;
      *paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI,
                    const CIEC_STRING &paCDC_CFG,
                    const CIEC_STRING &paAPP_CFG,
                    const CIEC_STRING &paDEV_NAME,
                    CAnyBitOutputParameter<CIEC_BOOL> paQO,
                    COutputParameter<CIEC_STRING> paSTATUS) {
      evt_INIT(paQI, paCDC_CFG, paAPP_CFG, paDEV_NAME, paQO, paSTATUS);
    }
};
