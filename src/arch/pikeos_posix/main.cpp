/*******************************************************************************
 * Copyright (c) 2006 - 2018 ACIN, Profactor GmbH, AIT, fortiss GmbH, SYSGO AG
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer, Thomas Strasser, Rene Smodic
 *  Martin Melik Merkumians, Ingo Hegny, Filip Andren - initial API and implementation and/or initial documentation
 *  Agostino Mascitti - Adaption to PikeOS 4.2
 *******************************************************************************/
#include <fortenew.h>
#include <stdio.h>
#include <signal.h>
#include "../../stdfblib/ita/RMT_DEV.h"

#include "../utils/mainparam_utils.h"


#ifdef CONFIG_POWERLINK_USERSTACK
#include <EplWrapper.h>
#endif

/* CG: testing */
#include <sys/debug.h>

#ifdef POSIX_LWIP
#include <lwip/sockets.h>
#endif

/*!\brief Check if the correct endianess has been configured.
 *
 * If the right endianess is not set this function will end FORTE.
 */
void checkEndianess();

//this keeps away a lot of rtti and exception handling stuff
extern "C" void __cxa_pure_virtual(void){
  //TODO maybe add some reporting here
  //Although we should never get here
  //if we are here something very very bad has happened e.g., stack overflow or other memory corruption

}

RMT_DEV *poDev = 0;

void endForte(int pa_nSig){
  (void) pa_nSig;
  if(0 != poDev){
    poDev->changeFBExecutionState(cg_nMGM_CMD_Kill);
  }
}

/*!\brief Creates the Device-Object
 * \param pa_acMGRID A string containing IP and Port like [IP]:[Port]
 */
void createDev(const char *pa_acMGRID){

  signal(SIGINT, endForte);
  signal(SIGTERM, endForte);
  signal(SIGHUP, endForte);

#ifdef CONFIG_POWERLINK_USERSTACK
  CEplStackWrapper::eplMainInit();
#endif

  poDev = new RMT_DEV;

  poDev->setMGR_ID(pa_acMGRID);
  poDev->startDevice();
  DEVLOG_INFO("FORTE is up and running\n");
  poDev->MGR.joinResourceThread();
  DEVLOG_INFO("FORTE finished\n");
  delete poDev;
}

int main(int argc, char *arg[]){

  checkEndianess();
  DEVLOG_INFO("FORTE starting\n");

#ifdef POSIX_LWIP
  /* Initialize the lwIP stack. */
  if (_lwip_init() != 0) {
    DEVLOG_ERROR("Initialization of lwIP stack failed.\n");
    exit(-1);
  }
#endif

  //gdb_breakpoint();

  const char *pIpPort = parseCommandLineArguments(argc, arg);
  if((0 != strlen(pIpPort)) && (NULL != strchr(pIpPort, ':'))){
    createDev(pIpPort);
  }
  else{ //! Lists the help for FORTE
    listHelp();
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

