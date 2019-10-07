/************************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Milan Vathoopan, Guru Chandrasekhara - initial API and implementation and/or initial documentation
 ************************************************************************************/
#include "../../core/devexec.h"
#include "fortenew.h"
#include "freertostiha.h"

CTimerHandler* CTimerHandler::createTimerHandler(CDeviceExecution& paDeviceExecution) {
  return new CFreeRTOSTimerHandler(paDeviceExecution);
}

CFreeRTOSTimerHandler::CFreeRTOSTimerHandler(CDeviceExecution& paDeviceExecution) :
    CTimerHandler(paDeviceExecution) {
  mTimer = xTimerCreate("FORTETimer", pdMS_TO_TICKS(1000 / getTicksPerSecond()), pdTRUE, (void *) this, vCallbackFunction);
}

CFreeRTOSTimerHandler::~CFreeRTOSTimerHandler() {
  disableHandler();
  xTimerDelete(mTimer, 0);
}

void CFreeRTOSTimerHandler::enableHandler(void) {
  xTimerStart(mTimer, 0);
}

void CFreeRTOSTimerHandler::disableHandler(void) {
  xTimerStop(mTimer, 0);
}

void CFreeRTOSTimerHandler::setPriority(int paPriority) {

}

int CFreeRTOSTimerHandler::getPriority(void) const {
  return 1;
}

void CFreeRTOSTimerHandler::vCallbackFunction(TimerHandle_t paTimer) {
  static_cast<CFreeRTOSTimerHandler*>(pvTimerGetTimerID(paTimer))->nextTick();
}

