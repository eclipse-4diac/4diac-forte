/*************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202604172003!
 ***
 *** Name: E_TONOF
 *** Description: standard timer function block (on/off-delay timing)
 *** Version:
 ***     3.2: 2026-06-22/Franz Höpfinger - HR Agrartechnik GmbH - validate all timers
 ***     3.1: 2026-05-08/Franz Höpfinger - HR Agrartechnik GmbH - E_RF_TRIG instead of E_SWITCH
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-04-23/Franz Höpfinger - HR Agrartechnik GmbH - Add a Reset to Timer FBs
 ***     1.0: 2024-03-04/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#pragma once

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_time.h"
#include "forte/forte_st_util.h"
#include "forte/iec61499/events/E_DELAY_fbt.h"
#include "forte/iec61499/events/E_RF_TRIG_fbt.h"
#include "forte/iec61499/events/E_SR_fbt.h"

namespace forte::iec61499::events::timers {
  class FORTE_E_TONOF final : public CCompositeFB {
      DECLARE_FIRMWARE_FB(FORTE_E_TONOF)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventRID = 1;

      CInternalFB<forte::iec61499::events::FORTE_E_RF_TRIG> fb_E_RF_TRIG;
      CInternalFB<forte::iec61499::events::FORTE_E_DELAY> fb_E_DELAY_ON;
      CInternalFB<forte::iec61499::events::FORTE_E_SR> fb_E_SR;
      CInternalFB<forte::iec61499::events::FORTE_E_DELAY> fb_E_DELAY_OFF;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_E_TONOF(StringId paInstanceNameId, CFBContainer &paContainer);

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;
      CDataConnection *conn_PT_ON;
      CDataConnection *conn_PT_OFF;

      COutDataConnection<CIEC_BOOL> conn_Q;

      COutDataConnection<CIEC_BOOL> conn_if2in_IN;
      COutDataConnection<CIEC_TIME> conn_if2in_PT_ON;
      COutDataConnection<CIEC_TIME> conn_if2in_PT_OFF;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
      CDataConnection *getIf2InConUnchecked(TPortId) override;
  };
} // namespace forte::iec61499::events::timers
