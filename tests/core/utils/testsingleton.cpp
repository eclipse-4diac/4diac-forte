/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "testsingleton.h"

DEFINE_SINGLETON(TestSingleton);

unsigned int TestSingleton::smInstanceCount = 0;

TestSingleton::TestSingleton(){
  smInstanceCount++;
  mSingletonID = smInstanceCount;
}

TestSingleton::~TestSingleton() = default;

