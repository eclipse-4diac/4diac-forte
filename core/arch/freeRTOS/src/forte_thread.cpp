/************************************************************************************
 * Copyright (c) 2016 fortiss GmbH, HR Agartechnik GmbH
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
#include "forte/arch/forte_thread.h"
#include "forte/util/criticalregion.h"

namespace forte::arch {

  /*
   * A working test was done using this priority, the ethernet thread used priority 2 and the IP thread used 3.
   */
  const int CFreeRTOSThread::scmForteTaskPriority = tskIDLE_PRIORITY + 4;

  CFreeRTOSThread::CFreeRTOSThread(long paStackSize) :
      CThreadBase(paStackSize),
      mFreeRTOSStack(getStackDepth()),
      /* Allocate the TCB via pvPortMalloc(), not as a member or via new, because
       * on systems with PSRAM (e.g. ESP32-S3) the CFreeRTOSThread object itself
       * may reside in PSRAM. FreeRTOS requires the TCB to be in internal SRAM;
       * pvPortMalloc() guarantees MALLOC_CAP_INTERNAL on ESP-IDF. */
      mTask{static_cast<StaticTask_t *>(pvPortMalloc(sizeof(StaticTask_t)))} {
  }

  void CFreeRTOSThread::deleteFreeRTOSTask() {
    if (mFreeRTOSThreadHandle != nullptr) {
      /* On SMP (dual-core ESP32) there is a narrow window between runThread() signalling
       * mJoinSem and threadFunction() actually reaching vTaskDelay(). Calling vTaskDelete
       * on a Running task across cores hangs on ESP-IDF SMP, so we wait. */
      while (eTaskGetState(mFreeRTOSThreadHandle) == eRunning) {
        vTaskDelay(1);
      }
      vTaskDelete(mFreeRTOSThreadHandle);
      mFreeRTOSThreadHandle = nullptr;
    }
  }

  CFreeRTOSThread::~CFreeRTOSThread() {
    end();
    /* After end(), run() has returned and join() completed, but the FreeRTOS task is
     * still alive and parked in vTaskDelay(). Delete it now that TCB and stack are no
     * longer needed. */
    deleteFreeRTOSTask();
  }

  void CFreeRTOSThread::threadFunction(void *paData) {
    auto paThreadInst = static_cast<CFreeRTOSThread *>(paData);
    CThreadBase::runThread(paThreadInst);
    /* join() is notified via mJoinSem.inc() inside runThread(), before we reach here.
     * Park the task so the TCB and stack remain valid until ~CFreeRTOSThread()
     * calls vTaskDelete() after join() has returned. */
    vTaskDelay(portMAX_DELAY);
  }

  CThreadBase<TaskHandle_t, TaskHandle_t(0), CFreeRTOSThread>::TThreadHandleType
  CFreeRTOSThread::createThread(long paStackSize) {
    if (mFreeRTOSStack.empty() || !mTask) {
      DEVLOG_ERROR("Error: Could not allocate memory for FreeRTOS Task!\n");
      return TaskHandle_t(0);
    }

    /* Clean up any previously parked task before creating a new one (restart case). */
    deleteFreeRTOSTask();
    mFreeRTOSThreadHandle = xTaskCreateStatic(threadFunction, "FORTE", mFreeRTOSStack.size(), this,
                                              scmForteTaskPriority, mFreeRTOSStack.data(), mTask.get());
    if (mFreeRTOSThreadHandle == nullptr) {
      DEVLOG_ERROR("Error: Could not create FreeRTOS Task thread!");
    }
    return mFreeRTOSThreadHandle;
  }

  void CFreeRTOSThread::sleepThread(unsigned int paMilliSeconds) {
    vTaskDelay(pdMS_TO_TICKS(paMilliSeconds));
  }

} // namespace forte::arch
