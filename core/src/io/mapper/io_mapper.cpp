/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com), fortiss GmbH,
 *                    Jonathan Lainer, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral      - Cleaning of namespaces
 *   Jonathan Lainer  - Add method for deregistering Handles by ID.
 *   Malte Grave, Alois Zoitl - reworked handle and oberserver handling reducing
 *                      memory overhead and allowing for to have several read
 *                      only observers
 *******************************************************************************/

#include "forte/io/mapper/io_mapper.h"
#include <algorithm>
#include <string_view>
#include "forte/io/mapper/io_handle.h"
#include "forte/io/mapper/io_observer.h"
#include "forte/util/devlog.h"
#include "forte/util/criticalregion.h"

namespace forte::io {
  DEFINE_SINGLETON(IOMapper)

  IOMapper::IOMapper() = default;

  IOMapper::~IOMapper() = default;

  bool IOMapper::registerHandle(std::string_view paId, IOHandle &paHandle) {
    util::CCriticalRegion criticalRegion(mSyncMutex);

    if (paHandle.mMapping != nullptr) {
      DEVLOG_WARNING("[IOMapper] Handle is already registered\n");
      return false;
    }

    auto it = lowerBound(paId);
    if (isMatch(it, paId)) {
      IOMappingData &m = **it;
      if (m.mHandle != nullptr) {
        DEVLOG_WARNING("[IOMapper] Duplicated handle entry '%s'\n", m.mId.c_str());
        return false;
      }
      m.mHandle = &paHandle; // entry existed only because of observers
      paHandle.bind(&m);
      for (IOObserver *o : m.mObservers) {
        o->onHandle(paHandle);
      }
      for (IOObserver *observer : m.mObservers) {
        paHandle.onNewObserver(*observer);
      }
      return true;
    }

    auto mapping = std::make_unique<IOMappingData>(paId);
    mapping->mHandle = &paHandle;
    paHandle.bind(mapping.get());
    mMappings.insert(it, std::move(mapping));
    return true;
  }

  void IOMapper::deregisterHandle(IOHandle &paHandle) {
    util::CCriticalRegion criticalRegion(mSyncMutex);

    const IOMappingData *m = paHandle.mMapping;
    if (m == nullptr) {
      DEVLOG_WARNING("[IOMapper] Tried to deregister unbound handle\n");
      return;
    }

    auto it = lowerBound(m->mId);
    if (!isMatch(it, m->mId) || (*it)->mHandle != &paHandle) {
      DEVLOG_WARNING("[IOMapper] Tried to deregister wrong handle '%s'\n", m->mId.c_str());
      return;
    }

    (*it)->mHandle = nullptr;
    paHandle.bind(nullptr);
    for (IOObserver *o : (*it)->mObservers) {
      o->dropHandle();
    }
    purgeIfUnused(it); // `m` may be dangling after this call
  }

  bool IOMapper::registerObserver(std::string_view paId, IOObserver &paObserver) {
    util::CCriticalRegion criticalRegion(mSyncMutex);

    auto it = lowerBound(paId);
    if (isMatch(it, paId)) {
      IOMappingData &m = **it;
      const auto &obs = m.mObservers;

      if (std::find(obs.begin(), obs.end(), &paObserver) != obs.end()) {
        DEVLOG_WARNING("[IOMapper]  Duplicated observer entry '%s'\n", m.mId.c_str());
        return false;
      }
      if (paObserver.getDirection() == Out) {
        // check if there is allready an output observer registered
        if (std::any_of(obs.begin(), obs.end(), [](const IOObserver *o) { return o->getDirection() == Out; })) {
          DEVLOG_WARNING("[IOMapper] A second output observer tried to register on '%s'\n", m.mId.c_str());
          return false;
        }
      }
      m.mObservers.push_back(&paObserver);
      if (m.mHandle != nullptr) {
        paObserver.onHandle(*m.mHandle);
        m.mHandle->onNewObserver(paObserver);
        DEVLOG_INFO("[IOMapper]  Connected %s\n", m.mId.c_str());
      }
      return true;
    }

    auto mapping = std::make_unique<IOMappingData>(paId);
    mapping->mObservers.push_back(&paObserver);
    mMappings.insert(it, std::move(mapping));
    return true;
  }

  void IOMapper::deregisterObserver(std::string_view paId, IOObserver &paObserver) {
    util::CCriticalRegion criticalRegion(mSyncMutex);

    auto it = lowerBound(paId);
    if (!isMatch(it, paId)) {
      return;
    }
    auto &obs = (*it)->mObservers;
    auto pos = std::find(obs.begin(), obs.end(), &paObserver);
    if (pos == obs.end()) {
      return;
    }
    paObserver.dropHandle();
    obs.erase(pos);
    purgeIfUnused(it);
  }

  IOMapper::MappingDataStorage::const_iterator IOMapper::lowerBound(std::string_view paId) const {
    return std::lower_bound(mMappings.begin(), mMappings.end(), paId,
                            [](const std::unique_ptr<IOMappingData> &paM, std::string_view paKey) {
                              return std::string_view(paM->mId) < paKey;
                            });
  }

  bool IOMapper::isMatch(MappingDataStorage::const_iterator paIt, std::string_view paId) const {
    return paIt != mMappings.end() && (*paIt)->mId == paId;
  }

  void IOMapper::purgeIfUnused(MappingDataStorage::const_iterator paIt) {
    const IOMappingData &m = **paIt;
    if (m.mHandle == nullptr && m.mObservers.empty()) {
      mMappings.erase(paIt);
    }
  }

} // namespace forte::io
