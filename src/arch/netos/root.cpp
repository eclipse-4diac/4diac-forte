/*******************************************************************************
 * Copyright (c) 2006 - 2011 Profactor GmbH, ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Rene Smodic, Alois Zoitl, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <threadx/tx_api.h>
#include "appconf_api.h"
#include "../../forte.h"
#include "../../stdfblib/ita/RMT_DEV.h"
#include <reg_def.h>
#include <fservapi.h>

/*
 * Set this to 1 to run the system POST tests during startup.
 */
const int APP_POST = 0;

/*
 * Set this to 1 to run the manufacturing burn in tests.
 */
int APP_BURN_IN_TEST = 0;

/*
 *  Function: void applicationTcpDown (void)
 *
 *  Description:
 *
 *      This routine will be called by the NET+OS root thread once every
 *      clock tick while it is waiting for the TCP/IP stack to come up.
 *
 *      This routone is called to print the application name and then
 *      to print the '.' every second.
 *
 *      This function will not be called once the stack has started.
 *
 *      This function uses the global C++ object PrintApplicationDown app_down.
 *
 *  Parameters:
 *
 *      none
 *
 *  Return Values:
 *
 *      none
 */
extern "C"
void applicationTcpDown(void) {
  static int ticksPassed = 0;
  ticksPassed++;
}


/*
 *  Function: void applicationStart (void)
 *
 *  Description:
 *
 *      This routine is responsible for starting the user application.  It should
 *      create any threads or other resources the application needs.
 *
 *      ThreadX, the NET+OS device drivers, and the TCP/IP stack will be running
 *      when this function is called.
 *
 *      This function uses global C++ object:
 *              PrintApplicationDown app_down.
 *
 *  Parameters:
 *
 *      none
 *
 *  Return Values:
 *
 *      none
 */

extern "C"
void applicationStart(void) {
  DEVLOG_INFO("\n\nStarting FTP-Server\n\n");
  naFtpDlInit(NA_FTP_DEF_USERS);

  DEVLOG_INFO("\n\nStarting forteMain()\n\n");

  // Starting  Runtime
  RMT_DEV dev;
  dev.startDevice();
  dev.MGR.joinResourceThread();
  tx_thread_suspend(tx_thread_identify());
}





