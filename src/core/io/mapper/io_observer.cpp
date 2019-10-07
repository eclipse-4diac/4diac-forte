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

#include "io_observer.h"
#include <devlog.h>

using namespace forte::core::io;

IOObserver::IOObserver() :
    mHandle(NULL), mType(CIEC_ANY::e_ANY), mDirection(IOMapper::UnknownDirection) {

}

IOObserver::~IOObserver() {
  IOMapper::getInstance().deregisterObserver(this);
}

void IOObserver::onHandle(IOHandle *paHandle) {
  this->mHandle = paHandle;
}

void IOObserver::dropHandle() {
  this->mHandle = NULL;
}

