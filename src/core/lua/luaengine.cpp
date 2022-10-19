/*******************************************************************************
 * Copyright (c) 2015 - 2016 fortiss GmbH
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
#include "luamodule.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

CLuaEngine::CLuaEngine() {
  luaState = luaL_newstate();
  luaL_openlibs(luaState);

  //load lua module STfunc
  lua_getglobal(luaState, "package");
  lua_getfield(luaState, -1, "preload");

  // package, preload, luaJIT_BC_STfunc
  luaL_loadbuffer(luaState, (char *)luaJIT_BC_STfunc, luaJIT_BC_STfunc_SIZE, NULL);

  // package.preload.STfunc = luaJIT_BC_STfunc
  lua_setfield(luaState, -2, "STfunc");

}

CLuaEngine::~CLuaEngine() {
  lua_close(luaState);
}

bool CLuaEngine::loadString(const std::string& paStr) {
  int err = luaL_loadstring(luaState, paStr.c_str());
  if(err) {
    switch(err){
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
  if(!call(0, 1)) {
    DEVLOG_ERROR("Error loading definitions\n");
    return false;
  }
  return true;
}

bool CLuaEngine::call(int paArgs, int paResults) {
  int err = lua_pcall(luaState, paArgs, paResults, 0);
  if(err) {
    switch(err){
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

bool CLuaEngine::luaPushAny(lua_State *paLuaState, CIEC_ANY &paValue) {
  /*if(!paValue) {
    return false;
  }*/

  switch(paValue.getDataTypeID()){
    case CIEC_ANY::e_BOOL:
      lua_pushboolean(paLuaState, static_cast<CIEC_BOOL&>(paValue).operator bool()) ;
      break;
    case CIEC_ANY::e_SINT:
      lua_pushinteger(paLuaState, static_cast<CIEC_SINT&>(paValue).operator TForteInt8());
      break;
    case CIEC_ANY::e_INT:
      lua_pushinteger(paLuaState, static_cast<CIEC_INT&>(paValue).operator TForteInt16());
      break;
    case CIEC_ANY::e_DINT:
      lua_pushinteger(paLuaState, static_cast<CIEC_DINT&>(paValue).operator TForteInt32());
      break;
    case CIEC_ANY::e_LINT:
      lua_pushinteger(paLuaState, static_cast<lua_Integer>(static_cast<CIEC_LINT&>(paValue).operator TForteInt64()));
      break;
    case CIEC_ANY::e_USINT:
      lua_pushinteger(paLuaState, static_cast<CIEC_USINT&>(paValue).operator TForteUInt8());
      break;
    case CIEC_ANY::e_UINT:
      lua_pushinteger(paLuaState, static_cast<CIEC_UINT&>(paValue).operator TForteUInt16());
      break;
    case CIEC_ANY::e_UDINT:
      lua_pushinteger(paLuaState, static_cast<CIEC_UDINT&>(paValue).operator TForteUInt32());
      break;
    case CIEC_ANY::e_ULINT:
      lua_pushinteger(paLuaState, static_cast<lua_Integer>(static_cast<CIEC_ULINT&>(paValue).operator TForteUInt64()));
      break;
    case CIEC_ANY::e_BYTE:
      lua_pushinteger(paLuaState, static_cast<CIEC_BYTE&>(paValue).operator TForteByte());
      break;
    case CIEC_ANY::e_WORD:
      lua_pushinteger(paLuaState, static_cast<CIEC_WORD&>(paValue).operator TForteWord());
      break;
    case CIEC_ANY::e_DWORD:
      lua_pushinteger(paLuaState, static_cast<CIEC_DWORD&>(paValue).operator TForteDWord());
      break;
    case CIEC_ANY::e_LWORD:
      lua_pushinteger(paLuaState, static_cast<CIEC_LWORD&>(paValue).operator TForteLWord());
      break;
    case CIEC_ANY::e_DATE:
      lua_pushinteger(paLuaState, static_cast<lua_Integer>(static_cast<CIEC_DATE&>(paValue).operator TForteUInt64()));
      break;
    case CIEC_ANY::e_TIME_OF_DAY:
      lua_pushinteger(paLuaState, static_cast<lua_Integer>(static_cast<CIEC_TIME_OF_DAY&>(paValue).operator TForteUInt64()));
      break;
    case CIEC_ANY::e_DATE_AND_TIME:
      lua_pushinteger(paLuaState, static_cast<lua_Integer>(static_cast<CIEC_DATE_AND_TIME&>(paValue).operator TForteUInt64()));
      break;
    case CIEC_ANY::e_TIME:
      lua_pushinteger(paLuaState, static_cast<lua_Integer>(static_cast<TForteInt64>(static_cast<CIEC_TIME&>(paValue))));
      break;
    case CIEC_ANY::e_REAL:
      lua_pushnumber(paLuaState, static_cast<CIEC_REAL&>(paValue).operator TForteFloat());
      break;
    case CIEC_ANY::e_LREAL:
      lua_pushnumber(paLuaState, static_cast<CIEC_LREAL&>(paValue).operator TForteDFloat());
      break;
    case CIEC_ANY::e_STRING:
      lua_pushstring(paLuaState, static_cast<CIEC_STRING&>(paValue).getValue());
      break;
    case CIEC_ANY::e_WSTRING:
      lua_pushstring(paLuaState, static_cast<CIEC_WSTRING&>(paValue).getValue());
      break;
    case CIEC_ANY::e_ARRAY:
      return luaPushArray(paLuaState, static_cast<CIEC_ARRAY<>&>(paValue));
    default:
      DEVLOG_ERROR("Conversion to Lua datatype unsupported for type with id %s\n", paValue.getDataTypeID());
      lua_pushnil(paLuaState);
  }

  return true;
}

