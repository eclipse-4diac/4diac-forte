/************************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Milan Vathoopan - initial API and implementation and/or initial documentation
 ************************************************************************************/

#include "../forte_architecture.h"
#include "forteinit.h"
bool CForteArchitecture::mInitialized = false;

bool CForteArchitecture::initialize() {
  if(!mInitialized) {
    initForte();
    mInitialized = true;
  }
  return true;
}

void CForteArchitecture::deinitialize() {
  if(mInitialized) {
    mInitialized = false;
  }
}

