/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Jose Cabral, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "forte_instance.h"
#include "fortenew.h"
#include "forte_architecture.h"
#include "../../stdfblib/ita/RMT_DEV.h"

#ifdef FORTE_STATIC_LIB
#include <forteinit.h>
#endif

RMT_DEV *gDev = 0;

extern "C"
void startupFORTE(){
  CForteArchitecture::initialize();
#ifdef FORTE_STATIC_LIB
  initForte();
#endif
  gDev = new RMT_DEV;
  gDev->startDevice();
}

extern "C"
void shutdownFORTE(){
  if(0 != gDev){
    gDev->changeFBExecutionState(cg_nMGM_CMD_Kill);
    gDev->MGR.joinResourceThread();
    delete gDev;
    gDev = 0;
  }
}

