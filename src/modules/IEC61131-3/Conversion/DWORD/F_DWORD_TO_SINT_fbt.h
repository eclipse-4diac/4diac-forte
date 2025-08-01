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
#include "core/datatypes/forte_dword.h"
#include "core/datatypes/forte_sint.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

class FORTE_F_DWORD_TO_SINT final : public CSimpleFB {
  DECLARE_FIRMWARE_FB(FORTE_F_DWORD_TO_SINT)

  private:
    static const TEventID scmEventREQID = 0;
    static const TEventID scmEventCNFID = 0;


    CIEC_ANY *getVarInternal(size_t) override;

    void alg_REQ(void);

    void enterStateREQ(CEventChainExecutionThread *const paECET);

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_F_DWORD_TO_SINT(forte::core::StringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_DWORD var_IN;

    CIEC_SINT var_OUT;

    CEventConnection conn_CNF;

    CDataConnection *conn_IN;

    COutDataConnection<CIEC_SINT> conn_OUT;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_REQ(const CIEC_DWORD &paIN, CIEC_SINT &paOUT) {
      var_IN = paIN;
      executeEvent(scmEventREQID, nullptr);
      paOUT = var_OUT;
    }

    void operator()(const CIEC_DWORD &paIN, CIEC_SINT &paOUT) {
      evt_REQ(paIN, paOUT);
    }
};

