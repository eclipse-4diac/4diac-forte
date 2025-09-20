/*******************************************************************************
 * Copyright (c) 2007 - 2011 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/iec61499/events/E_SWITCH_fbt.h"
#include "forte/iec61499/events/E_D_FF_fbt.h"

namespace forte::iec61499::events {
  class FORTE_E_R_TRIG : public CCompositeFB {
      DECLARE_FIRMWARE_FB(FORTE_E_R_TRIG)

    private:
      static const TEventID scmEventEIID = 0;
      static const TEventID scmEventEOID = 0;

      CInternalFB<FORTE_E_D_FF> fb_E_D_FF;
      CInternalFB<FORTE_E_SWITCH> fb_E_SWITCH;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;
      CDataConnection *getIf2InConUnchecked(TPortId paDIID) override;

    public:
      FORTE_E_R_TRIG(const StringId paInstanceNameId, CFBContainer &paContainer);

      CEventConnection conn_EO;
      CDataConnection *conn_QI;
      COutDataConnection<CIEC_BOOL> conn_if2in_QI;
      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };
} // namespace forte::iec61499::events
