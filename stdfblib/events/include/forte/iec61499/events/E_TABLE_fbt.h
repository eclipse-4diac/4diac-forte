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
 *** Name: E_TABLE
 *** Description: Composite Function Block Type
 *** Version:
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 *************************************************************************/

#pragma once

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "forte/datatypes/forte_time.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/iec61499/events/E_DELAY_fbt.h"
#include "forte/iec61499/events/E_TABLE_CTRL_fbt.h"

namespace forte::iec61499::events {
  class FORTE_E_TABLE final : public CCompositeFB {
      DECLARE_FIRMWARE_FB(FORTE_E_TABLE)

    private:
      static const TEventID scmEventSTARTID = 0;
      static const TEventID scmEventSTOPID = 1;
      static const TEventID scmEventEOID = 0;

      CInternalFB<FORTE_E_TABLE_CTRL> fb_E_TABLE_CTRL;
      CInternalFB<FORTE_E_DELAY> fb_E_DELAY;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;
      CDataConnection *getIf2InConUnchecked(TPortId paDIID) override;

    public:
      FORTE_E_TABLE(StringId paInstanceNameId, CFBContainer &paContainer);

      CEventConnection conn_EO;

      CDataConnection *conn_DT;
      CDataConnection *conn_N;

      COutDataConnection<CIEC_UINT> conn_CV;

      COutDataConnection<CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3>> conn_if2in_DT;
      COutDataConnection<CIEC_UINT> conn_if2in_N;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };
} // namespace forte::iec61499::events
