/************************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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

