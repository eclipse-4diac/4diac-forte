/*******************************************************************************
 * Copyright (c) 2022, 2024 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Christoph Binder - initial implementation
 *    Ernst Blecha - add jumping to a point in time that has already passed
 *******************************************************************************/
#include "faketimerha.h"
#include "../../core/devexec.h"
#include "ecet.h"
#include "device.h"

uint_fast64_t trackFakeForteTime();
uint_fast64_t jumpFakeForteTime(uint_fast64_t destination);

const CIEC_TIME::TValueType cNanosecondsToMilliseconds = forte::core::constants::cNanosecondsPerSecond / forte::core::constants::cMillisecondsPerSecond;
const CIEC_TIME::TValueType cHourInMilliseconds = 60 * 60 * forte::core::constants::cMillisecondsPerSecond;

CFakeTimerHandler::CFakeTimerHandler(CDeviceExecution &paDeviceExecution) :
  CTimerHandler(paDeviceExecution) {
}

CFakeTimerHandler::~CFakeTimerHandler() {
  disableHandler();
}

void CFakeTimerHandler::run() {
  while(isAlive()) {
    if(!sleepTimes.empty()) {
      CFakeTimerHandler::napinfo nap = sleepTimes.front();
      sleepTimes.pop();
      if(nap.napDuration == 0 && nap.wakeupTime < (getNanoSecondsMonotonic() / cNanosecondsToMilliseconds)) {
        // We have to go back in time!
        nextTick();
        jumpFakeForteTime(nap.wakeupTime);
        DEVLOG_DEBUG("[FAKETIME]: time-jumping to destination %d ms\n", nap.wakeupTime);
        sleepThread(0);
      } else {
        if(nap.wakeupTime != 0) {
          nap.napDuration = nap.wakeupTime - getNanoSecondsMonotonic() / cNanosecondsToMilliseconds;
        }
        if(nap.napDuration > cHourInMilliseconds) {
          // if a destination time is more than one hour away from the current time:
          // skip to one hour before the destination and start simulation from there
          // this is done to avoid timeouts when communicating with 4diac-ide
          jumpFakeForteTime(nap.wakeupTime - cHourInMilliseconds);
          DEVLOG_DEBUG("[FAKETIME]: time-jumping to one hour before destination %d ms\n", nap.wakeupTime - cHourInMilliseconds);
          nap.napDuration = cHourInMilliseconds;
        }
        DEVLOG_DEBUG("[FAKETIME]: advance time for %d ms\n", nap.napDuration);
        auto execThread = getExecThread(nap.fakeSleepFb);
        while(nap.napDuration  > 0) {
          nextTick();
          trackFakeForteTime();
          nap.napDuration--;
          while(execThread != 0 && execThread->isProcessingEvents()) {
            sleepThread(0);
          }
        }
        if(nap.napDuration == 0) {
          DEVLOG_DEBUG("[FAKETIME]: reached target time\n");
        }
      }
      startOutputEvent(nap.fakeSleepFb);
    } else {
      sleepThread(0);
    }
  }
}

void CFakeTimerHandler::sleepToTime(const CIEC_TIME &t) {
  const TLargestUIntValueType wakeup = static_cast<TLargestUIntValueType>(t.getInMilliSeconds());
  sleepTimes.push({
    .napDuration = 0,
    .wakeupTime = wakeup,
    .fakeSleepFb = nullptr
  });
  DEVLOG_DEBUG("[FAKETIME]: received time destination %d ms\n", wakeup);
  while((getNanoSecondsMonotonic() / cNanosecondsToMilliseconds) != wakeup) {
    sleepThread(20);
  }
  DEVLOG_DEBUG("[FAKETIME]: finished waiting for target time\n");
}

CEventChainExecutionThread* CFakeTimerHandler::getExecThread(CFunctionBlock *fakeSleepFb) {
  return fakeSleepFb ? fakeSleepFb->getResource()->getResourceEventExecution() : nullptr;
}

void CFakeTimerHandler::setSleepTime(const CIEC_TIME &t, CFunctionBlock *fb) {
  const TLargestUIntValueType duration = static_cast<TLargestUIntValueType>(t.getInMilliSeconds());
  sleepTimes.push({
    .napDuration = duration,
    .wakeupTime = 0,
    .fakeSleepFb = fb
  });
  DEVLOG_DEBUG("[FAKETIME]: received sleep time of %d ms\n", duration);
}

void CFakeTimerHandler::startOutputEvent(CFunctionBlock *fakeSleepFb) {
  const auto execThread = getExecThread(fakeSleepFb);
  if(fakeSleepFb && execThread) {
    fakeSleepFb->receiveInputEvent(cgExternalEventID, execThread);
  }
  sleepThread(0);
}

void CFakeTimerHandler::enableHandler(void) {
  start();
}

void CFakeTimerHandler::disableHandler(void) {
  end();
}

void CFakeTimerHandler::setPriority(int) {
  // empty implementation
}

int CFakeTimerHandler::getPriority(void) const {
  return 1;
}
