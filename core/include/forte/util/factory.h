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

#include <memory>
#include <unordered_map>
#include <concepts>
#include <ranges>

namespace forte::util::factory {

  template<typename Impl, typename T, typename... Args>
  concept Constructible = requires(Impl impl) {
    { Impl(std::declval<Args>()...) };
    requires std::derived_from<Impl, T>;
  };

  struct DynamicImpl {
      DynamicImpl() = delete;
  };

  template<typename FixedImpl, fixed_string, typename T, typename... Args>
    requires Constructible<FixedImpl, T, Args...> || std::same_as<FixedImpl, DynamicImpl>
  class Factory {
    public:
      class Entry {
        public:
          virtual ~Entry() = default;

          virtual std::unique_ptr<T> create(Args... paArgs) = 0;

        protected:
          explicit Entry(const StringId) {
          }
      };

      template<typename Impl>
        requires Constructible<Impl, T, Args...>
      class EntryImpl final : public Entry {
        public:
          explicit EntryImpl(const StringId paName) : Entry(paName) {
          }

          std::unique_ptr<T> create(Args... paArgs) override {
            return std::make_unique<Impl>(std::forward<Args>(paArgs)...);
          }
      };

      static std::unique_ptr<T> create(Args... paArgs) {
        return std::make_unique<FixedImpl>(std::forward<Args>(paArgs)...);
      }

      static auto getNames() {
        return std::views::empty<StringId>;
      }

      [[nodiscard]] static StringId getDefaultImpl() {
        return {};
      }

      static bool setDefaultImpl(const StringId) {
        return false;
      }

      Factory() = delete;

    private:
      static void registerEntry(StringId, Entry *) {
      }
  };

  template<fixed_string DefaultImpl, typename T, typename... Args>
  class Factory<DynamicImpl, DefaultImpl, T, Args...> {
    public:
      class Entry {
        public:
          virtual ~Entry() = default;

          virtual std::unique_ptr<T> create(Args... paArgs) = 0;

        protected:
          explicit Entry(const StringId paName) {
            registerEntry(paName, this);
          }
      };

      template<typename Impl>
        requires Constructible<Impl, T, Args...>
      class EntryImpl final : public Entry {
        public:
          explicit EntryImpl(const StringId paName) : Entry(paName) {
          }

          std::unique_ptr<T> create(Args... paArgs) override {
            return std::make_unique<Impl>(std::forward<Args>(paArgs)...);
          }
      };

      static std::unique_ptr<T> create(Args... paArgs) {
        return create(getDefaultImpl(), std::forward<Args>(paArgs)...);
      }

      static std::unique_ptr<T> create(StringId paName, Args... paArgs) {
        if (auto entry = entries().find(paName); entry != entries().end()) {
          return entry->second->create(std::forward<Args>(paArgs)...);
        }
        return nullptr;
      }

      static auto getNames() {
        return entries() | std::views::keys;
      }

      [[nodiscard]] static StringId getDefaultImpl() {
        return defaultImpl();
      }

      static bool setDefaultImpl(const StringId paName) {
        if (!entries().contains(paName)) {
          return false;
        }
        defaultImpl() = paName;
        return true;
      }

      Factory() = delete;

    private:
      static void registerEntry(StringId paName, Entry *paEntry) {
        entries()[paName] = paEntry;
      }

      static std::unordered_map<StringId, Entry *> &entries() {
        static std::unordered_map<StringId, Entry *> entries;
        return entries;
      }

      static StringId &defaultImpl() {
        static StringId defaultImpl = StringId::fixed<DefaultImpl>();
        return defaultImpl;
      }
  };
} // namespace forte::util::factory
