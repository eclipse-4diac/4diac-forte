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
 *    Alois Zoitl - introduced new CGenFB namespace forte::iec61131::arithmetic {
class for better handling generic FBs
 *   Martin Jobst
 *     - refactor for ANY variant
 *     - add generic readInputData and writeOutputData
 *******************************************************************************/

#pragma once

#include "genarithbase_fbt.h"
#include "forte/datatypes/forte_any_magnitude_variant.h"

namespace forte::iec61131::arithmetic {
  class GEN_ADD final : public CGenArithBase<CIEC_ANY_MAGNITUDE_VARIANT> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_ADD)

    private:
      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    public:
      GEN_ADD(const StringId paInstanceNameId, CFBContainer &paContainer);
      ~GEN_ADD() override = default;
  };
} // namespace forte::iec61131::arithmetic
