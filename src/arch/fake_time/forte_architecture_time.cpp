/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Christoph Binder - initial implementation
 *    Martin Jobst - add high-resolution realtime clock
 *******************************************************************************/

#include "forte_architecture_time.h"
#include "forte_constants.h"

uint_fast64_t fakeForteTime=0;

uint_fast64_t trackFakeForteTime(){
    fakeForteTime++;
    return fakeForteTime;
}

uint_fast64_t jumpFakeForteTime(uint_fast64_t destination) {
    fakeForteTime = destination;
    return fakeForteTime;
}

#ifdef FORTE_FAKE_TIME
uint_fast64_t getNanoSecondsMonotonic() {
    return fakeForteTime * ( forte::core::constants::cNanosecondsPerSecond / forte::core::constants::cMillisecondsPerSecond );  //ms to ns
}

uint_fast64_t getNanoSecondsRealtime() {
    return static_cast<uint_fast64_t>(time(0)) * forte::core::constants::cNanosecondsPerSecond + fakeForteTime * ( forte::core::constants::cNanosecondsPerSecond / forte::core::constants::cMillisecondsPerSecond );
}

time_t forte_time(){
    return time(0) + fakeForteTime / forte::core::constants::cMillisecondsPerSecond;  //time() in s
}
#endif


