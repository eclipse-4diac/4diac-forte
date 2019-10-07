/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "io_mapper.h"
#include "io_handle.h"
#include "io_observer.h"
#include <devlog.h>
#include "criticalregion.h"

using namespace forte::core::io;

DEFINE_SINGLETON(IOMapper)

IOMapper::IOMapper() {

}

IOMapper::~IOMapper() {

}

bool IOMapper::registerHandle(CIEC_WSTRING const &paId, IOHandle* paHandle) {
  CCriticalRegion criticalRegion(mSyncMutex);
  std::string idStr(paId.getValue());

  // Check for duplicates
  if(mHandles.find(idStr) != mHandles.end()) {
    DEVLOG_WARNING("[IOMapper] Duplicated handle entry '%s'\n", paId.getValue());
    return false;
  }

  // Insert into handles list
  mHandles.insert(std::make_pair(idStr, paHandle));

  DEVLOG_DEBUG("[IOMapper] Register handle %s\n", paId.getValue());

  // Check for existing observer
  if(mObservers.find(idStr) != mObservers.end()) {
    paHandle->onObserver(mObservers[idStr]);
    mObservers[idStr]->onHandle(paHandle);

    DEVLOG_INFO("[IOMapper] Connected %s\n", paId.getValue());
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

bool IOMapper::registerObserver(CIEC_WSTRING const &paId, IOObserver* paObserver) {
  CCriticalRegion criticalRegion(mSyncMutex);
  std::string idStr(paId.getValue());

  // Check for duplicates
  if(mObservers.find(idStr) != mObservers.end()) {
    DEVLOG_WARNING("[IOMapper]  Duplicated observer entry '%s'\n", paId.getValue());
    return false;
  }

  // Insert into observer list
  mObservers.insert(std::make_pair(idStr, paObserver));

  DEVLOG_DEBUG("[IOMapper] Register observer %s\n", paId.getValue());

  // Check for existing handle
  if(mHandles.find(idStr) != mHandles.end()) {
    mHandles[idStr]->onObserver(paObserver);
    paObserver->onHandle(mHandles[idStr]);

    DEVLOG_INFO("[IOMapper]  Connected %s\n", paId.getValue());
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
