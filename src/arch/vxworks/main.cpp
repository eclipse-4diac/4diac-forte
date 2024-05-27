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
#include <device.h>

/*!\brief Check if the correct endianess has been configured.
 *
 * If the right endianess is not set this function will end FORTE.
 */
void checkEndianess();

//this keeps away a lot of rtti and exception handling stuff
#ifndef __cpp_exceptions
extern "C" void __cxa_pure_virtual(void){
  //TODO maybe add some reporting here
  //Although we should never get here
  //if we are here something very very bad has happened e.g., stack overflow or other memory corruption

}
#endif

CDevice *poDev = 0;

void endForte(int paSig){
  (void) paSig;
  if(0 != poDev){
    poDev->changeFBExecutionState(EMGMCommandType::Kill);
  }
}

/*!\brief Creates the Device-Object
 * \param paMGRID A string containing IP and Port like [IP]:[Port]
 */
void createDev(const char *paMGRID){

  signal(SIGINT, endForte);
  signal(SIGTERM, endForte);
  signal(SIGHUP, endForte);

  poDev = CDevice::createDev(paMGRID);
  poDev->startDevice();
  DEVLOG_INFO("FORTE is up and running\n");
  poDev->awaitShutdown();
  DEVLOG_INFO("FORTE finished\n");
  delete poDev;
}

/*!\brief Lists the help for FORTE
 *
 */
void listHelp(){
  printf("\nUsage of FORTE:\n");
  printf("   -h\t lists this help.\n");
  printf("\n");
  printf("   -c\t sets the destination for the connection.\n");
  printf("     \t Usage: forte -c <IP>:<Port>");
  printf("\n");
}

int startForte(){

  checkEndianess();

  createDev("localhost:61499");
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

