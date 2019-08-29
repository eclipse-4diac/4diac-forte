/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Rene Smodic, Alois Zoitl, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "devlog.h"

#ifndef NOLOG

# include <forte_config.h>
# include "timerha.h"
# include "forte_printer.h"
# include "../core/utils/criticalregion.h"
# include "forte_architecture_time.h"
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

static const char* scLogLevel[] = { "INFO", "WARNING", "ERROR", "DEBUG", "TRACE" };

//this define allows to provide an own log handler (see LMS for an example of this)
# ifndef FORTE_EXTERNAL_LOG_HANDLER

/*! \brief print the given log message with the error level and a time stamp
 *
 * @param pa_ecLevel the message's log level
 * @param pa_acMessage the message to log
 */
void printLogMessage(E_MsgLevel paLevel, const char *paMessage);

static const int scMsgBufSize = FORTE_LOGGER_BUFFER_SIZE;
static char sMsgBuf[scMsgBufSize]; //!<Buffer for the messages created by the variable addMsg function

static CSyncObject sMessageLock;

void logMessage(E_MsgLevel paLevel, const char *paMessage, ...) {
  CCriticalRegion crticalRegion(sMessageLock);
  va_list pstArgPtr;

  va_start(pstArgPtr, paMessage);
  forte_vsnprintf(sMsgBuf, scMsgBufSize, paMessage, pstArgPtr);
  va_end(pstArgPtr);

  printLogMessage(paLevel, sMsgBuf);
}

void printLogMessage(E_MsgLevel paLevel, const char *paMessage) {
  fprintf(stderr, "%s: T#%" PRIuFAST64 ": %s", scLogLevel[paLevel], getNanoSecondsMonotonic(), paMessage);
}

# endif  /* FORTE_EXTERNAL_LOG_HANDLER */

#endif  /* NOLOG */
