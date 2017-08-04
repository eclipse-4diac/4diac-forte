/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Martin Jobst, Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "funcbloc.h"
#include "luaengine.h"

namespace luatype {
  bool getAdapterInstanceDefinition(SAdapterInstanceDef& def, CLuaEngine* luaEngine, int index) {
    def.m_nAdapterNameID = luaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(index, "adapterNameID");
    def.m_nAdapterTypeNameID = luaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(index, "adapterTypeNameID");
    def.m_bIsPlug = luaEngine->getField<bool, &CLuaEngine::getBoolean>(index, "isPlug");
    if (def.m_nAdapterNameID == CStringDictionary::scm_nInvalidStringId || def.m_nAdapterTypeNameID == CStringDictionary::scm_nInvalidStringId) {
      return false;
    }
    return true;
  }

  bool getTypeNameId(CStringDictionary::TStringId& id, CLuaEngine* luaEngine, int index) {
    switch (luaEngine->type(index)) {
      case CLuaEngine::TNUMBER:
        id = luaEngine->getInteger<CStringDictionary::TStringId>(index);
        break;
      case CLuaEngine::TSTRING:
        id = luaEngine->getStringId(index);
        break;
      default:
        return false;
    }
    return true;
  }
}
