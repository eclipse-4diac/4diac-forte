/*************************************************************************
 *** Copyright (c) 2023, 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_PULSE
 *** Description: standard timer function block (pulse)
 *** Version:
 ***     1.0: 2023-08-21/Franz Hoepfinger - HR Agrartechnik GmbH - initial implementation as E_IMPULSE
 ***     1.0: 2024-03-05/Franz Hoepfinger - HR Agrartechnik GmbH - renamed to E_PULSE
 ***     1.1: 2024-04-23/Franz Hoepfinger - HR Agrartechnik GmbH - Add a Reset to Timer FBs
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 *************************************************************************/

#pragma once

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "forte/datatypes/forte_time.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/iec61499/events/E_DELAY_fbt.h"
#include "forte/iec61499/events/E_SR_fbt.h"

namespace forte::iec61499::events::timers {
  class FORTE_E_PULSE final : public CCompositeFB {
      DECLARE_FIRMWARE_FB(FORTE_E_PULSE)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventRID = 1;
      static const TEventID scmEventCNFID = 0;

      CInternalFB<forte::iec61499::events::FORTE_E_DELAY> fb_E_DELAY;
      CInternalFB<forte::iec61499::events::FORTE_E_SR> fb_E_SR;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;
      CDataConnection *getIf2InConUnchecked(TPortId paDIID) override;

    public:
      FORTE_E_PULSE(StringId paInstanceNameId, CFBContainer &paContainer);

      CEventConnection conn_CNF;

      CDataConnection *conn_PT;

      COutDataConnection<CIEC_BOOL> conn_Q;

      COutDataConnection<CIEC_TIME> conn_if2in_PT;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };
} // namespace forte::iec61499::events::timers
