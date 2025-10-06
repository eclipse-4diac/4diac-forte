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

#include "forte/util/hook.h"

#include <open62541.h>

namespace forte {
  namespace util::hook {
    extern template class Registry<UA_StatusCode, UA_Server *>;
  }

  namespace com_infra::opc_ua {
    using OPC_UA_Nodesets = util::hook::Registry<UA_StatusCode, UA_Server *>;
  }
} // namespace forte
