/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/basicfb.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61499::events {
  class FORTE_E_DEMUX final : public CBasicFB {
      DECLARE_FIRMWARE_FB(FORTE_E_DEMUX)

    private:
      static const TEventID scmEventEIID = 0;
      static const TEventID scmEventEO0ID = 0;
      static const TEventID scmEventEO1ID = 1;
      static const TEventID scmEventEO2ID = 2;
      static const TEventID scmEventEO3ID = 3;

      CIEC_ANY *getVarInternal(size_t) override;

      static const TForteInt16 scmStateSTART = 0;
      static const TForteInt16 scmStateState = 1;
      static const TForteInt16 scmStateState_1 = 2;
      static const TForteInt16 scmStateState_2 = 3;
      static const TForteInt16 scmStateState_3 = 4;
      static const TForteInt16 scmStateState_4 = 5;

      void enterStateSTART(CEventChainExecutionThread *const paECET);
      void enterStateState(CEventChainExecutionThread *const paECET);
      void enterStateState_1(CEventChainExecutionThread *const paECET);
      void enterStateState_2(CEventChainExecutionThread *const paECET);
      void enterStateState_3(CEventChainExecutionThread *const paECET);
      void enterStateState_4(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_E_DEMUX(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_UINT var_K;

      CEventConnection conn_EO0;
      CEventConnection conn_EO1;
      CEventConnection conn_EO2;
      CEventConnection conn_EO3;

      CDataConnection *conn_K;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_EI(const CIEC_UINT &paK) {
        var_K = paK;
        receiveInputEvent(scmEventEIID, nullptr);
      }

      void operator()(const CIEC_UINT &paK) {
        evt_EI(paK);
      }
  };
} // namespace forte::iec61499::events
