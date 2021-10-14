/*******************************************************************************
 * Copyright (c) 2018 TU Wien/ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#include "simplefb.h"

CSimpleFB::CSimpleFB(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId,
    const SInternalVarsInformation *paVarInternals, TForteByte *paFBConnData, TForteByte *paBasicFBVarsData) :
    CBasicFB(paSrcRes, paInterfaceSpec, paInstanceNameId, paVarInternals, paFBConnData, paBasicFBVarsData) {

}

CSimpleFB::CSimpleFB(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec, const CStringDictionary::TStringId pa_nInstanceNameId,
    const SInternalVarsInformation *pa_pstVarInternals, TForteByte *pa_acFBConnData, TForteByte *pa_acBasicFBVarsData,
    const SCFB_FBInstanceData *const pa_astInternalFBs, const size_t pa_numberOfInternalFbs) :
        CBasicFB(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId, pa_pstVarInternals, pa_acFBConnData, pa_acBasicFBVarsData, pa_astInternalFBs,
          pa_numberOfInternalFbs) {

}

CSimpleFB::~CSimpleFB() {

}

void CSimpleFB::executeEvent(int paEIID) {
  if(0 == paEIID) {
    alg_REQ();
    sendOutputEvent(0);
  }
}
