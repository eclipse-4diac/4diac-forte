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
#include "basicfb.h"

CBasicFB::CBasicFB(forte::core::CFBContainer &paContainer,
                   const SFBInterfaceSpec &paInterfaceSpec,
                   const CStringDictionary::TStringId paInstanceNameId,
                   const SInternalVarsInformation *const paVarInternals) :
    CBaseFB(paContainer, paInterfaceSpec, paInstanceNameId, paVarInternals),
    mECCState(0) {
}

void CBasicFB::setInitialValues() {
  CBaseFB::setInitialValues();
  mECCState = CIEC_STATE(0);
}

CIEC_ANY *CBasicFB::getVar(CStringDictionary::TStringId *paNameList, unsigned int paNameListSize) {
  CIEC_ANY *poRetVal = CBaseFB::getVar(paNameList, paNameListSize);
  if ((nullptr == poRetVal) && (1 == paNameListSize) &&
      !strcmp("!ECC", CStringDictionary::get(
                          *paNameList))) { // TODO consider if this can also be an string ID in a different way
    poRetVal = &mECCState;
  }
  return poRetVal;
}
