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

#include <memory>
#include <span>
#include <unordered_map>

#include "channel.h"

#include "core/stringdict.h"

namespace forte::com {

  template<typename T>
  class ComChannelEntry;

  template<typename T>
  class ComFactory final {
      friend ComChannelEntry<T>;

    public:
      static std::unique_ptr<ComChannel<T>> createChannel(CStringDictionary::TStringId paChannel,
                                                          ComObserver<T> &paObserver) {
        if (auto entry = getEntries().find(paChannel); entry != getEntries().end()) {
          return entry->second->createChannel(paObserver);
        }
        return nullptr;
      }

      ComFactory() = delete;

    private:
      static void registerChannel(CStringDictionary::TStringId paName, ComChannelEntry<T> *paEntry) {
        getEntries()[paName] = paEntry;
      }

      static std::unordered_map<CStringDictionary::TStringId, ComChannelEntry<T> *> &getEntries() {
        static std::unordered_map<CStringDictionary::TStringId, ComChannelEntry<T> *> entries;
        return entries;
      }
  };

  template<typename T>
  class ComChannelEntry {
      friend ComFactory<T>;

    public:
      virtual ~ComChannelEntry() = default;

    protected:
      explicit ComChannelEntry(CStringDictionary::TStringId paName) {
        ComFactory<T>::registerChannel(paName, this);
      }

    private:
      virtual std::unique_ptr<ComChannel<T>> createChannel(ComObserver<T> &paObserver) = 0;
  };

  template<typename T, ComChannelImpl<T> U>
  class ComChannelEntryImpl final : public ComChannelEntry<T> {
    public:
      explicit ComChannelEntryImpl(CStringDictionary::TStringId paName) : ComChannelEntry<T>(paName) {
      }

      std::unique_ptr<ComChannel<T>> createChannel(ComObserver<T> &paObserver) override {
        return std::make_unique<U>(paObserver);
      }
  };

} // namespace forte::com
