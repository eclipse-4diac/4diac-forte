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

#ifndef SRC_CORE_LUAADAPTER_H_
#define SRC_CORE_LUAADAPTER_H_

#include "adapter.h"
#include "luaengine.h"
#include "forte_any.h"
#include "forte_array.h"
#include "luaadaptertypeentry.h"

class CLuaAdapter: public CAdapter {
private:
  const CLuaAdapterTypeEntry* typeEntry;

public:
  CLuaAdapter(CStringDictionary::TStringId instanceNameId, const CLuaAdapterTypeEntry* typeEntry, bool pa_bIsPlug, TForteByte *connData, TForteByte *varsData, CResource *resource);
  virtual ~CLuaAdapter();

  virtual CStringDictionary::TStringId getFBTypeId(void) const {
    return typeEntry->getTypeNameId();
  }
};

#endif /* SRC_CORE_LUAADAPTER_H_ */
