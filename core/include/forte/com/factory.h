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

#include "forte/com/channel.h"

#include "forte/util/factory.h"

namespace forte::com {

  template<typename T>
  using ComChannelFactory =
      core::util::factory::Factory<core::util::factory::DynamicImpl, "", ComChannel<T>, ComObserver<T> &>;
} // namespace forte::com
