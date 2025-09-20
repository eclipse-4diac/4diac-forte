/*******************************************************************************
 * Copyright (c) 2017, 2025 fortiss GmbH, HR Agrartechnik GmbH
 *
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
 *** Name: E_TRAIN
 *** Description: Generate of a finite train of events seperated by the time DT
 *** Version:
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 ***     1.1: 2025-02-15/Franz Höpfinger -  HR Agrartechnik GmbH   - Bugfix
 *************************************************************************/

#pragma once

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_time.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/iec61499/events/E_CTU_fbt.h"
#include "forte/iec61499/events/E_DELAY_fbt.h"
#include "forte/iec61499/events/E_SWITCH_fbt.h"

namespace forte::iec61499::events {
  class FORTE_E_TRAIN final : public CCompositeFB {
      DECLARE_FIRMWARE_FB(FORTE_E_TRAIN)

    private:
      static const TEventID scmEventSTARTID = 0;
      static const TEventID scmEventSTOPID = 1;
      static const TEventID scmEventEOID = 0;

      CInternalFB<FORTE_E_CTU> fb_CTR;
      CInternalFB<FORTE_E_SWITCH> fb_GATE;
      CInternalFB<FORTE_E_DELAY> fb_DLY;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;
      CDataConnection *getIf2InConUnchecked(TPortId paDIID) override;

    public:
      FORTE_E_TRAIN(StringId paInstanceNameId, CFBContainer &paContainer);

      CEventConnection conn_EO;

      CDataConnection *conn_DT;
      CDataConnection *conn_N;

      COutDataConnection<CIEC_UINT> conn_CV;

      COutDataConnection<CIEC_TIME> conn_if2in_DT;
      COutDataConnection<CIEC_UINT> conn_if2in_N;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };
} // namespace forte::iec61499::events
