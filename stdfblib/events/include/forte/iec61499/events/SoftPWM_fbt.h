/*******************************************************************************
 * Copyright (c) 2026 Thomas Mayr
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Mayr - Initial implementation.
 *******************************************************************************/

#pragma once

#include "forte/esfb.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_time.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/timerha.h"

namespace forte::iec61499::events {

  class FORTE_SoftPWM final : public CEventSourceFB {
      DECLARE_FIRMWARE_FB(FORTE_SoftPWM)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;
      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEOID) override;
      void setInitialValues() override;

      CIEC_ANY *getVarInternal(size_t paIndex);

      void startSequence(CEventChainExecutionThread *const paECET);
      void handleTimerTick(CEventChainExecutionThread *const paECET);
      void scheduleNextInterval();
      void stopSequence();
      void setStatus(const char *paStatus);

      bool computeTiming(const CIEC_TIME &paPeriod, const CIEC_UINT &paDuty, CIEC_TIME &paOnTime, CIEC_TIME &paOffTime);

      void applyPendingUpdate();

    public:
      FORTE_SoftPWM(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_TIME var_PeriodeTime;
      CIEC_UINT var_DutyCycle;

      CIEC_BOOL var_QO;
      CIEC_WSTRING var_STATUS;

      CDataConnection *conn_QI;
      CDataConnection *conn_PeriodeTime;
      CDataConnection *conn_DutyCycle;

      CEventConnection conn_CNF;
      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_WSTRING> conn_STATUS;

      CIEC_ANY *getDI(size_t paIndex) override;
      CIEC_ANY *getDO(size_t paIndex) override;
      CEventConnection *getEOConUnchecked(TPortId paIndex) override;
      CDataConnection **getDIConUnchecked(TPortId paIndex) override;
      CDataConnection *getDOConUnchecked(TPortId paIndex) override;

    private:
      bool mRunning;
      bool mHighPhase;

      CIEC_TIME mOnTime;
      CIEC_TIME mOffTime;

      bool mPendingUpdate;
      CIEC_UINT mPendingDutyCycle;
      CIEC_TIME mPendingOnTime;
      CIEC_TIME mPendingOffTime;
  };

} // namespace forte::iec61499::events
