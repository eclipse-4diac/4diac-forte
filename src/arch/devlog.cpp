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
# include "core/util/criticalregion.h"
# include "forte_architecture_time.h"
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

#include <iostream>

#ifdef FORTE_STACKTRACE
#ifdef FORTE_STACKTRACE_BOOST
#include <boost/stacktrace.hpp>
#endif // FORTE_STACKTRACE_BOOST

#ifdef FORTE_STACKTRACE_CXX23
#include <stacktrace>
#endif // FORTE_STACKTRACE_CXX23
#endif // FORTE_STACKTRACE

std::string getRealtimeString();

static const char* scLogLevel[] = { "INFO", "WARNING", "ERROR", "DEBUG", "TRACE" };

//this define allows to provide an own log handler (see LMS for an example of this)
# ifndef FORTE_EXTERNAL_LOG_HANDLER

/*! \brief print the given log message with the error level and a time stamp
 *
 * @param paLevel the message's log level
 * @param paMessage the message to log
 */
void printLogMessage(E_MsgLevel paLevel, const char *paMessage);

static const int scMsgBufSize = FORTE_LOGGER_BUFFER_SIZE;
static char sMsgBuf[scMsgBufSize]; //!<Buffer for the messages created by the variable addMsg function

static CSyncObject sMessageLock;

#ifdef FORTE_STACKTRACE_BOOST
static void printStacktrace() {
  std::cerr << boost::stacktrace::stacktrace() << std::endl;
}
#endif // FORTE_STACKTRACE_BOOST

#ifdef FORTE_STACKTRACE_CXX23
static void printStacktrace() {
  std::cerr << std::stacktrace::current() << std::endl;
}
#endif // FORTE_STACKTRACE_CXX23

void logMessage(E_MsgLevel paLevel, const char *paMessage, ...) {
  CCriticalRegion crticalRegion(sMessageLock);
  va_list pstArgPtr;

  va_start(pstArgPtr, paMessage);
  forte_vsnprintf(sMsgBuf, scMsgBufSize, paMessage, pstArgPtr);
  va_end(pstArgPtr);

  printLogMessage(paLevel, sMsgBuf);

#ifdef FORTE_STACKTRACE
  if(paLevel == E_MsgLevel::Error) {
    printStacktrace();
  }
#endif // FORTE_STACKTRACE
}

void printLogMessage(E_MsgLevel paLevel, const char *paMessage) {
  (paLevel == E_MsgLevel::Error ? std::cerr : std::cout) << scLogLevel[static_cast<int>(paLevel)] << ": " << getRealtimeString() << ": " << paMessage;
  #ifdef WIN32
  (paLevel == E_MsgLevel::Error ? std::cerr : std::cout) << std::flush;
  #endif
}

# endif  /* FORTE_EXTERNAL_LOG_HANDLER */

#endif  /* NOLOG */
