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

#include <vector>

namespace forte::core::util::hook {

  template<typename... Args>
  class Registry final {
    public:
      class Entry {
        public:
          virtual ~Entry() = default;

          virtual void operator()(Args... paArgs) = 0;

        protected:
          Entry() {
            registerEntry(this);
          }
      };

      template<void (*Impl)(Args...)>
      class EntryImpl final : public Entry {
        public:
          EntryImpl() = default;

          void operator()(Args... paArgs) override {
            Impl(std::forward<Args>(paArgs)...);
          }
      };

      static void invoke(Args... paArgs) {
        for (auto &entry : entries) {
          (*entry)(std::forward<Args>(paArgs)...);
        }
      }

      Registry() = delete;

    private:
      static void registerEntry(Entry *paEntry) {
        entries.push_back(paEntry);
      }

      constinit static std::vector<Entry *> entries;
  };

  template<typename... Args>
  constinit std::vector<typename Registry<Args...>::Entry *> Registry<Args...>::entries;
} // namespace forte::core::util::hook
