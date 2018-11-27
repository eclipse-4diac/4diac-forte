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

#ifndef SRC_CORE_SIMPLEFB_H_
#define SRC_CORE_SIMPLEFB_H_

#include "basicfb.h"

class CSimpleFB : public CBasicFB{
  public:
    CSimpleFB(CResource *paSrcRes,
        const SFBInterfaceSpec *paInterfaceSpec,
        const CStringDictionary::TStringId paInstanceNameId,
        const SInternalVarsInformation *paVarInternals,
        TForteByte *paFBConnData,
        TForteByte *paBasicFBVarsData);
    virtual ~CSimpleFB();

    virtual void executeEvent(int paEIID);
    virtual void alg_REQ(void) = 0;
};

#endif /* SRC_CORE_SIMPLEFB_H_ */
