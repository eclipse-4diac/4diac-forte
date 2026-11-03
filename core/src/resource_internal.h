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
 *******************************************************************************/

#pragma once

#include "monitoring.h"

#ifdef FORTE_DYNAMIC_TYPE_LOAD
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
      CLuaEngine &getLuaEngine() {
        return luaEngine;
      }
#endif

    private:
      CMonitoringHandler mMonitoringHandler;

#ifdef FORTE_DYNAMIC_TYPE_LOAD
      CLuaEngine luaEngine; //!< The Lua engine for this container
#endif
  };
} // namespace forte
