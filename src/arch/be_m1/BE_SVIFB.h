/*******************************************************************************
 * Copyright (c) 2011 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef BE_SVIFB_H_
#define BE_SVIFB_H_

#include <arch/be_m1/fortemodule.h>
#include "funcbloc.h"

class BE_SVIFB : public CFunctionBlock{
  public:
    BE_SVIFB(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId) :
        CFunctionBlock(paSrcRes, paInterfaceSpec, paInstanceNameId){
    }

  protected:
    bool registerSVIvar(const CIEC_STRING& paID, CIEC_ANY& paValue, const TForteUInt32 paMode);

  private:
    static const TForteUInt32 scmBE_SVI_Types[];
    static const TForteUInt32 scmBE_SVI_VarSizes[];

};

#endif /* BE_SVIFB_H_ */
