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
#include "freertostiha.h"
#include "forte/devexec.h"

#include "../../common/src/utils/timespec_utils.h"
#include "forte/timerhandlerfactory.h"

using namespace forte::literals;

namespace forte::arch {
  namespace {
    [[maybe_unused]] const TimerHandlerFactory::EntryImpl<CFreeRTOSTimerHandler> entry("default"_STRID);
  }

  CFreeRTOSTimerHandler::CFreeRTOSTimerHandler(CDeviceExecution &paDeviceExecution) : CTimerHandler(paDeviceExecution) {
    mTimer =
        xTimerCreate("FORTETimer", pdMS_TO_TICKS(1000 / getTicksPerSecond()), pdTRUE, (void *) this, vCallbackFunction);
  }

  CFreeRTOSTimerHandler::~CFreeRTOSTimerHandler() {
    disableHandler();
    xTimerDelete(mTimer, 0);
  }

  void CFreeRTOSTimerHandler::enableHandler() {
    xTimerStart(mTimer, 0);
  }

  void CFreeRTOSTimerHandler::disableHandler() {
    xTimerStop(mTimer, 0);
  }

  void CFreeRTOSTimerHandler::vCallbackFunction(TimerHandle_t paTimer) {
    static_cast<CFreeRTOSTimerHandler *>(pvTimerGetTimerID(paTimer))->nextTick();
  }
} // namespace forte::arch
