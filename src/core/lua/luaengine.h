/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_CORE_LUAENGINE_H_
#define SRC_CORE_LUAENGINE_H_

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <string>
#include <algorithm>

#if !defined LUA_VERSION_NUM || LUA_VERSION_NUM==501
#define lua_rawgetp(L,i,p) (lua_pushlightuserdata(L, const_cast<void*>(p)), lua_rawget(L, (i)), lua_type(luaState, -1))
#define lua_rawsetp(L,i,p) (lua_pushlightuserdata(L, const_cast<void*>(p)), lua_insert(L, -2), lua_rawset(L, (i)), lua_type(luaState, -1))
#define lua_len(L,i) (lua_objlen(L, (i)))
#endif

#include "../datatypes/forte_any.h"

class CIEC_ARRAY;


class CLuaEngine {
private:
  lua_State* luaState;

public:
  enum CLuaType {
    TNONE = -1, TNIL = 0, TBOOLEAN = 1, TLIGHTUSERDATA = 2, TNUMBER = 3, TSTRING = 4, TTABLE = 5, TFUNCTION = 6, TUSERDATA = 7, TTHREAD = 8
  };

  CLuaEngine();
  virtual ~CLuaEngine();

  bool loadString(const std::string& str);
  bool loadFile(const std::string& path);
  bool call(int args, int results);

  static bool luaPushAny(lua_State* luaState, CIEC_ANY* value);
  static bool luaGetAny(lua_State* luaState, CIEC_ANY* value, int index);
  static bool luaPushArray(lua_State *luaState, CIEC_ARRAY* array);
  static bool luaGetArray(lua_State *luaState, CIEC_ARRAY* array, int index);

  void store(const void* ref) {
    lua_rawsetp(luaState, LUA_REGISTRYINDEX, ref);
  }

  bool load(const void* ref) {
    if (lua_rawgetp(luaState, LUA_REGISTRYINDEX, ref) == LUA_TNIL) {
      lua_pop(luaState, 1);
      return false;
    }
    return true;
  }

  template<class T>
  void registerType() {
    // create new metatable if it doesn't exist yet (and fill if created)
    if (luaL_newmetatable(luaState, T::LUA_NAME)) {
#if LUA_VERSION_NUM > 501
      luaL_setfuncs(luaState, T::LUA_FUNCS, 0);
#else
      luaL_register(luaState, NULL, T::LUA_FUNCS);
#endif
      lua_pop(luaState, 1);
    }
  }

  template<class T>
  static void luaPushObject(lua_State* luaState, T* value) {
    T** userdata = static_cast<T**>(lua_newuserdata(luaState, sizeof(T*)));
    *userdata = value;
#if LUA_VERSION_NUM > 501
    luaL_setmetatable(luaState, T::LUA_NAME);
#else
    luaL_getmetatable(luaState, T::LUA_NAME);
    lua_setmetatable(luaState, -2);
#endif
  }

  template<class T>
  void pushObject(T* value) {
    luaPushObject<T>(luaState, value);
  }

  template<class T>
  static T* luaGetObject(lua_State* luaState, int index) {
    T** userdata = static_cast<T**>(luaL_checkudata(luaState, index, T::LUA_NAME));
    if (userdata == NULL) {
      luaL_argerror(luaState, index, T::LUA_NAME);
    }
    return *userdata;
  }

  template<class T>
  T* getObject(int index) {
    return luaGetObject<T>(luaState, index);
  }

  void pushBoolean(bool value) {
    lua_pushboolean(luaState, value);
  }

  bool getBoolean(int index) {
    return static_cast<bool>(lua_toboolean(luaState, index));
  }

  template<class T>
  void pushInteger(T value) {
    lua_pushinteger(luaState, static_cast<lua_Integer>(value));
  }

  template<class T>
  T getInteger(int index) {
    return static_cast<T>(lua_tointeger(luaState, index));
  }

  template<class T>
  void pushNumber(T value) {
    lua_pushnumber(luaState, static_cast<lua_Number>(value));
  }

  template<class T>
  T getNumber(int index) {
    return static_cast<T>(lua_tonumber(luaState, index));
  }

  void pushString(const char* value) {
    lua_pushstring(luaState, value);
  }

  const char* getString(int index) {
    return lua_tostring(luaState, index);
  }

  void pushStringId(CStringDictionary::TStringId value) {
    pushString(CStringDictionary::getInstance().get(value));
  }

  CStringDictionary::TStringId getStringId(int index) {
    return CStringDictionary::getInstance().insert(lua_tostring(luaState, index));
  }

  template<class T, T (CLuaEngine::*F)(int)>
  T* getArray(int index, size_t& length) {
    if (!lua_istable(luaState, index)) {
      return NULL;
    }
#if LUA_VERSION_NUM > 501
    size_t len = lua_rawlen(luaState, index);
#else
    size_t len = lua_objlen(luaState, index);
#endif
    if (length == SIZE_MAX) {
      length = len;
    } else if (len != length) {
      return NULL;
    }
    T* array = new T[len];
    for (size_t i = len; i > 0; i--) {
      lua_rawgeti(luaState, index, (int) i);
      T value = (this->*F)(-1);
      array[i - 1] = value;
      lua_pop(luaState, 1);
    }
    return array;
  }

  template<class T, bool (*F)(T&, CLuaEngine*, int)>
  T* getCustomArray(int index, size_t& length) {
    if (!lua_istable(luaState, index)) {
      return NULL;
    }
#if LUA_VERSION_NUM > 501
    size_t len = lua_rawlen(luaState, index);
#else
    size_t len = lua_objlen(luaState, index);
#endif
    if (length == SIZE_MAX) {
      length = len;
    } else if (len != length) {
      return NULL;
    }
    T* array = new T[len];
    for (size_t i = len; i > 0; i--) {
      lua_rawgeti(luaState, index, (int) i);
      if (!F(array[i - 1], this, -1)) {
        delete[] array;
        return NULL;
      }
      lua_pop(luaState, 1);
    }
    return array;
  }

  bool pushField(int index, const char* name, int expectedType) {
    lua_getfield(luaState, index, name);
    if (expectedType != LUA_TNONE && lua_type(luaState, -1) != expectedType) {
      lua_pop(luaState, 1);
      return false;
    }
    return true;
  }

  template<class T, T (CLuaEngine::*F)(int)>
  T getField(int index, const char* name) {
    lua_getfield(luaState, index, name);
    T value = (this->*F)(-1);
    lua_pop(luaState, 1);
    return value;
  }

  template<class T, T (CLuaEngine::*F)(int)>
  T* getArrayField(int index, const char* name, size_t& length) {
    lua_getfield(luaState, index, name);
    T* value = getArray<T, F>(-1, length);
    lua_pop(luaState, 1);
    return value;
  }

  template<class T, bool (*F)(T&, CLuaEngine*, int)>
  T* getCustomArrayField(int index, const char* name, size_t& length) {
    lua_getfield(luaState, index, name);
    T* value = getCustomArray<T, F>(-1, length);
    lua_pop(luaState, 1);
    return value;
  }

  bool pushAny(CIEC_ANY* value) {
    return luaPushAny(luaState, value);
  }

  bool getAny(CIEC_ANY *value, int index) {
    return luaGetAny(luaState, value, index);
  }

  void pop() {
    lua_pop(luaState, 1);
  }

  CLuaType type(int index) {
    return static_cast<CLuaType>(lua_type(luaState, index));
  }

};

#endif /* SRC_CORE_LUAENGINE_H_ */
