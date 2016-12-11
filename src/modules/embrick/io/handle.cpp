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

#include "handle.h"
#include "observer.h"
#include "mapper.h"

namespace EmBrick {

IOHandle::IOHandle() :
    observer(NULL), type(AnyType) {

}

IOHandle::~IOHandle() {
  IOMapper::getInstance().deregisterHandle(this);
}

void IOHandle::onObserver(IOObserver *observer) {
  this->observer = observer;
}

void IOHandle::dropObserver() {
  this->observer = NULL;
}

} /* namespace EmBrick */
