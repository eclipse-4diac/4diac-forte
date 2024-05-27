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
#include <device.h>

#include <forteinit.h>

CDevice *gDev = nullptr;

extern "C"
void startupFORTE(){
  CForteArchitecture::initialize();
  initForte();
  gDev = CDevice::createDev("");
  gDev->startDevice();
}

extern "C"
void shutdownFORTE(){
  if(0 != gDev){
    gDev->changeFBExecutionState(EMGMCommandType::Kill);
    gDev->awaitShutdown();
    delete gDev;
    gDev = 0;
  }
}

