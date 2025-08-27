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

#include "core/timerha.h"
#include "core/util/factory.h"
#include "generated/timerhandlerfactory_config.h"

#include <string_view>

namespace forte::core {
  using TimerHandlerFactory =
      util::factory::Factory<FixedTimerHandlerImpl, DefaultTimerHandlerImpl, CTimerHandler, CDeviceExecution &>;
} // namespace forte::core