bool CLuaEngine::luaGetAny(lua_State *paLuaState, CIEC_ANY &paValue, int paIndex) {
  /*if(!paValue) {
    return false;
  }*/
  switch(paValue.getDataTypeID()){
    case CIEC_ANY::e_BOOL:
      static_cast<CIEC_BOOL&>(paValue) = CIEC_BOOL(static_cast<bool>(lua_toboolean(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_SINT:
      static_cast<CIEC_SINT&>(paValue) = CIEC_SINT(static_cast<TForteInt8>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_INT:
      static_cast<CIEC_INT&>(paValue) = CIEC_INT(static_cast<TForteInt16>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_DINT:
      static_cast<CIEC_DINT&>(paValue) = CIEC_DINT(static_cast<TForteInt32>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_LINT:
      static_cast<CIEC_LINT&>(paValue) = CIEC_LINT(static_cast<TForteInt64>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_USINT:
      static_cast<CIEC_USINT&>(paValue) = CIEC_USINT(static_cast<TForteUInt8>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_UINT:
      static_cast<CIEC_UINT&>(paValue) = CIEC_UINT(static_cast<TForteUInt16>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_UDINT:
      static_cast<CIEC_UDINT&>(paValue) = CIEC_UDINT(static_cast<TForteUInt32>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_ULINT:
      static_cast<CIEC_ULINT&>(paValue) = CIEC_ULINT(static_cast<TForteUInt64>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_BYTE:
      static_cast<CIEC_BYTE&>(paValue) = CIEC_BYTE(static_cast<TForteByte>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_WORD:
      static_cast<CIEC_WORD&>(paValue) = CIEC_WORD(static_cast<TForteWord>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_DWORD:
      static_cast<CIEC_DWORD&>(paValue) = CIEC_DWORD(static_cast<TForteDWord>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_LWORD:
      static_cast<CIEC_LWORD&>(paValue) = CIEC_LWORD(static_cast<TForteLWord>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_DATE:
      static_cast<CIEC_DATE&>(paValue) = CIEC_DATE(static_cast<TForteUInt64>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_TIME_OF_DAY:
      static_cast<CIEC_TIME_OF_DAY&>(paValue) = CIEC_TIME_OF_DAY(static_cast<TForteUInt64>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_DATE_AND_TIME:
      static_cast<CIEC_DATE_AND_TIME&>(paValue) = CIEC_DATE_AND_TIME(static_cast<TForteUInt64>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_TIME:
      static_cast<CIEC_TIME&>(paValue) = CIEC_TIME(static_cast<TForteUInt64>(lua_tointeger(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_REAL:
      static_cast<CIEC_REAL&>(paValue) = CIEC_REAL(static_cast<TForteFloat>(lua_tonumber(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_LREAL:
      static_cast<CIEC_LREAL&>(paValue) = CIEC_LREAL(static_cast<TForteDFloat>(lua_tonumber(paLuaState, paIndex)));
      break;
    case CIEC_ANY::e_STRING:
      static_cast<CIEC_STRING&>(paValue) = CIEC_STRING(lua_tostring(paLuaState, paIndex));
      break;
    case CIEC_ANY::e_WSTRING:
      static_cast<CIEC_WSTRING&>(paValue) = CIEC_WSTRING(lua_tostring(paLuaState, paIndex));
      break;
    case CIEC_ANY::e_ARRAY:
      return luaGetArray(paLuaState, static_cast<CIEC_ARRAY<>&>(paValue), paIndex);
    default:
      DEVLOG_ERROR("Conversion from Lua datatype unsupported for type with id %s\n", paValue.getDataTypeID());
      return false;
  }

  return true;
}

bool CLuaEngine::luaPushArray(lua_State *paLuaState, CIEC_ARRAY<>& paArray) {
  /*if(!paArray) {
    return false;
  }*/
  lua_createtable(paLuaState, paArray.size(), 0);
  for(int i = paArray.size(); i > 0; i--) {
    if(!luaPushAny(paLuaState, paArray[(TForteUInt16) (i - 1)])) { // index starts at 0
      lua_pop(paLuaState, 1); // pop table
      return false;
    }
    lua_rawseti(paLuaState, -2, i); // index starts at 1
  }
  return true;
}

bool CLuaEngine::luaGetArray(lua_State *paLuaState, CIEC_ARRAY<>& paArray, int paIndex) {
  if(/*!paArray || */!lua_istable(paLuaState, paIndex)) {
    return false;
  }
  for(int i = paArray.size(); i > 0; i--) {
    lua_rawgeti(paLuaState, paIndex, i); // index starts at 1
    bool res = luaGetAny(paLuaState, paArray[(TForteUInt16) (i - 1)], -1); // index starts at 0
    lua_pop(paLuaState, 1); // pop element
    if(!res) {
      return false;
    }
  }
  return true;
}
