/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH
 *                      2018 Johannes Kepler University
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB namespace forte::iec61131::bitwiseOperators {
class for better handling generic FBs
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#pragma once

#include "genbitbase_fbt.h"

namespace forte::iec61131::bitwiseOperators {
  class GEN_XOR : public CGenBitBase {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_XOR)

    private:
      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    public:
      GEN_XOR(const StringId paInstanceNameId, CFBContainer &paContainer);
      ~GEN_XOR() override = default;
  };
} // namespace forte::iec61131::bitwiseOperators
