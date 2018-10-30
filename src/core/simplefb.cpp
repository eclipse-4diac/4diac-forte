/*******************************************************************************
  * Copyright (c) 2018 TU Wien/ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Martin Melik Merkumians
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/

#include "simplefb.h"

CSimpleFB::CSimpleFB(CResource *paSrcRes,
    const SFBInterfaceSpec *paInterfaceSpec,
    const CStringDictionary::TStringId paInstanceNameId,
    const SInternalVarsInformation *paVarInternals,
    TForteByte *paFBConnData,
    TForteByte *paBasicFBVarsData) :
    CBasicFB(paSrcRes, paInterfaceSpec, paInstanceNameId, paVarInternals, paFBConnData, paBasicFBVarsData){
      
}

CSimpleFB::~CSimpleFB() {

}

void CSimpleFB::executeEvent(int paEIID){
  if (0 == paEIID) {
    alg_REQ();
    sendOutputEvent(0);
  }
}
