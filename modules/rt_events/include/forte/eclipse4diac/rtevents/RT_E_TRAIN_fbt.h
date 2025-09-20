/*******************************************************************************
 * Copyright (c) 2006 - 2014 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

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

#include "RT_E_CYCLE_fbt.h"
#include "forte/iec61499/events/E_CTU_fbt.h"
#include "forte/iec61499/events/E_SWITCH_fbt.h"

namespace forte::eclipse4diac::rtevents {
  class FORTE_RT_E_TRAIN final : public CCompositeFB {
      DECLARE_FIRMWARE_FB(FORTE_RT_E_TRAIN)

    private:
      static const TEventID scmEventSTARTID = 0;
      static const TEventID scmEventSTOPID = 1;
      static const TEventID scmEventEOID = 0;

      CInternalFB<FORTE_RT_E_CYCLE> fb_RT_E_CYCLE;
      CInternalFB<iec61499::events::FORTE_E_CTU> fb_E_CTU;
      CInternalFB<iec61499::events::FORTE_E_SWITCH> fb_E_SWITCH;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;
      CDataConnection *getIf2InConUnchecked(TPortId paDIID) override;

    public:
      FORTE_RT_E_TRAIN(const StringId paInstanceNameId, CFBContainer &paContainer);

      CEventConnection conn_EO;
      CDataConnection *conn_DT;
      CDataConnection *conn_N;
      CDataConnection *conn_Deadline;
      CDataConnection *conn_WCET;
      COutDataConnection<CIEC_UINT> conn_CV;
      COutDataConnection<CIEC_TIME> conn_if2in_DT;
      COutDataConnection<CIEC_UINT> conn_if2in_N;
      COutDataConnection<CIEC_TIME> conn_if2in_Deadline;
      COutDataConnection<CIEC_TIME> conn_if2in_WCET;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };
} // namespace forte::eclipse4diac::rtevents
