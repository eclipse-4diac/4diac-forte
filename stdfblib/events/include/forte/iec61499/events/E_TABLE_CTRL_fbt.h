/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
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
 *** Name: E_TABLE_CTRL
 *** Description: Support function block for E_TABLE
 *** Version:
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 *************************************************************************/

#pragma once

#include "forte/basicfb.h"
#include "forte/datatypes/forte_time.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61499::events {
  class FORTE_E_TABLE_CTRL final : public CBasicFB {
      DECLARE_FIRMWARE_FB(FORTE_E_TABLE_CTRL)

    private:
      static const TEventID scmEventSTARTID = 0;
      static const TEventID scmEventCLKID = 1;
      static const TEventID scmEventCLKOID = 0;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_INIT(void);
      void alg_NEXT_STEP(void);

      static const TForteInt16 scmStateSTART = 0;
      static const TForteInt16 scmStateINIT = 1;
      static const TForteInt16 scmStateINIT1 = 2;
      static const TForteInt16 scmStateNEXT_STEP = 3;

      void enterStateSTART(CEventChainExecutionThread *const paECET);
      void enterStateINIT(CEventChainExecutionThread *const paECET);
      void enterStateINIT1(CEventChainExecutionThread *const paECET);
      void enterStateNEXT_STEP(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_E_TABLE_CTRL(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3> var_DT;
      CIEC_UINT var_N;

      CIEC_TIME var_DTO;
      CIEC_UINT var_CV;

      CEventConnection conn_CLKO;

      CDataConnection *conn_DT;
      CDataConnection *conn_N;

      COutDataConnection<CIEC_TIME> conn_DTO;
      COutDataConnection<CIEC_UINT> conn_CV;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_START(const CIEC_ARRAY_COMMON<CIEC_TIME> &paDT,
                     const CIEC_UINT &paN,
                     COutputParameter<CIEC_TIME> paDTO,
                     COutputParameter<CIEC_UINT> paCV) {
        COutputGuard guard_paDTO(paDTO);
        COutputGuard guard_paCV(paCV);
        var_DT = paDT;
        var_N = paN;
        executeEvent(scmEventSTARTID, nullptr);
        *paDTO = var_DTO;
        *paCV = var_CV;
      }

      void evt_CLK(const CIEC_ARRAY_COMMON<CIEC_TIME> &paDT,
                   const CIEC_UINT &paN,
                   COutputParameter<CIEC_TIME> paDTO,
                   COutputParameter<CIEC_UINT> paCV) {
        COutputGuard guard_paDTO(paDTO);
        COutputGuard guard_paCV(paCV);
        var_DT = paDT;
        var_N = paN;
        executeEvent(scmEventCLKID, nullptr);
        *paDTO = var_DTO;
        *paCV = var_CV;
      }

      void operator()(const CIEC_ARRAY_COMMON<CIEC_TIME> &paDT,
                      const CIEC_UINT &paN,
                      COutputParameter<CIEC_TIME> paDTO,
                      COutputParameter<CIEC_UINT> paCV) {
        evt_START(paDT, paN, paDTO, paCV);
      }
  };
} // namespace forte::iec61499::events
