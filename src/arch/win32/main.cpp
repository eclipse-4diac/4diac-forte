/*******************************************************************************
 * Copyright (c) 2010 - 2012 ACIN, Profactor GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl, Ingo Hegny, Gerhard Ebenhofer - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <fortealloc.h>
#include <stdio.h>
#include <signal.h>
#include "../../stdfblib/ita/RMT_DEV.h"
#ifdef WIN32
#include <sockhand.h>
#endif
#ifdef FORTE_FBTESTS
#include  <fbtestsmanager.h>
#endif

//this keeps away a lot of rtti and exception handling stuff
extern "C" void
__cxa_pure_virtual(void) {
  //TODO maybe add some reporting here
  //Although we should never get here
  //if we are here something very very bad has happened e.g., stack overflow or other memory corruption

}

RMT_DEV *poDev = 0;

void endForte(int pa_nSig){
 (void)pa_nSig;
  if(0 != poDev){
    poDev->changeFBExecutionState(cg_nMGM_CMD_Kill);
  }
}

/*!\brief Creates the Device-Object
 * \param pa_acMGRID A string containing IP and Port like [IP]:[Port]
 */
void createDev(const char *pa_acMGRID) {

  signal(SIGINT, endForte);
  signal(SIGTERM, endForte);

  poDev = new RMT_DEV;

  #ifdef FORTE_FBTESTS
    CFBTestsManager::getInstance().runAllTests();
  #endif

  poDev->setMGR_ID(pa_acMGRID);
  poDev->startDevice();
  DEVLOG_INFO("FORTE is up and running\n");
  poDev->MGR.getResourceEventExecution()->joinEventChainExecutionThread();
  DEVLOG_INFO("FORTE finished\n");
  delete poDev;
}

/*!\brief Lists the help for FORTE
 *
 */
void listHelp() {
  printf("\nUsage of FORTE:\n");
  printf("   -h\t lists this help.\n");
  printf("\n");
  printf("   -c\t sets the destination for the connection.\n");
  printf("     \t Usage: forte -c <IP>:<Port>");
  printf("\n");
}

#ifdef FORTE_SHARED_LIB
extern "C" __declspec(dllexport) int __stdcall main(int argc, char *arg[]) {
#else
 int main(int argc, char *arg[]) {
#endif
  #ifdef WIN32
  // Windows Socket Startupcode
  WORD wVersionRequested;
  WSADATA wsaData;

  /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
  wVersionRequested = MAKEWORD(2, 2);

  WSAStartup(wVersionRequested, &wsaData);
  #endif

  if (argc <= 1) { //! Default Value (localhost:61499)
    createDev("localhost:61499");
  }
  else {
    if (strcmp("-c", arg[1]) == 0) { //! sets the destination for the connection
      createDev(arg[2]);
    }
    else { //! Lists the help for FORTE
      listHelp();
    }
  }

  #ifdef WIN32
  WSACleanup();
  #endif
  return 0;
}
