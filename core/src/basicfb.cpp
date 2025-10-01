/*******************************************************************************
 * Copyright (c) 2005, 2025 Profactor GmbH, ACIN, fortiss GmbH,
 *                          Martin Erich Jobst, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Gunnar Grabmair, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *   Martin Jobst - account for data type size in FB initialization
 *   Alois Zoitl  - exracted internal variable handling to new CBaseFB
 *******************************************************************************/
#include <string.h>
#include "forte/basicfb.h"

using namespace forte::literals;

namespace forte {
  CBasicFB::CBasicFB(CFBContainer &paContainer,
                     const SFBInterfaceSpec &paInterfaceSpec,
                     const StringId paInstanceNameId,
                     std::span<const StringId> paVarInternalNames) :
      CBaseFB(paContainer, paInterfaceSpec, paInstanceNameId, paVarInternalNames),
      mECCState(0) {
  }

  void CBasicFB::setInitialValues() {
    CBaseFB::setInitialValues();
    mECCState = CIEC_STATE(0);
  }

  CIEC_ANY *CBasicFB::getVar(const std::span<const StringId> paNameList) {
    if (paNameList.empty()) {
      return nullptr;
    }
    if (paNameList.front() == "!ECC"_STRID) {
      return mECCState.getVar(paNameList.subspan(1));
    };
    return CBaseFB::getVar(paNameList);
  }
} // namespace forte
