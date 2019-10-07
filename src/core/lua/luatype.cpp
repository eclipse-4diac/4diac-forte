/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst, Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "luatype.h"
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
