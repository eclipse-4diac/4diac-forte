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

namespace forte::util::hook {

  template<typename T, typename... Args>
  class Registry final {
    public:
      class Entry {
        public:
          virtual ~Entry() = default;

          virtual T operator()(Args... paArgs) = 0;

        protected:
          Entry() {
            registerEntry(this);
          }
      };

      template<T (*Impl)(Args...)>
      class EntryImpl final : public Entry {
        public:
          EntryImpl() = default;

          T operator()(Args... paArgs) override {
            return Impl(std::forward<Args>(paArgs)...);
          }
      };

      static T invoke(Args... paArgs) {
        if constexpr (std::is_same_v<T, void>) {
          for (auto &entry : entries) {
            (*entry)(std::forward<Args>(paArgs)...);
          }
          return;
        } else {
          T result{};
          for (auto &entry : entries) {
            result += (*entry)(std::forward<Args>(paArgs)...);
          }
          return result;
        }
      }

      Registry() = delete;

    private:
      static void registerEntry(Entry *paEntry) {
        entries.push_back(paEntry);
      }

      constinit static std::vector<Entry *> entries;
  };

  template<typename T, typename... Args>
  constinit std::vector<typename Registry<T, Args...>::Entry *> Registry<T, Args...>::entries;
} // namespace forte::util::hook
