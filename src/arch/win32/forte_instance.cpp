/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "forte_instance.h"
#include "fortenew.h"
#include "forte_architecture.h"
#include "forteinstance.h"

#include <forteinit.h>

C4diacFORTEInstance g4diacForteLibInstance;

extern "C"
void startupFORTE(){
  CForteArchitecture::initialize();
  initForte();
  g4diacForteLibInstance.startupNewDevice("");
}

extern "C"
void shutdownFORTE(){
  g4diacForteLibInstance.triggerDeviceShutdown();
  g4diacForteLibInstance.awaitDeviceShutdown();
}

