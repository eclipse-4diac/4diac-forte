/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "io_mapper.h"
#include "io_handle.h"
#include "io_observer.h"
#include <devlog.h>
#include "criticalregion.h"

using namespace forte::core::IO;

DEFINE_SINGLETON(IOMapper)

IOMapper::IOMapper() {

}

IOMapper::~IOMapper() {

}

bool IOMapper::registerHandle(CIEC_WSTRING const &id, IOHandle* handle) {
  CCriticalRegion criticalRegion(syncMutex);
  std::string idStr(id.getValue());

  // Check for duplicates
  if (handles.find(idStr) != handles.end()) {
    DEVLOG_WARNING("[IOMapper] Duplicated handle entry '%s'\n",
        id.getValue());
    return false;
  }

  // Insert into handles list
  handles.insert(std::make_pair(idStr, handle));

  DEVLOG_DEBUG("[IOMapper] Register handle %s\n", id.getValue());

  // Check for existing observer
  if (observers.find(idStr) != observers.end()) {
    handle->onObserver(observers[idStr]);
    observers[idStr]->onHandle(handle);

    DEVLOG_INFO("[IOMapper] Connected %s\n", id.getValue());
  }

  return true;
}

void IOMapper::deregisterHandle(IOHandle* handle) {
  CCriticalRegion criticalRegion(syncMutex);

  for (THandleMap::iterator it = handles.begin(); it != handles.end(); ++it)
    if (it->second == handle) {
      if (observers.find(it->first) != observers.end()) {
        handle->dropObserver();
        observers[it->first]->dropHandle();
        DEVLOG_INFO("[IOMapper]  Disconnected %s (lost handle)\n",
            it->first.data());
      }

      DEVLOG_DEBUG("[IOMapper] Deregister handle %s\n", it->first.data());

      handles.erase(it);
      break;
    }

}

bool IOMapper::registerObserver(CIEC_WSTRING const &id, IOObserver* observer) {
  CCriticalRegion criticalRegion(syncMutex);
  std::string idStr(id.getValue());

  // Check for duplicates
  if (observers.find(idStr) != observers.end()) {
    DEVLOG_WARNING("[IOMapper]  Duplicated observer entry '%s'\n",
        id.getValue());
    return false;
  }

  // Insert into observer list
  observers.insert(std::make_pair(idStr, observer));

  DEVLOG_DEBUG("[IOMapper] Register observer %s\n", id.getValue());

  // Check for existing handle
  if (handles.find(idStr) != handles.end()) {
    handles[idStr]->onObserver(observer);
    observer->onHandle(handles[idStr]);

    DEVLOG_INFO("[IOMapper]  Connected %s\n", id.getValue());
  }

  return true;
}

void IOMapper::deregisterObserver(IOObserver* observer) {
  CCriticalRegion criticalRegion(syncMutex);

  for (TObserverMap::iterator it = observers.begin(); it != observers.end();
      ++it)
    if (it->second == observer) {
      if (handles.find(it->first) != handles.end()) {
        handles[it->first]->dropObserver();
        observer->dropHandle();
        DEVLOG_INFO("[IOMapper]  Disconnected %s (lost observer)\n",
            it->first.data());
      }

      DEVLOG_DEBUG("[IOMapper] Deregister observer %s\n", it->first.data());

      observers.erase(it);
      break;
    }
}
