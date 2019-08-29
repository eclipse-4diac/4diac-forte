/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_CORE_LUA_LUATYPE_H_
#define SRC_CORE_LUA_LUATYPE_H_

#include "../stringdict.h"

class SAdapterInstanceDef;
class CLuaEngine;

namespace luatype {
  bool getAdapterInstanceDefinition(SAdapterInstanceDef& def, CLuaEngine* luaEngine, int index);
  bool getTypeNameId(CStringDictionary::TStringId& id, CLuaEngine* luaEngine, int index);
}

#endif /* SRC_CORE_LUA_LUATYPE_H_ */
