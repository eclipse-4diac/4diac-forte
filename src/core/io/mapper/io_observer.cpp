/*******************************************************************************
 * Copyright (c) 2016 - 2022 Johannes Messmer (admin@jomess.com), fortiss GmbH, 
 *                           Jonathan Lainer
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *   Jonathan Lainer - Adding getter for mDirection.
 *******************************************************************************/

#include "io_observer.h"
#include <devlog.h>

using namespace forte::core::io;

IOObserver::IOObserver() :
    mHandle(nullptr), mType(CIEC_ANY::e_ANY), mDirection(IOMapper::UnknownDirection) {

}

IOObserver::~IOObserver() {
  IOMapper::getInstance().deregisterObserver(this);
}

void IOObserver::onHandle(IOHandle *paHandle) {
  this->mHandle = paHandle;
}

void IOObserver::dropHandle() {
  this->mHandle = nullptr;
}

IOMapper::Direction IOObserver::getDirection() {
  return this->mDirection;
}
