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

class CLuaCFB: public CCompositeFB {
private:
  const CLuaCFBTypeEntry* typeEntry;

public:
  CLuaCFB(CStringDictionary::TStringId instanceNameId, const CLuaCFBTypeEntry* typeEntry, SCFB_FBNData *fbnData, TForteByte *connData, TForteByte *varsData, CResource *resource);
  virtual ~CLuaCFB();

  virtual CStringDictionary::TStringId getFBTypeId(void) const {
    return typeEntry->getTypeNameId();
  }
};

#endif /* SRC_CORE_LUACFB_H_ */
