/*******************************************************************************
 * Copyright (c) 2026 Martin Erich Jobst
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

#include "channeltestfixtures.h"

namespace forte::com::test {
  namespace {
    [[maybe_unused]] ComChannelFactory<std::span<CIEC_ANY_VARIANT>>::EntryImpl<TestChannel>
        testChannelEntry("test-channel"_STRID);
    [[maybe_unused]] ComChannelFactory<std::span<CIEC_ANY_VARIANT>>::EntryImpl<AlternateTestChannel>
        alternateChannelEntry("test-alternate-channel"_STRID);
    [[maybe_unused]] ComChannelFactory<ComBuffer>::EntryImpl<TestBufferChannel>
        testBufferChannelEntry("test-buffer-channel"_STRID);
  } // namespace
} // namespace forte::com::test
