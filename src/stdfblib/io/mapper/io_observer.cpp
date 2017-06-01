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

#include "io_observer.h"
#include <devlog.h>

namespace IO {

Observer::Observer() :
    handle(NULL), type(CIEC_ANY::e_ANY), direction(Mapper::UnknownDirection) {

}

Observer::~Observer() {
  Mapper::getInstance().deregisterObserver(this);
}

void Observer::onHandle(Handle *handle) {
  this->handle = handle;
}

void Observer::dropHandle() {
  this->handle = NULL;
}

} /* namespace IO */
