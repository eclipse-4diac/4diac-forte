/*************************************************************************
 *** Copyright (c) 2026 HR Agrartechnik GmbH
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
 *** Name: E_RF_TRIG
 *** Description: Boolean falling and rising edge detection
 *** Version:
 ***     1.0: 2026-05-08/Franz Höpfinger - HR Agrartechnik GmbH - initial API and implementation and/or initial
 *documentation
 *************************************************************************/

#pragma once

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61499/events/E_D_FF_fbt.h"
#include "forte/iec61499/events/E_SWITCH_fbt.h"

namespace forte::iec61499::events {
  class FORTE_E_RF_TRIG final : public CCompositeFB {
      DECLARE_FIRMWARE_FB(FORTE_E_RF_TRIG)

    private:
      static const TEventID scmEventERID = 0;
      static const TEventID scmEventEFID = 1;
      static const TEventID scmEventEIID = 0;

      CInternalFB<forte::iec61499::events::FORTE_E_D_FF> fb_E_D_FF;
      CInternalFB<forte::iec61499::events::FORTE_E_SWITCH> fb_E_SWITCH;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_E_RF_TRIG(StringId paInstanceNameId, CFBContainer &paContainer);

      CEventConnection conn_ER;
      CEventConnection conn_EF;

      CDataConnection *conn_QI;

      COutDataConnection<CIEC_BOOL> conn_if2in_QI;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
      CDataConnection *getIf2InConUnchecked(TPortId) override;
  };
} // namespace forte::iec61499::events
