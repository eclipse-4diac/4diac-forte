/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Rene Smodic, Alois Zoitl, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "devlog.h"

#ifndef NOLOG

# include "timerha.h"
# include "forte_printer.h"
# include "../core/utils/criticalregion.h"
# include <cstdio>
# include <cstdlib>
# include <cstdarg>

# if __cplusplus < 201103L // < stdc11
#  ifndef VXWORKS //inttypes.h is not present for VXWORKS_KERNEL compilation type. PRIuFAST64 is defined in forte_config
#   ifndef __STDC_FORMAT_MACROS
#    define __STDC_FORMAT_MACROS
#   endif //__STDC_FORMAT_MACROS
#   include <inttypes.h>
#  endif //VXWORKS
# else //< stdc11
#  include <cinttypes>
# endif //< stdc11

static const char* cg_acLogLevel[] = {"INFO", "WARNING", "ERROR", "DEBUG", "TRACE" };

//this define allows to provide an own log handler (see LMS for an example of this)
#ifndef FORTE_EXTERNAL_LOG_HANDLER

/*! \brief print the given log message with the error level and a time stamp
 *
 * @param pa_ecLevel the message's log level
 * @param pa_acMessage the message to log
 */
void printLogMessage(E_MsgLevel pa_eLevel, const char *pa_acMessage);

static const int scm_nMsgBufSize = 300;
static char sm_acMsgBuf[scm_nMsgBufSize]; //!<Buffer for the messages created by the variable addMsg function

static CSyncObject  sgMessageLock;


void logMessage(E_MsgLevel pa_eLevel, const char *pa_acMessage, ...){
  CCriticalRegion crticalRegion(sgMessageLock);
  va_list pstArgPtr;

  va_start(pstArgPtr, pa_acMessage);
  forte_vsnprintf(sm_acMsgBuf, scm_nMsgBufSize, pa_acMessage, pstArgPtr);
  va_end(pstArgPtr);

  printLogMessage(pa_eLevel, sm_acMsgBuf);
}

void printLogMessage(E_MsgLevel pa_eLevel, const char *pa_acMessage){
  fprintf(stderr, "%s", cg_acLogLevel[pa_eLevel]);

  if (CTimerHandler::smFORTETimer != 0){
    fprintf(stderr, ": T#%" PRIuFAST64 ": ", CTimerHandler::smFORTETimer->getForteTime());
  }
  else{
    fprintf(stderr, "%s", ": T#notime: ");
  }
  fprintf(stderr, "%s", pa_acMessage);
}

#endif //NOLOG
#endif
