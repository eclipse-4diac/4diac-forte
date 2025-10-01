/*******************************************************************************
 * Copyright (c) 2006 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/resource.h"
#include "forte/iec61499/events/E_RESTART_fbt.h"

namespace forte::iec61499::system {
  class EMB_RES : public CResource {
      DECLARE_FIRMWARE_FB(EMB_RES);

    public:
      EMB_RES(StringId paInstanceNameId, CFBContainer &paDevice);
      ~EMB_RES() override;

      CIEC_ANY *getDI(size_t) override;
      CDataConnection **getDIConUnchecked(TPortId) override;

    private:
      CInternalFB<events::FORTE_E_RESTART> fb_START;
  };
} // namespace forte::iec61499::system
