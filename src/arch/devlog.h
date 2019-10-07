/*******************************************************************************
 * Copyright (c) 2005 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Rene Smodic, Thomas Strasser, Alois Zoitl, Ingo Hegny, Monika Wenger
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _DEVLOG_H_
#define _DEVLOG_H_

/*!\ingroup FORTE_HAL
 * \brief CDeviceLog is the entity that logs messages created by the FORTE Runtime system.
 * They can be printed to a console or archived somewhere (This is implementation dependent).
 */

enum E_MsgLevel {
  E_INFO,
  E_WARNING,
  E_ERROR,
  E_DEBUG,
  E_TRACE
};

// possible loglevels: NOLOG, LOGERROR, LOGWARNING, LOGINFO, LOGDEBUG
/* Meaning:
 * LOGERROR: log only error messages
 * LOGWARNING: log error and warning messages
 * LOGINFO: log error, warning, and info messages
 * LOGDEBUG: log all messages: error, warning, info, and debug
 * NOLOG: log no messages
 */

#if !(defined(NOLOG) || defined(LOGERROR) || defined(LOGWARNING) || defined(LOGINFO) || defined(LOGDEBUG))
# define LOGDEBUG  /* Set default loglevel */
#endif

#ifdef LOGDEBUG
# define DEVLOG_ERROR(...) logMessage(E_ERROR, __VA_ARGS__)
# define DEVLOG_WARNING(...) logMessage(E_WARNING, __VA_ARGS__)
# define DEVLOG_INFO(...) logMessage(E_INFO, __VA_ARGS__)
# define DEVLOG_DEBUG(...) logMessage(E_DEBUG, __VA_ARGS__)
# define DEVLOG_ERROR_VAR(X) X
# define DEVLOG_WARNING_VAR(X) X
# define DEVLOG_INFO_VAR(X) X
# define DEVLOG_DEBUG_VAR(X) X
#endif

#ifdef LOGERROR
# define DEVLOG_ERROR(...) logMessage(E_ERROR, __VA_ARGS__)
# define DEVLOG_WARNING(...)
# define DEVLOG_INFO(...)
# define DEVLOG_DEBUG(...)
# define DEVLOG_ERROR_VAR(X) X
# define DEVLOG_WARNING_VAR(X)
# define DEVLOG_INFO_VAR(X)
# define DEVLOG_DEBUG_VAR(X)
#endif

#ifdef LOGWARNING
# define DEVLOG_ERROR(...) logMessage(E_ERROR, __VA_ARGS__)
# define DEVLOG_WARNING(...) logMessage(E_WARNING, __VA_ARGS__)
# define DEVLOG_INFO(...)
# define DEVLOG_DEBUG(...)
# define DEVLOG_ERROR_VAR(X) X
# define DEVLOG_WARNING_VAR(X) X
# define DEVLOG_INFO_VAR(X)
# define DEVLOG_DEBUG_VAR(X)
#endif

#ifdef LOGINFO
# define DEVLOG_ERROR(...) logMessage(E_ERROR, __VA_ARGS__)
# define DEVLOG_WARNING(...) logMessage(E_WARNING, __VA_ARGS__)
# define DEVLOG_INFO(...) logMessage(E_INFO, __VA_ARGS__)
# define DEVLOG_DEBUG(...)
# define DEVLOG_ERROR_VAR(X) X
# define DEVLOG_WARNING_VAR(X) X
# define DEVLOG_INFO_VAR(X) X
# define DEVLOG_DEBUG_VAR(X)
#endif

#ifdef NOLOG
# define DEVLOG_INFO(...)
# define DEVLOG_WARNING(...)
# define DEVLOG_ERROR(...)
# define DEVLOG_DEBUG(...)
# define DEVLOG_ERROR_VAR(X)
# define DEVLOG_WARNING_VAR(X)
# define DEVLOG_INFO_VAR(X)
# define DEVLOG_DEBUG_VAR(X)
#endif

#if (defined(FORTE_TRACE_EVENTS) && !defined(NOLOG))
# define FORTE_TRACE(...) logMessage(E_TRACE,  __VA_ARGS__)
#else
# define FORTE_TRACE(...)
#endif

#ifndef NOLOG

/*! \brief Adds an Entry to the LogBook
 *
 */
void logMessage(E_MsgLevel paLevel, const char *pacMessage, ...);

#endif  //#ifndef NOLOG

#endif //_DEVLOG_H_
