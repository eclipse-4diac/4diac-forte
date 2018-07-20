/*******************************************************************************
 * Copyright (c) 2015 - 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Martin Jobst, Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "luaengine.h"

#include "../datatypes/forte_any.h"
#include "../datatypes/forte_bool.h"
#include "../datatypes/forte_sint.h"
#include "../datatypes/forte_int.h"
#include "../datatypes/forte_dint.h"
#include "../datatypes/forte_lint.h"
#include "../datatypes/forte_usint.h"
#include "../datatypes/forte_uint.h"
#include "../datatypes/forte_udint.h"
#include "../datatypes/forte_ulint.h"
#include "../datatypes/forte_byte.h"
#include "../datatypes/forte_word.h"
#include "../datatypes/forte_dword.h"
#include "../datatypes/forte_lword.h"
#include "../datatypes/forte_real.h"
#include "../datatypes/forte_lreal.h"
#include "../datatypes/forte_date.h"
#include "../datatypes/forte_time_of_day.h"
#include "../datatypes/forte_date_and_time.h"
#include "../datatypes/forte_time.h"
#include "../datatypes/forte_array.h"
#include "../../arch/devlog.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

CLuaEngine::CLuaEngine() {
  luaState = luaL_newstate();
  luaL_openlibs(luaState);
}

CLuaEngine::~CLuaEngine() {
  lua_close(luaState);
}

bool CLuaEngine::loadString(const std::string& str) {
  int err = luaL_loadstring(luaState, str.c_str());
  if (err) {
    switch (err) {
      case LUA_ERRSYNTAX:
        DEVLOG_ERROR("Syntax error loading Lua definitions\n");
        break;
      case LUA_ERRMEM:
        DEVLOG_ERROR("Allocation error loading Lua definitions\n");
        break;
      default:
        DEVLOG_ERROR("Unknown error loading Lua definitions\n");
    }
    return false;
  }
  if (!call(0, 1)) {
    DEVLOG_ERROR("Error loading definitions\n");
    return false;
  }
  return true;
}

bool CLuaEngine::call(int args, int results) {
  int err = lua_pcall(luaState, args, results, 0);
  if (err) {
    switch (err) {
      case LUA_ERRRUN:
        DEVLOG_ERROR("Runtime error calling Lua function: %s\n", lua_tostring(luaState, -1));
        break;
      case LUA_ERRMEM:
        DEVLOG_ERROR("Memory allocation error calling Lua function\n");
        break;
      case LUA_ERRERR:
        DEVLOG_ERROR("Message handler error calling Lua function\n");
        break;
      default:
        DEVLOG_ERROR("Unknown error calling Lua function\n");
    }
    return false;
  }
  return true;
}

bool CLuaEngine::luaPushAny(lua_State *luaState, CIEC_ANY *value) {
  if (!value)
    return false;

  switch (value->getDataTypeID()) {
    case CIEC_ANY::e_BOOL:
      lua_pushboolean(luaState, static_cast<bool>(*static_cast<CIEC_BOOL*>(value)));
      break;
    case CIEC_ANY::e_SINT:
      lua_pushinteger(luaState, static_cast<TForteInt8>(*static_cast<CIEC_SINT*>(value)));
      break;
    case CIEC_ANY::e_INT:
      lua_pushinteger(luaState, static_cast<TForteInt16>(*static_cast<CIEC_INT*>(value)));
      break;
    case CIEC_ANY::e_DINT:
      lua_pushinteger(luaState, static_cast<TForteInt32>(*static_cast<CIEC_DINT*>(value)));
      break;
    case CIEC_ANY::e_LINT:
      lua_pushinteger(luaState, static_cast<TForteInt64>(*static_cast<CIEC_LINT*>(value)));
      break;
    case CIEC_ANY::e_USINT:
      lua_pushinteger(luaState, static_cast<TForteUInt8>(*static_cast<CIEC_USINT*>(value)));
      break;
    case CIEC_ANY::e_UINT:
      lua_pushinteger(luaState, static_cast<TForteUInt16>(*static_cast<CIEC_UINT*>(value)));
      break;
    case CIEC_ANY::e_UDINT:
      lua_pushinteger(luaState, static_cast<TForteUInt32>(*static_cast<CIEC_UDINT*>(value)));
      break;
    case CIEC_ANY::e_ULINT:
      lua_pushinteger(luaState, static_cast<TForteUInt64>(*static_cast<CIEC_ULINT*>(value)));
      break;
    case CIEC_ANY::e_BYTE:
      lua_pushinteger(luaState, static_cast<TForteByte>(*static_cast<CIEC_BYTE*>(value)));
      break;
    case CIEC_ANY::e_WORD:
      lua_pushinteger(luaState, static_cast<TForteWord>(*static_cast<CIEC_WORD*>(value)));
      break;
    case CIEC_ANY::e_DWORD:
      lua_pushinteger(luaState, static_cast<TForteDWord>(*static_cast<CIEC_DWORD*>(value)));
      break;
    case CIEC_ANY::e_LWORD:
      lua_pushinteger(luaState, static_cast<TForteLWord>(*static_cast<CIEC_LWORD*>(value)));
      break;
    case CIEC_ANY::e_DATE:
      lua_pushinteger(luaState, static_cast<TForteUInt64>(*static_cast<CIEC_DATE*>(value)));
      break;
    case CIEC_ANY::e_TIME_OF_DAY:
      lua_pushinteger(luaState, static_cast<TForteUInt64>(*static_cast<CIEC_TIME_OF_DAY*>(value)));
      break;
    case CIEC_ANY::e_DATE_AND_TIME:
      lua_pushinteger(luaState, static_cast<TForteUInt64>(*static_cast<CIEC_DATE_AND_TIME*>(value)));
      break;
    case CIEC_ANY::e_TIME:
      lua_pushinteger(luaState, static_cast<TForteInt64>(*static_cast<CIEC_TIME*>(value)));
      break;
    case CIEC_ANY::e_REAL:
      lua_pushnumber(luaState, static_cast<TForteFloat>(*static_cast<CIEC_REAL*>(value)));
      break;
    case CIEC_ANY::e_LREAL:
      lua_pushnumber(luaState, static_cast<TForteDFloat>(*static_cast<CIEC_LREAL*>(value)));
      break;
    case CIEC_ANY::e_STRING:
      lua_pushstring(luaState, static_cast<CIEC_STRING*>(value)->getValue());
      break;
    case CIEC_ANY::e_WSTRING:
      lua_pushstring(luaState, static_cast<CIEC_WSTRING*>(value)->getValue());
      break;
    case CIEC_ANY::e_ARRAY:
      return luaPushArray(luaState, static_cast<CIEC_ARRAY*>(value));
    default:
      DEVLOG_ERROR("Conversion to Lua datatype unsupported for type with id %s\n", value->getDataTypeID());
      lua_pushnil(luaState);
  }

  return true;
}

bool CLuaEngine::luaGetAny(lua_State *luaState, CIEC_ANY *value, int index) {
  if (!value)
    return false;
  switch (value->getDataTypeID()) {
    case CIEC_ANY::e_BOOL:
      *static_cast<CIEC_BOOL*>(value) = static_cast<bool>(lua_toboolean(luaState, index));
      break;
    case CIEC_ANY::e_SINT:
      *static_cast<CIEC_SINT*>(value) = static_cast<TForteInt8>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_INT:
      *static_cast<CIEC_INT*>(value) = static_cast<TForteInt16>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_DINT:
      *static_cast<CIEC_DINT*>(value) = static_cast<TForteInt32>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_LINT:
      *static_cast<CIEC_LINT*>(value) = static_cast<TForteInt64>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_USINT:
      *static_cast<CIEC_USINT*>(value) = static_cast<TForteUInt8>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_UINT:
      *static_cast<CIEC_UINT*>(value) = static_cast<TForteUInt16>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_UDINT:
      *static_cast<CIEC_UDINT*>(value) = static_cast<TForteUInt32>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_ULINT:
      *static_cast<CIEC_ULINT*>(value) = static_cast<TForteUInt64>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_BYTE:
      *static_cast<CIEC_BYTE*>(value) = static_cast<TForteByte>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_WORD:
      *static_cast<CIEC_WORD*>(value) = static_cast<TForteWord>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_DWORD:
      *static_cast<CIEC_DWORD*>(value) = static_cast<TForteDWord>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_LWORD:
      *static_cast<CIEC_LWORD*>(value) = static_cast<TForteLWord>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_DATE:
      *static_cast<CIEC_DATE*>(value) = static_cast<TForteUInt64>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_TIME_OF_DAY:
      *static_cast<CIEC_TIME_OF_DAY*>(value) = static_cast<TForteUInt64>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_DATE_AND_TIME:
      *static_cast<CIEC_DATE_AND_TIME*>(value) = static_cast<TForteUInt64>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_TIME:
      *static_cast<CIEC_TIME*>(value) = static_cast<TForteInt64>(lua_tointeger(luaState, index));
      break;
    case CIEC_ANY::e_REAL:
      *static_cast<CIEC_REAL*>(value) = static_cast<TForteFloat>(lua_tonumber(luaState, index));
      break;
    case CIEC_ANY::e_LREAL:
      *static_cast<CIEC_LREAL*>(value) = static_cast<TForteDFloat>(lua_tonumber(luaState, index));
      break;
    case CIEC_ANY::e_STRING:
      *static_cast<CIEC_STRING*>(value) = strdup(lua_tostring(luaState, index));
      break;
    case CIEC_ANY::e_WSTRING:
      *static_cast<CIEC_WSTRING*>(value) = strdup(lua_tostring(luaState, index));
      break;
    case CIEC_ANY::e_ARRAY:
      return luaGetArray(luaState, static_cast<CIEC_ARRAY*>(value), index);
    default:
      DEVLOG_ERROR("Conversion from Lua datatype unsupported for type with id %s\n", value->getDataTypeID());
      return false;
  }

  return true;
}

bool CLuaEngine::luaPushArray(lua_State *luaState, CIEC_ARRAY* array) {
  if (!array) {
    return false;
  }
  lua_createtable(luaState, array->size(), 0);
  for (int i = array->size(); i > 0; i--) {
    if (!luaPushAny(luaState, (*array)[(TForteUInt16)(i - 1)])) { // index starts at 0
      lua_pop(luaState, 1); // pop table
      return false;
    }
    lua_rawseti(luaState, -2, i); // index starts at 1
  }
  return true;
}

bool CLuaEngine::luaGetArray(lua_State *luaState, CIEC_ARRAY* array, int index) {
  if (!array || !lua_istable(luaState, index)) {
    return false;
  }
  for (int i = array->size(); i > 0; i--) {
    lua_rawgeti(luaState, index, i); // index starts at 1
    bool res = luaGetAny(luaState, (*array)[(TForteUInt16)(i - 1)], -1); // index starts at 0
    lua_pop(luaState, 1); // pop element
    if (!res) {
      return false;
    }
  }
  return true;
}
