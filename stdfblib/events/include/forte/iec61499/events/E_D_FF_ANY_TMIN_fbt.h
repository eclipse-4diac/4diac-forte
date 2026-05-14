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
 *** Name: E_D_FF_ANY_TMIN
 *** Description: Data latch (d) flip flop, with a Minimum inter-disposal Time between EO
 *** Version:
 ***     1.0: 2026-05-14/Franz Höpfinger - HR Agrartechnik GmbH - Inital API
 *************************************************************************/

#pragma once

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "forte/datatypes/forte_any_variant.h"
#include "forte/datatypes/forte_time.h"
#include "forte/forte_st_util.h"
#include "forte/iec61499/events/E_D_FF_ANY_fbt.h"
#include "forte/iec61499/events/E_TMIN_fbt.h"

namespace forte::iec61499::events {
  class FORTE_E_D_FF_ANY_TMIN final : public CCompositeFB {
      DECLARE_FIRMWARE_FB(FORTE_E_D_FF_ANY_TMIN)

    private:
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventEOID = 1;
      static const TEventID scmEventINITID = 0;
      static const TEventID scmEventCLKID = 1;

      CInternalFB<forte::iec61499::events::FORTE_E_D_FF_ANY> fb_E_D_FF_ANY;
      CInternalFB<forte::iec61499::events::FORTE_E_TMIN> fb_E_TMIN;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_E_D_FF_ANY_TMIN(StringId paInstanceNameId, CFBContainer &paContainer);

      CEventConnection conn_INITO;
      CEventConnection conn_EO;

      CDataConnection *conn_D;
      CDataConnection *conn_Tmin;

      COutDataConnection<CIEC_ANY_VARIANT> conn_Q;

      COutDataConnection<CIEC_ANY_VARIANT> conn_if2in_D;
      COutDataConnection<CIEC_TIME> conn_if2in_Tmin;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
      CDataConnection *getIf2InConUnchecked(TPortId) override;
  };
} // namespace forte::iec61499::events
