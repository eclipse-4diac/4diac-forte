/*************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: RangeBasedPulse
 *** Description: Distance based Impulse Generator
 *** Version:
 ***     1.0: 2024-10-18/Franz Höpfinger - HR Agrartechnik GmbH - initial Version
 ***     1.1: 2024-10-18/Franz Höpfinger - HR Agrartechnik GmbH - added Offset
 *************************************************************************/

#pragma once

#include "forte/simplefb.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_udint.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::signalprocessing::distance {
  class FORTE_RangeBasedPulse final : public CSimpleFB {
      DECLARE_FIRMWARE_FB(FORTE_RangeBasedPulse)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      CIEC_UDINT var_DIST_REMAINDER;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_REQ(void);

      void enterStateREQ(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_RangeBasedPulse(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_UDINT var_DIST_IN;
      CIEC_UDINT var_DIST_OFF;
      CIEC_UDINT var_DIST_HIGH;
      CIEC_UDINT var_DIST_LOW;

      CIEC_BOOL var_Q;

      CEventConnection conn_CNF;

      CDataConnection *conn_DIST_IN;
      CDataConnection *conn_DIST_OFF;
      CDataConnection *conn_DIST_HIGH;
      CDataConnection *conn_DIST_LOW;

      COutDataConnection<CIEC_BOOL> conn_Q;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_UDINT &paDIST_IN,
                   const CIEC_UDINT &paDIST_OFF,
                   const CIEC_UDINT &paDIST_HIGH,
                   const CIEC_UDINT &paDIST_LOW,
                   CAnyBitOutputParameter<CIEC_BOOL> paQ) {
        COutputGuard guard_paQ(paQ);
        var_DIST_IN = paDIST_IN;
        var_DIST_OFF = paDIST_OFF;
        var_DIST_HIGH = paDIST_HIGH;
        var_DIST_LOW = paDIST_LOW;
        executeEvent(scmEventREQID, nullptr);
        *paQ = var_Q;
      }

      void operator()(const CIEC_UDINT &paDIST_IN,
                      const CIEC_UDINT &paDIST_OFF,
                      const CIEC_UDINT &paDIST_HIGH,
                      const CIEC_UDINT &paDIST_LOW,
                      CAnyBitOutputParameter<CIEC_BOOL> paQ) {
        evt_REQ(paDIST_IN, paDIST_OFF, paDIST_HIGH, paDIST_LOW, paQ);
      }
  };
} // namespace forte::eclipse4diac::signalprocessing::distance
