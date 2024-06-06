/*******************************************************************************
 * Copyright (c) 2006, 2024 ACIN, Profactor GmbH, AIT, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer, Thomas Strasser, Rene Smodic,
 *  Martin Melik Merkumians, Ingo Hegny, Filip Andren - initial API and implementation and/or initial documentation
 *  Tarik Terzimehic - make OPC UA server port setable from the command line
 *******************************************************************************/
#include "forte_architecture.h"
#include "src/core/forteinstance.h"
#include "startuphook.h"
#include "utils/mainparam_utils.h"
#include "forteinit.h"

#include <signal.h>


/*!\brief Check if the correct endianess has been configured.
 *
 * If the right endianess is not set this function will end FORTE.
 */
void checkEndianess();

void hookSignals();

C4diacFORTEInstance g4diacForteInstance;

void endForte(int ){
  g4diacForteInstance.triggerDeviceShutdown();
}

int main(int argc, char *arg[]){

  checkEndianess();

  hookSignals();

  if(!CForteArchitecture::initialize()){
    return -1;
  }

  initForte();

  startupHook(argc, arg);

  const char *pIpPort = parseCommandLineArguments(argc, arg);
  if((0 == strlen(pIpPort)) || (nullptr == strchr(pIpPort, ':'))) {
    listHelp();
    return -1;
  }

  if(!g4diacForteInstance.startupNewDevice(pIpPort)) {
    return -1;
  }
  
  DEVLOG_INFO("FORTE is up and running\n");
  g4diacForteInstance.awaitDeviceShutdown();
  DEVLOG_INFO("FORTE finished\n");

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

void hookSignals() {
  signal(SIGINT, endForte);
  signal(SIGTERM, endForte);
  signal(SIGHUP, endForte);
}

