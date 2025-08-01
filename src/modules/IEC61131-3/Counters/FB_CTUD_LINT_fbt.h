/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "core/simplefb.h"
#include "core/datatypes/forte_bool.h"
#include "core/datatypes/forte_lint.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

class FORTE_FB_CTUD_LINT final : public CSimpleFB {
  DECLARE_FIRMWARE_FB(FORTE_FB_CTUD_LINT)

  private:
    static const TEventID scmEventCNFID = 0;
    static const TEventID scmEventREQID = 0;

    CIEC_ANY *getVarInternal(size_t) override;

    void alg_REQ(void);

    void enterStateREQ(CEventChainExecutionThread *const paECET);

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_FB_CTUD_LINT(forte::core::StringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_CU;
    CIEC_BOOL var_CD;
    CIEC_BOOL var_R;
    CIEC_BOOL var_LD;
    CIEC_LINT var_PV;

    CIEC_BOOL var_QU;
    CIEC_BOOL var_QD;
    CIEC_LINT var_CV;

    CEventConnection conn_CNF;

    CDataConnection *conn_CU;
    CDataConnection *conn_CD;
    CDataConnection *conn_R;
    CDataConnection *conn_LD;
    CDataConnection *conn_PV;

    COutDataConnection<CIEC_BOOL> conn_QU;
    COutDataConnection<CIEC_BOOL> conn_QD;
    COutDataConnection<CIEC_LINT> conn_CV;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_REQ(const CIEC_BOOL &paCU, const CIEC_BOOL &paCD, const CIEC_BOOL &paR, const CIEC_BOOL &paLD, const CIEC_LINT &paPV, CIEC_BOOL &paQU, CIEC_BOOL &paQD, CIEC_LINT &paCV) {
      var_CU = paCU;
      var_CD = paCD;
      var_R = paR;
      var_LD = paLD;
      var_PV = paPV;
      executeEvent(scmEventREQID, nullptr);
      paQU = var_QU;
      paQD = var_QD;
      paCV = var_CV;
    }

    void operator()(const CIEC_BOOL &paCU, const CIEC_BOOL &paCD, const CIEC_BOOL &paR, const CIEC_BOOL &paLD, const CIEC_LINT &paPV, CIEC_BOOL &paQU, CIEC_BOOL &paQD, CIEC_LINT &paCV) {
      evt_REQ(paCU, paCD, paR, paLD, paPV, paQU, paQD, paCV);
    }
};

