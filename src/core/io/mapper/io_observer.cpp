/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "io_observer.h"
#include <devlog.h>

using namespace forte::core::IO;

IOObserver::IOObserver() :
    handle(NULL), type(CIEC_ANY::e_ANY), direction(IOMapper::UnknownDirection) {

}

IOObserver::~IOObserver() {
  IOMapper::getInstance().deregisterObserver(this);
}

void IOObserver::onHandle(IOHandle *handle) {
  this->handle = handle;
}

void IOObserver::dropHandle() {
  this->handle = NULL;
}

