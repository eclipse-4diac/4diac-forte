/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61499/events/E_PERMIT_fbt.h"
#include "forte/iec61499/events/E_SR_fbt.h"
#include "forte/iec61499/events/E_SWITCH_fbt.h"
#include "forte/iec61499/events/GEN_E_DEMUX_fbt.h"
#include "forte/iec61499/events/GEN_E_MUX_fbt.h"

namespace forte::test {
  class FORTE_CFB_TEST final : public CCompositeFB {
      DECLARE_FIRMWARE_FB(FORTE_CFB_TEST)

    private:
      static const TEventID scmEventSETID = 0;
      static const TEventID scmEventRESETID = 1;
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventCHANGEDID = 1;

      CInternalFB<iec61499::events::FORTE_E_PERMIT> fb_PERMIT_OP;
      CInternalFB<iec61499::events::FORTE_E_SR> fb_E_SR;
      CInternalFB<iec61499::events::FORTE_E_SWITCH> fb_SET_CHANGED;
      CInternalFB<iec61499::events::GEN_E_DEMUX> fb_E_DEMUX_2;
      CInternalFB<iec61499::events::GEN_E_MUX> fb_E_MUX_2;
      CInternalFB<iec61499::events::FORTE_E_SWITCH> fb_RESET_CHANGED;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;
      CDataConnection *getIf2InConUnchecked(TPortId paDIID) override;

    public:
      FORTE_CFB_TEST(StringId paInstanceNameId, CFBContainer &paContainer);

      CEventConnection conn_CNF;
      CEventConnection conn_CHANGED;

      CDataConnection *conn_QI;

      COutDataConnection<CIEC_BOOL> conn_QO;

      COutDataConnection<CIEC_BOOL> conn_if2in_QI;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };
} // namespace forte::test
