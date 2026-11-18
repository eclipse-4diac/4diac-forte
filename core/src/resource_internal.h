/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *    Pedro Ricardo C. Souza - Lua states of the Lua types
 *******************************************************************************/

#pragma once

#include "monitoring.h"

#ifdef FORTE_DYNAMIC_TYPE_LOAD
#include <memory>

#include "lua/luaengine.h"
#endif

namespace forte {
  class ResourceInternal {
    public:
      explicit ResourceInternal(CResource &paResource) : mMonitoringHandler(paResource) {
      }

      CMonitoringHandler &getMonitoringHandler() {
        return mMonitoringHandler;
      }

#ifdef FORTE_DYNAMIC_TYPE_LOAD
      /*!\brief The Lua state of this resource's Lua FB instances, created on first use
       *
       * Instances keep a reference, so the state outlives them however the resource is torn down. A state that
       * could not be set up, e.g. for lack of memory, is set up again on the next call.
       * Only called from the management thread, which creates the instances.
       */
      const std::shared_ptr<lua::CLuaEngine> &getLuaEngine() {
#ifdef FORTE_LUA_SHARED_STATE
        return getSharedLuaEngine();
#else
        if (!mLuaEngine || !mLuaEngine->isValid()) {
          mLuaEngine.reset(); // a failed state goes before a new one needs memory
          mLuaEngine = std::make_shared<lua::CLuaEngine>();
        }
        return mLuaEngine;
#endif
      }

      /*!\brief The Lua state that reads the modules of new Lua types, created on first use
       *
       * Types are created on the device from the management thread, so only the device's is used. It is kept
       * between types instead of setting up a state for each, and set up again if that failed, e.g. for lack
       * of memory.
       */
      lua::CLuaEngine &getLuaDefinitionEngine() {
#ifdef FORTE_LUA_SHARED_STATE
        return *getSharedLuaEngine();
#else
        if (!mLuaDefinitionEngine || !mLuaDefinitionEngine->isValid()) {
          mLuaDefinitionEngine.reset();
          mLuaDefinitionEngine = std::make_unique<lua::CLuaEngine>(lua::CLuaEngine::scmDefinitionInstructionLimit);
        }
        return *mLuaDefinitionEngine;
#endif
      }
#endif

    private:
      CMonitoringHandler mMonitoringHandler;

#ifdef FORTE_LUA_SHARED_STATE
      /*!\brief The one Lua state of the process: type definitions and the instances of every resource
       *
       * Saves a state per resource and the definition state on small targets; the resources then take turns
       * running Lua code. Set up again after a failed set up like the others.
       */
      static const std::shared_ptr<lua::CLuaEngine> &getSharedLuaEngine() {
        static std::shared_ptr<lua::CLuaEngine> engine;
        if (!engine || !engine->isValid()) {
          engine.reset();
          engine = std::make_shared<lua::CLuaEngine>();
        }
        return engine;
      }
#elif defined(FORTE_DYNAMIC_TYPE_LOAD)
      std::shared_ptr<lua::CLuaEngine> mLuaEngine;
      std::unique_ptr<lua::CLuaEngine> mLuaDefinitionEngine;
#endif
  };
} // namespace forte
