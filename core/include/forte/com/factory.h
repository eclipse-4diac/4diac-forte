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

#include "forte/com/buffer.h"
#include "forte/com/channel.h"

#include "forte/util/factory.h"

namespace forte {
  class CIEC_ANY_VARIANT;

  namespace com {
    template<typename T>
    class ComChannelFactory
        : public util::factory::Factory<util::factory::DynamicImpl, "", ComChannel<T>, ComObserver<T> &> {};

    extern template class ComChannelFactory<ComBuffer>;
    extern template class ComChannelFactory<std::span<CIEC_ANY_VARIANT>>;
  } // namespace com
} // namespace forte
