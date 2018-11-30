/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_CORE_LUACFB_H_
#define SRC_CORE_LUACFB_H_

#include "cfb.h"
#include "luacfbtypeentry.h"

class CLuaCFB : public CCompositeFB {
  private:
    const CLuaCFBTypeEntry* mTypeEntry;

  public:
    CLuaCFB(CStringDictionary::TStringId paInstanceNameId, const CLuaCFBTypeEntry* paTypeEntry, SCFB_FBNData *paFbnData, TForteByte *paConnData,
        TForteByte *paVarsData, CResource *paResource);
    virtual ~CLuaCFB();

    virtual CStringDictionary::TStringId getFBTypeId(void) const {
      return mTypeEntry->getTypeNameId();
    }
};

#endif /* SRC_CORE_LUACFB_H_ */
