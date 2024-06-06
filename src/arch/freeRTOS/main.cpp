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

#include "FreeRTOS.h"
#include "task.h"

#include "../forte_architecture.h"
#include "../devlog.h"
#include "forteinstance.h"

const static unsigned mainFORTE_TASK_PRIORITY = tskIDLE_PRIORITY + 1;

void vForteTask(void* ) {
  C4diacFORTEInstance instance;

  if(instance.startupNewDevice ("localhost:61499")) {
    DEVLOG_INFO("FORTE is up and running\n");
    instance.awaitDeviceShutdown();
    DEVLOG_INFO("FORTE finished\n");
  }
  vTaskDelete(nullptr);
}

void vStartForteServerTask(UBaseType_t uxPriority) {
  /* Spawn the task. */
  xTaskCreate(vForteTask, "forte", 2000, nullptr, uxPriority, nullptr);
}

int main() {

  vStartForteServerTask(mainFORTE_TASK_PRIORITY);

  vTaskStartScheduler();

  // Will not get here unless there is insufficient RAM.
}

