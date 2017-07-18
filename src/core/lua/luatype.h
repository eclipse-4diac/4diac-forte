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

#ifndef SRC_CORE_LUA_LUATYPE_H_
#define SRC_CORE_LUA_LUATYPE_H_

namespace luatype {
  bool getAdapterInstanceDefinition(SAdapterInstanceDef& def, CLuaEngine* luaEngine, int index);
  bool getTypeNameId(CStringDictionary::TStringId& id, CLuaEngine* luaEngine, int index);
}

#endif /* SRC_CORE_LUA_LUATYPE_H_ */
