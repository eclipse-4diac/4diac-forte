/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_ARCH_FORTE_ARCHITECTURE_TIME_H_
#define SRC_ARCH_FORTE_ARCHITECTURE_TIME_H_

#include <time.h>

#if defined(WINCE)
#include <wce_time.h>

inline
struct tm* forte_localtime(const time_t* pa_time){
  return wceex_localtime(pa_time);
}

inline
time_t forte_mktime(struct tm* pa_tm){
  return wceex_mktime(pa_tm);
}

inline
struct tm* forte_gmtime(const time_t* pa_time){
  return wceex_gmtime(pa_time);
}

inline
time_t forte_time(time_t* pa_time){
  return wceex_time(pa_time);
}
#else

inline
struct tm* forte_localtime(const time_t* pa_time){
  return localtime(pa_time);
}

inline
time_t forte_mktime(struct tm* pa_tm){
  return mktime(pa_tm);
}

inline
struct tm* forte_gmtime(const time_t* pa_time){
  return gmtime(pa_time);
}

inline
time_t forte_time(time_t* pa_time){
  return time(pa_time);
}
#endif

#endif /* SRC_ARCH_FORTE_ARCHITECTURE_TIME_H_ */
