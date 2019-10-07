/************************************************************************************
 * Copyright (c) 2016 - 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Milan Vathoopan, Guru Chandrasekhara - initial API and implementation and/or initial documentation
 * Jose Cabral - Cleaning
 ************************************************************************************/
#include <criticalregion.h>
#include <fortealloc.h>

#include "forte_thread.h"

/*
 * A working test was done using this priority, the ethernet thread used priority 2 and the IP thread used 3.
 */
const int CFreeRTOSThread::scmForteTaskPriority = tskIDLE_PRIORITY + 4;

CFreeRTOSThread::CFreeRTOSThread(long paStackSize) :
    CThreadBase(paStackSize) {
  mStack = new char[paStackSize];
}

CFreeRTOSThread::~CFreeRTOSThread() {
}

void CFreeRTOSThread::threadFunction(void *paData) {
  CThreadBase::runThread(static_cast<CFreeRTOSThread *>(paData));
  /* Tasks must not attempt to return from their implementing
   function or otherwise exit.
   https://www.freertos.org/implementing-a-FreeRTOS-task.html */
  vTaskDelete(NULL);
}

forte::arch::CThreadBase<TaskHandle_t, TaskHandle_t(0), CFreeRTOSThread>::TThreadHandleType CFreeRTOSThread::createThread(long paStackSize) {
  TaskHandle_t handle = 0;

  if(pdPASS != xTaskCreate(threadFunction, "FORTE", paStackSize, this, scmForteTaskPriority, &handle)) {
    DEVLOG_ERROR("Error: Could not create FreeRTOS Task thread!");
  }

  return handle;
}

void CFreeRTOSThread::sleepThread(unsigned int paMilliSeconds) {
  vTaskDelay(pdMS_TO_TICKS(paMilliSeconds));
}
void CFreeRTOSThread::setDeadline(const CIEC_TIME &paVal) {
}
