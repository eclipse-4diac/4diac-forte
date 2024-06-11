/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <fortenew.h>
#include <stdio.h>
#include <signal.h>
#include "forteinstance.h"

/*!\brief Check if the correct endianess has been configured.
 *
 * If the right endianess is not set this function will end FORTE.
 */
void checkEndianess();

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

int startForte(){

  checkEndianess();

  signal(SIGINT, endForte);
  signal(SIGTERM, endForte);
  signal(SIGHUP, endForte);

  if(g4diacForteInstance.startupNewDevice("")) {
    DEVLOG_INFO("FORTE is up and running\n");
    g4diacForteInstance.awaitDeviceShutdown();
    DEVLOG_INFO("FORTE finished\n");
  }
  return 0;
}

void checkEndianess(){
  int i = 1;
  char *p = (char *) &i;
  if(p[0] == 1){
    //we are on a little endian platform
#ifdef FORTE_BIG_ENDIAN
    DEVLOG_ERROR("Wrong endianess configured! You are on a little endian platform and have configured big endian!\n");
    exit(-1);
#endif
  }
  else{
    //we are on a big endian platform
#ifdef FORTE_LITTLE_ENDIAN
    DEVLOG_ERROR("Wrong endianess configured! You are on a big endian platform and have configured little endian!\n");
    exit(-1);
#endif
  }
}

