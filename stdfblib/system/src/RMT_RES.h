/*******************************************************************************
 * Copyright (c) 2005, 2025 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Thomas Strasser,
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/resource.h"
#include "forte/iec61499/events/E_RESTART_fbt.h"
#include "forte/iec61499/events/E_SR_fbt.h"
#include "DEV_MGR.h"

namespace forte::iec61499::system {
  class RMT_RES final : public CResource {
      DECLARE_FIRMWARE_FB(RMT_RES)

    public:
      RMT_RES(StringId paInstanceNameId, CFBContainer &paDevice);
      ~RMT_RES() override;

      bool initialize() override;

      void joinResourceThread() const;

      CIEC_ANY *getDI(size_t) override;

    private:
      CDataConnection **getDIConUnchecked(TPortId) override;
      CConnection *getResIf2InConnectionUnchecked(TPortId) override;

      CDataConnection *conn_MGR_ID;
      COutDataConnection<CIEC_WSTRING> conn_MGR_ID_int;

      CInternalFB<events::FORTE_E_RESTART> fb_START;
      CInternalFB<events::FORTE_E_SR> fb_MGR_FF;
      CInternalFB<DEV_MGR> fb_MGR;

      static const StringId scmVarInputNameIds[];
      static const StringId scmDIDataTypeIds[];
  };
} // namespace forte::iec61499::system
