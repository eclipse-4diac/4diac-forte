/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_SPLIT3
 *** Description: Split an event
 *** Version:
 ***     1.0: 2017/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 ***     1.0: 2025-02-12/Franz Höpfinger - HR Agrartechnik GmbH - copy from E_SPLIT and make a E_SPLIT3 in the same way
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *************************************************************************/

#pragma once

#include "basicfb.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

class FORTE_E_SPLIT3 final : public CBasicFB {
  DECLARE_FIRMWARE_FB(FORTE_E_SPLIT3)

  private:
    static const TEventID scmEventEIID = 0;
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventEO1ID = 0;
    static const TEventID scmEventEO2ID = 1;
    static const TEventID scmEventEO3ID = 2;
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    CIEC_ANY *getVarInternal(size_t) override;

    static const TForteInt16 scmStateSTART = 0;
    static const TForteInt16 scmStateState = 1;

    void enterStateSTART(CEventChainExecutionThread *const paECET);
    void enterStateState(CEventChainExecutionThread *const paECET);

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;

  public:
    FORTE_E_SPLIT3(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CEventConnection conn_EO1;
    CEventConnection conn_EO2;
    CEventConnection conn_EO3;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_EI() {
      executeEvent(scmEventEIID, nullptr);
    }

    void operator()() {
      evt_EI();
    }
};

