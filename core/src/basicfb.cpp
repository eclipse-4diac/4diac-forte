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

CBasicFB::CBasicFB(CFBContainer &paContainer,
                   const SFBInterfaceSpec &paInterfaceSpec,
                   const forte::StringId paInstanceNameId,
                   std::span<const forte::StringId> paVarInternalNames) :
    CBaseFB(paContainer, paInterfaceSpec, paInstanceNameId, paVarInternalNames),
    mECCState(0) {
}

void CBasicFB::setInitialValues() {
  CBaseFB::setInitialValues();
  mECCState = CIEC_STATE(0);
}

CIEC_ANY *CBasicFB::getVar(forte::StringId *paNameList, unsigned int paNameListSize) {
  CIEC_ANY *poRetVal = CBaseFB::getVar(paNameList, paNameListSize);
  if ((nullptr == poRetVal) && (1 == paNameListSize) && *paNameList == "!ECC"_STRID) {
    poRetVal = &mECCState;
  }
  return poRetVal;
}
