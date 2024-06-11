/*******************************************************************************
 * Copyright (c) 2010 - 2018 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Ingo Hegny, Gerhard Ebenhofer - initial API and implementation and/or initial documentation
 *  Alois Zoitl - cleaned up main, inserted new architecture initilasation api
 *******************************************************************************/
#include "../forte_architecture.h"
#include "../devlog.h"
#include "../startuphook.h"
#include "forteinstance.h"
#include "../utils/mainparam_utils.h"

#include <stdio.h>
#include <signal.h>

void hookSignals();

C4diacFORTEInstance g4diacForteInstance;

//this keeps away a lot of rtti and exception handling stuff
#ifndef __cpp_exceptions
extern "C" void __cxa_pure_virtual(void){
  //TODO maybe add some reporting here
  //Although we should never get here
  //if we are here something very very bad has happened e.g., stack overflow or other memory corruption

}
#endif

void endForte(int ){
  g4diacForteInstance.triggerDeviceShutdown();
}

int main(int argc, char *arg[]){

  if(CForteArchitecture::initialize()){

    startupHook(argc, arg);

    hookSignals();

    const char *pIpPort = parseCommandLineArguments(argc, arg);
    if((0 != strlen(pIpPort)) && (nullptr != strchr(pIpPort, ':'))){
      if(g4diacForteInstance.startupNewDevice(pIpPort)) {
        DEVLOG_INFO("FORTE is up and running\n");
        g4diacForteInstance.awaitDeviceShutdown();
        DEVLOG_INFO("FORTE finished\n");
      }
    }
    else{ //! Lists the help for FORTE
      listHelp();
    }

    CForteArchitecture::deinitialize();

  }
  else{
    DEVLOG_ERROR("Architecture could not be initialized\n");
  }
  return 0;
}

void hookSignals() {
  signal(SIGINT, endForte);
  signal(SIGTERM, endForte);
}
