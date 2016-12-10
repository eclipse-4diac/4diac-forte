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

#include "observer.h"
#include "mapper.h"
#include "handle.h"
#include <devlog.h>

namespace EmBrick {

IOObserver::IOObserver() :
    handle(NULL) {

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

} /* namespace EmBrick */
