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

namespace IO {

DEFINE_SINGLETON(Mapper)

Mapper::Mapper() {

}

Mapper::~Mapper() {

}

bool Mapper::registerHandle(CIEC_WSTRING const &id, Handle* handle) {
  std::string idStr(id.getValue());

  syncMutex.lock();

  // Check for duplicates
  if (handles.find(idStr) != handles.end()) {
    DEVLOG_WARNING("[IO:Mapper] Duplicated handle entry '%s'\n",
        id.getValue());
    syncMutex.unlock();
    return false;
  }

  // Insert into handles list
  handles.insert(std::make_pair(idStr, handle));

  DEVLOG_DEBUG("[IO:Mapper] Register handle %s\n", id.getValue());

  // Check for existing observer
  if (observers.find(idStr) != observers.end()) {
    handle->onObserver(observers[idStr]);
    observers[idStr]->onHandle(handle);

    DEVLOG_INFO("[IO:Mapper] Connected %s\n", id.getValue());
  }

  syncMutex.unlock();

  return true;
}

void Mapper::deregisterHandle(Handle* handle) {
  syncMutex.lock();

  for (THandleMap::iterator it = handles.begin(); it != handles.end(); it++)
    if (it->second == handle) {
      if (observers.find(it->first) != observers.end()) {
        handle->dropObserver();
        observers[it->first]->dropHandle();
        DEVLOG_INFO("[IO:Mapper]  Disconnected %s (lost handle)\n",
            it->first.data());
      }

      DEVLOG_DEBUG("[IO:Mapper] Deregister handle %s\n", it->first.data());

      handles.erase(it);
      break;
    }

  syncMutex.unlock();
}

bool Mapper::registerObserver(CIEC_WSTRING const &id, Observer* observer) {
  std::string idStr(id.getValue());

  syncMutex.lock();

  // Check for duplicates
  if (observers.find(idStr) != observers.end()) {
    DEVLOG_WARNING("[IO:Mapper]  Duplicated observer entry '%s'\n",
        id.getValue());
    syncMutex.unlock();
    return false;
  }

  // Insert into observer list
  observers.insert(std::make_pair(idStr, observer));

  DEVLOG_DEBUG("[IO:Mapper] Register observer %s\n", id.getValue());

  // Check for existing handle
  if (handles.find(idStr) != handles.end()) {
    handles[idStr]->onObserver(observer);
    observer->onHandle(handles[idStr]);

    DEVLOG_INFO("[IO:Mapper]  Connected %s\n", id.getValue());
  }

  syncMutex.unlock();

  return true;
}

void Mapper::deregisterObserver(Observer* observer) {
  syncMutex.lock();

  for (TObserverMap::iterator it = observers.begin(); it != observers.end();
      it++)
    if (it->second == observer) {
      if (handles.find(it->first) != handles.end()) {
        handles[it->first]->dropObserver();
        observer->dropHandle();
        DEVLOG_INFO("[IO:Mapper]  Disconnected %s (lost observer)\n",
            it->first.data());
      }

      DEVLOG_DEBUG("[IO:Mapper] Deregister observer %s\n", it->first.data());

      observers.erase(it);
      break;
    }

  syncMutex.unlock();
}

} /* namespace IO */
