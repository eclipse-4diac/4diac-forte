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

#include <cstdio> 

#include "../forte.h"

static const unsigned scForteTaskPriority = tskIDLE_PRIORITY + 1;

static const unsigned int scDesiredFortePort = 61499;

static const configSTACK_DEPTH_TYPE scStackDepth = 2000,

void vForteTask(void* ) {
  TForteInstance forteInstance;

  if(auto result = forteStartInstance(scDesiredFortePort, &forteInstance); result != FORTE_OK){
    vTaskDelete(nullptr);
  }

  printf("FORTE is up and running\n");
  forteWaitForInstanceToStop(forteInstance);
  printf("FORTE finished\n");

  vTaskDelete(nullptr);
}

int main() {

  if(auto result = forteGlobalInitialize(0, nullptr); result != FORTE_OK){
    return result;
  }

  xTaskCreate(vForteTask, "forte", scStackDepth, nullptr, scForteTaskPriority, nullptr);

  vTaskStartScheduler();

  // Will not get here unless there is insufficient RAM.
}

