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

#include "mapper.h"
#include <devlog.h>

namespace EmBrick {

DEFINE_SINGLETON(IOMapper)

IOMapper::IOMapper() {

}

IOMapper::~IOMapper() {

}

bool IOMapper::registerHandle(CIEC_WSTRING const &id, IOHandle* handle) {
  std::string idStr(id.getValue());

  syncMutex.lock();

  // Check for duplicates
  if (handles.find(idStr) != handles.end()) {
    DEVLOG_WARNING("emBrick[IOMapper]: Duplicated handle entry '%s'\n",
        id.getValue());
    syncMutex.unlock();
    return false;
  }

  // Insert into handles list
  handles.insert(std::make_pair(idStr, handle));

  // Check for existing observer
  if (observers.find(idStr) != observers.end()) {
    observers[idStr]->onHandle(handle);
    handle->onObserver(observers[idStr]);

    DEVLOG_INFO("emBrick[IOMapper]: Connected %s\n", id.getValue());
  }

  syncMutex.unlock();

  return true;
}

void IOMapper::deregisterHandle(IOHandle* handle) {
  syncMutex.lock();

  for (THandleMap::iterator it = handles.begin(); it != handles.end(); it++)
    if (it->second == handle) {
      if (observers.find(it->first) != observers.end()) {
        observers[it->first]->dropHandle();
        DEVLOG_INFO("emBrick[IOMapper]: Disconnected %s\n", it->first.data());
      }

      handles.erase(it);
      break;
    }

  syncMutex.unlock();
}

bool IOMapper::registerObserver(CIEC_WSTRING const &id, IOObserver* observer) {
  std::string idStr(id.getValue());

  syncMutex.lock();

  // Check for duplicates
  if (observers.find(idStr) != observers.end()) {
    DEVLOG_WARNING("emBrick[IOMapper]: Duplicated observer entry '%s'\n",
        id.getValue());
    syncMutex.unlock();
    return false;
  }

  // Insert into observer list
  observers.insert(std::make_pair(idStr, observer));

  // Check for existing handle
  if (handles.find(idStr) != handles.end()) {
    handles[idStr]->onObserver(observer);
    observer->onHandle(handles[idStr]);

    DEVLOG_INFO("emBrick[IOMapper]: Connected %s\n", id.getValue());
  }

  syncMutex.unlock();

  return true;
}

void IOMapper::deregisterObserver(IOObserver* observer) {
  syncMutex.lock();

  for (TObserverMap::iterator it = observers.begin(); it != observers.end();
      it++)
    if (it->second == observer) {
      if (handles.find(it->first) != handles.end()) {
        handles[it->first]->dropObserver();
        DEVLOG_INFO("emBrick[IOMapper]: Disconnected %s\n", it->first.data());
      }

      observers.erase(it);
      break;
    }

  syncMutex.unlock();
}

} /* namespace EmBrick */
