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
 *** Name: E_TMIN
 *** Description: Forwards Events with a minimum inter-arrival time between EI events
 *** Version:
 ***     1.0: 2026-05-14/Franz Höpfinger - HR Agrartechnik GmbH - Initial API
 *************************************************************************/

#pragma once

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "forte/datatypes/forte_time.h"
#include "forte/iec61499/events/E_DELAY_fbt.h"
#include "forte/iec61499/events/E_REND_fbt.h"

namespace forte::iec61499::events {
  class FORTE_E_TMIN final : public CCompositeFB {
      DECLARE_FIRMWARE_FB(FORTE_E_TMIN)

    private:
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventEOID = 1;
      static const TEventID scmEventINITID = 0;
      static const TEventID scmEventEIID = 1;

      CInternalFB<forte::iec61499::events::FORTE_E_REND> fb_E_REND;
      CInternalFB<forte::iec61499::events::FORTE_E_DELAY> fb_E_DELAY;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_E_TMIN(StringId paInstanceNameId, CFBContainer &paContainer);

      CEventConnection conn_INITO;
      CEventConnection conn_EO;

      CDataConnection *conn_Tmin;

      COutDataConnection<CIEC_TIME> conn_if2in_Tmin;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
      CDataConnection *getIf2InConUnchecked(TPortId) override;
  };
} // namespace forte::iec61499::events
