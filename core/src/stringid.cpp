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

#include "forte/stringid.h"

#include <deque>
#include <string>
#include <unordered_set>

#include "forte/arch/forte_sync.h"
#include "forte/util/criticalregion.h"

namespace forte {
  namespace {
    arch::CSyncObject &internMutex() {
      static arch::CSyncObject internMutex;
      return internMutex;
    }

    std::unordered_set<std::string_view> &internSet() {
      static std::unordered_set<std::string_view> internSet;
      return internSet;
    }

    std::deque<std::string> &runtimeDeque() {
      static std::deque<std::string> runtimeDeque;
      return runtimeDeque;
    }
  } // namespace

  std::string_view StringId::intern(const std::string_view paString) {
    util::CCriticalRegion lock(internMutex());
    return *internSet().insert(paString).first;
  }

  StringId StringId::lookup(std::string_view paString) {
    util::CCriticalRegion lock(internMutex());
    const auto it = internSet().find(paString);
    if (it == internSet().end()) {
      return {};
    }
    return StringId(*it);
  }

  StringId StringId::insert(const std::string_view paString) {
    util::CCriticalRegion lock(internMutex());
    auto it = internSet().find(paString);
    if (it == internSet().end()) {
      it = internSet().insert(runtimeDeque().emplace_back(paString)).first;
    }
    return StringId(*it);
  }
} // namespace forte
