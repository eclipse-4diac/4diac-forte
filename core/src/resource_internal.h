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
        if (!mLuaEngine || !mLuaEngine->isValid()) {
          mLuaEngine.reset(); // a failed state goes before a new one needs memory
          mLuaEngine = std::make_shared<lua::CLuaEngine>();
        }
        return mLuaEngine;
      }
#endif

    private:
      CMonitoringHandler mMonitoringHandler;

#ifdef FORTE_DYNAMIC_TYPE_LOAD
      std::shared_ptr<lua::CLuaEngine> mLuaEngine;
#endif
  };
} // namespace forte
