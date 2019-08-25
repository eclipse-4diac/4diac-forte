/*******************************************************************************
 * Copyright (c) 2018 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <datatype.h>
#include <time.h>

static const TForteInt64 SecondInNanoSeconds = 1000000000LL;

void timespecSub(const struct timespec * const minuend, const struct timespec * const subtrahend, struct timespec * const result){

  result->tv_sec = minuend->tv_sec - subtrahend->tv_sec;
  result->tv_nsec = minuend->tv_nsec - subtrahend->tv_nsec;
  if(result->tv_nsec < 0){
    result->tv_sec--;
    result->tv_nsec = static_cast<long int>(result->tv_nsec + SecondInNanoSeconds); //cast to avoid warnings in some compilers
  }
}

void timespecAdd(const struct timespec * const start, const struct timespec * const end, struct timespec * const result){
  (result)->tv_sec = start->tv_sec + end->tv_sec;
  (result)->tv_nsec = start->tv_nsec + end->tv_nsec;
  if((result)->tv_nsec >= SecondInNanoSeconds){
    ++(result)->tv_sec;
    (result)->tv_nsec =  static_cast<long int>(result->tv_nsec - SecondInNanoSeconds); //cast to avoid warnings in some compilers
  }
}

bool timespecLessThan(const struct timespec *const lhs, const struct timespec *const rhs) {
  if(lhs->tv_sec > rhs->tv_sec) {
    return false;
  }
  if(lhs->tv_sec == rhs->tv_sec && lhs->tv_nsec >= rhs->tv_nsec) {
    return false;
  }
  return true;
}
