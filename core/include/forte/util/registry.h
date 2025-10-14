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

#include "forte/stringid.h"

#include <unordered_map>

namespace forte::util {

  template<fixed_string Default, typename T>
  class Registry final {
    public:
      class Entry final {
        public:
          explicit Entry(const StringId paName, T paValue) : mValue(paValue) {
            registerEntry(paName, this);
          }

          T get() {
            return mValue;
          }

        private:
          T mValue;
      };

      static T get() {
        return get(getDefault());
      }

      static T get(StringId paName) {
        if (auto entry = entries().find(paName); entry != entries().end()) {
          return entry->second->get();
        }
        return nullptr;
      }

      static auto getNames() {
        return entries() | std::views::keys;
      }

      [[nodiscard]] static StringId getDefault() {
        return defaultName();
      }

      static bool setDefault(const StringId paName) {
        if (!entries().contains(paName)) {
          return false;
        }
        defaultName() = paName;
        return true;
      }

      Registry() = delete;

    private:
      static void registerEntry(StringId paName, Entry *paEntry) {
        entries()[paName] = paEntry;
      }

      static std::unordered_map<StringId, Entry *> &entries() {
        static std::unordered_map<StringId, Entry *> entries;
        return entries;
      }

      static StringId &defaultName() {
        static StringId defaultName;
        return defaultName;
      }
  };
} // namespace forte::util
