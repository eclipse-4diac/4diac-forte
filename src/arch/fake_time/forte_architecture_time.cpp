/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Christoph Binder - initial implementation
 *******************************************************************************/

#include "forte_architecture_time.h"
#include "forte_constants.h"

uint_fast64_t fakeForteTime=0;

uint_fast64_t trackFakeForteTime(){
    fakeForteTime++;
    return fakeForteTime;
}

#ifdef FORTE_FAKE_TIME
uint_fast64_t getNanoSecondsMonotonic() {
    return fakeForteTime*1000000;  //ms to ns
}

time_t forte_time(){
    return time(0)+fakeForteTime/1000;  //time() in s
}
#endif


