/*******************************************************************************
 * Copyright (c) 2015, 2026 fortiss GmbH, Johannes Kepler University Linz, Aimirim STI
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst - initial API and implementation and/or initial documentation
 *   Alois Zoitl  - upgraded to new FB memory layout
 *   Pedro Ricardo C. Souza - sandboxed modules, IEC value mapping for ARRAY and STRUCT
 *******************************************************************************/

#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "forte/arch/forte_sync.h"

struct lua_State;

namespace forte {
  class CIEC_ANY;
}

namespace forte::lua {

  /*!\brief One Lua 5.1 state running the Lua modules of run-time types
   *
   * Every module runs in its own environment table holding a safe subset of the base library, copies of the
   * string, table, math and bit libraries, and a require that only knows STfunc. There is no io, os, debug or
   * package access, and only source text is accepted (no precompiled byte code).
   *
   * Calls into Lua must hold getLock(), as instances of one resource are created from the management thread and
   * executed from the resource's event chain thread.
   *
   * Lua errors unwind through C++ frames with longjmp, which skips destructors. Functions called from Lua
   * (lua_CFunctions) must therefore not hold objects with destructors when they raise an error.
   */
  class CLuaEngine {
    public:
      //! The default VM instruction budget per call, 0 for none
      static const unsigned int scmDefaultInstructionLimit;
      //! The VM instruction budget for running a module while its type is created, so a module cannot hang CREATE
      static constexpr unsigned int scmDefinitionInstructionLimit = 10000000;

      explicit CLuaEngine(unsigned int paInstructionLimit = scmDefaultInstructionLimit);
      ~CLuaEngine();

      CLuaEngine(const CLuaEngine &) = delete;
      CLuaEngine &operator=(const CLuaEngine &) = delete;

      //! false if the state or the STfunc library could not be set up
      [[nodiscard]] bool isValid() const {
        return mValid;
      }

      [[nodiscard]] lua_State *getState() const {
        return mState;
      }

      arch::CSyncObject &getLock() {
        return mLock;
      }

      [[nodiscard]] unsigned int getInstructionLimit() const {
        return mInstructionLimit;
      }

      //! Changes the budget of the next calls, e.g. while a state shared with instances reads a type's module
      void setInstructionLimit(unsigned int paInstructionLimit) {
        mInstructionLimit = paInstructionLimit;
      }

      /*!\brief Runs a module chunk in a fresh sandbox
       *
       * On success the table returned by the chunk is pushed onto the stack.
       * @param paSource the Lua source text
       * @param paChunkName name used in error messages, e.g. the type name
       * @param paError receives the error message on failure
       */
      bool loadModule(std::string_view paSource, const char *paChunkName, std::string &paError);

      /*!\brief Pushes the module table of a type, running the module in this state on first use
       *
       * The table is kept while paType is alive, so all instances of a type in this state share it.
       * @param paType the type the module belongs to, identifies the cached table
       */
      bool pushTypeModule(const std::shared_ptr<const void> &paType,
                          std::string_view paSource,
                          const char *paChunkName,
                          std::string &paError);

      /*!\brief Calls the function below paNumArgs arguments on the stack in protected mode
       *
       * Applies the instruction budget. On failure nothing is left on the stack and paError holds the message
       * with a traceback.
       */
      bool call(int paNumArgs, int paNumResults, std::string &paError);

      //! Pops the top value and stores it in the registry, returns its reference
      int ref();
      void unref(int paRef);
      void pushRef(int paRef);

      //! Pushes a read only view of the STfunc library
      void pushSTfunc();

      //! Runs a full garbage collection cycle
      void collectGarbage();

      /*!\brief Pushes an IEC value as a Lua value
       *
       * ARRAYs become tables keyed by their IEC index (lower to upper bound) with the fields lo and up,
       * STRUCTs become tables keyed by member name, CHAR and WCHAR one character strings, the time types
       * integers in nanoseconds.
       */
      static void pushValue(lua_State *paState, const CIEC_ANY &paValue);

      /*!\brief Stores the Lua value at paIndex into an IEC value
       *
       * Integers wrap to the width of the IEC type, as in a C cast. A Lua value of the wrong kind is rejected.
       * A missing (nil) array element or struct member leaves the IEC element unchanged.
       * @return nullptr on success, otherwise a static description of the error (nothing raised)
       */
      static const char *getValue(lua_State *paState, int paIndex, CIEC_ANY &paValue);

    private:
      struct STypeModule {
          std::weak_ptr<const void> mType;
          int mRef;
      };

      bool setUp();
      void releaseExpiredTypeModules();
      void armBudget();
      void disarmBudget();

      static int panic(lua_State *paState);
      static int traceback(lua_State *paState);

      lua_State *mState;
      arch::CSyncObject mLock;
      unsigned int mInstructionLimit;
      int mSandboxFactoryRef;
      int mSTfuncRef;
      bool mValid;
      std::map<const void *, STypeModule> mTypeModules;
  };

  //! The names STfunc defines, functions and then aliases, each compiled into a state on first use
  std::vector<std::string_view> getSTfuncNames();

} // namespace forte::lua
