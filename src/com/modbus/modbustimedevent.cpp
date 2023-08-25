/*******************************************************************************
 * Copyright (c) 2012 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "modbustimedevent.h"
#include "../../core/iec61131_functions.h"

CModbusTimedEvent::CModbusTimedEvent(TForteUInt32 paUpdateInterval) :
    mIsStarted(false) {
  mUpdateInterval = paUpdateInterval;

  if(paUpdateInterval == 0) {
    mSingleShotEvent = true;

    activate();
  } else
    mSingleShotEvent = false;
}

void CModbusTimedEvent::setUpdateInterval(TForteUInt32 paUpdateInterval) {
  mUpdateInterval = paUpdateInterval;
}

void CModbusTimedEvent::activate() {
  mStartTime = func_NOW_MONOTONIC().getInMilliSeconds();
  mIsStarted = true;
}

void CModbusTimedEvent::deactivate() {
  mIsStarted = false;
}

bool CModbusTimedEvent::readyToExecute() const {
  uint_fast64_t currentTime = func_NOW_MONOTONIC().getInMilliSeconds();
  if(mUpdateInterval > currentTime) {
    return false;
  }
  currentTime -= mUpdateInterval;

  if(isStarted() && (currentTime > mStartTime || currentTime == mStartTime)) {
    return true;
  }

  return false;
}

void CModbusTimedEvent::restartTimer() {

  activate();
}
