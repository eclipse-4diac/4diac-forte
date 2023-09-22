/*******************************************************************************
 * Copyright (c) 2016, 2022 Johannes Messmer (admin@jomess.com), fortiss GmbH,
 *                          Jonathan Lainer
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *   Jonathan Lainer - Add method for deregistering Handles by ID.
 *******************************************************************************/

#include "io_mapper.h"
#include "io_handle.h"
#include "io_observer.h"
#include <devlog.h>
#include "criticalregion.h"

using namespace forte::core::io;

DEFINE_SINGLETON(IOMapper)

IOMapper::IOMapper() = default;

IOMapper::~IOMapper() = default;

bool IOMapper::registerHandle(std::string const &paId, IOHandle* paHandle) {
  CCriticalRegion criticalRegion(mSyncMutex);

  // Check for duplicates
  if(mHandles.find(paId) != mHandles.end()) {
    DEVLOG_WARNING("[IOMapper] Duplicated handle entry '%s'\n", paId.c_str());
    return false;
  }

  // Insert into handles list
  mHandles.insert(std::make_pair(paId, paHandle));

  DEVLOG_DEBUG("[IOMapper] Register handle %s\n", paId.c_str());

  // Check for existing observer
  if(mObservers.find(paId) != mObservers.end()) {
    paHandle->onObserver(mObservers[paId]);
    mObservers[paId]->onHandle(paHandle);

    DEVLOG_INFO("[IOMapper] Connected %s\n", paId.c_str());
  }

  return true;
}

void IOMapper::deregisterHandle(IOHandle* paHandle) {
  CCriticalRegion criticalRegion(mSyncMutex);

  for(THandleMap::iterator it = mHandles.begin(); it != mHandles.end(); ++it) {
    if(it->second == paHandle) {
      if(mObservers.find(it->first) != mObservers.end()) {
        paHandle->dropObserver();
        mObservers[it->first]->dropHandle();
        DEVLOG_INFO("[IOMapper]  Disconnected %s (lost handle)\n", it->first.data());
      }

      DEVLOG_DEBUG("[IOMapper] Deregister handle %s\n", it->first.data());

      mHandles.erase(it);
      break;
    }
  }
}

void IOMapper::deregisterHandle(std::string const &paId) {
  CCriticalRegion criticalRegion(mSyncMutex);

  auto handleIt = mHandles.find(paId);
  if(handleIt != mHandles.end()){
    auto observerIt = mObservers.find(paId);
    if(observerIt != mObservers.end()){
      handleIt->second->dropObserver();
      observerIt->second->dropHandle();
      DEVLOG_INFO("[IOMapper] Disconnected %s (lost handle)\n", paId.c_str());
    }
    DEVLOG_DEBUG("[IOMapper] Deregister handle %s\n", paId.c_str());
    mHandles.erase(handleIt);
  }
}

bool IOMapper::registerObserver(std::string const &paId, IOObserver* paObserver) {
  CCriticalRegion criticalRegion(mSyncMutex);

  // Check for duplicates
  if(mObservers.find(paId) != mObservers.end()) {
    DEVLOG_WARNING("[IOMapper]  Duplicated observer entry '%s'\n", paId.c_str());
    return false;
  }

  // Insert into observer list
  mObservers.insert(std::make_pair(paId, paObserver));

  DEVLOG_DEBUG("[IOMapper] Register observer %s\n", paId.c_str());

  // Check for existing handle
  if(auto handleIt = mHandles.find(paId); handleIt != mHandles.end()) {
    auto handle = handleIt->second;
    handle->onObserver(paObserver);
    paObserver->onHandle(handle);
    DEVLOG_INFO("[IOMapper]  Connected %s\n", paId.c_str());
  }

  return true;
}

void IOMapper::deregisterObserver(IOObserver* paObserver) {
  CCriticalRegion criticalRegion(mSyncMutex);

  for(TObserverMap::iterator it = mObservers.begin(); it != mObservers.end(); ++it) {
    if(it->second == paObserver) {
      if(mHandles.find(it->first) != mHandles.end()) {
        mHandles[it->first]->dropObserver();
        paObserver->dropHandle();
        DEVLOG_INFO("[IOMapper]  Disconnected %s (lost observer)\n", it->first.data());
      }

      DEVLOG_DEBUG("[IOMapper] Deregister observer %s\n", it->first.data());

      mObservers.erase(it);
      break;
    }
  }
}
