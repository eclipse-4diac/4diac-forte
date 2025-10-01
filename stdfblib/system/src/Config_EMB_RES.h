/*******************************************************************************
 * Copyright (c) 2024 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Markus Meingast
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#pragma once

#include "forte/resource.h"
#include "forte/iec61499/events/E_RESTART_fbt.h"
#include "forte/datatypes/forte_wstring.h"

namespace forte::iec61499::system {
  class Config_EMB_RES final : public CResource {
      DECLARE_FIRMWARE_FB(Config_EMB_RES);

    public:
      Config_EMB_RES(StringId paInstanceNameId, CFBContainer &paDevice);
      ~Config_EMB_RES() override;

      bool initialize() override;

      CIEC_ANY *getDI(size_t) override;

    private:
      CDataConnection **getDIConUnchecked(TPortId) override;

      CConnection *getResIf2InConnectionUnchecked(TPortId) override;

      CInternalFB<events::FORTE_E_RESTART> fb_START;

      CDataConnection *conn_opcua_namespace;
      COutDataConnection<CIEC_WSTRING> conn_opcua_namespace_int;

      static const StringId scmVarInputNameIds[];
      static const StringId scmDIDataTypeIds[];
  };
} // namespace forte::iec61499::system
