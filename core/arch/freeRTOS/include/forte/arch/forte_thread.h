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

#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <memory>
#include <vector>

#include "forte/datatype.h"
#include "forte/util/devlog.h"
#include "forte/datatypes/forte_time.h"
#include "forte/arch/threadbase.h"
#include "forte/arch/forte_sync.h"

// Allow to set a special stack size for forte threads
#ifndef configMINIMAL_STACK_SIZE_FORTE
#define configMINIMAL_STACK_SIZE_FORTE 15000
#endif

namespace forte::arch {

  class CFreeRTOSThread;

  typedef CFreeRTOSThread *TFreeRTOSThreadPtr;

  /**  \defgroup FreeRTOS-HAL FreeRTOS FORTE Hardware Abstraction Layer
   *\ingroup FORTE-HAL
   * \brief The FORTE-HAL implementation for the FreeRTOS operating system.
   *
   *
   */
  /*! \ingroup FreeRTOS-HAL
   * \brief This class is a wrapper class the FreeRTOS multitasking support
   */

  class CFreeRTOSThread : public CThreadBase<TaskHandle_t> {
    public:
      /*! \brief Constructor of the Thread class
       *
       *  Does all the necessary steps in order to get the thread running with the start()-method
       *  @param paStackSize the Size of the stack the thread is allowed to use. this class will
       *         allocate the stack size in bytes from the heap
       */
      explicit CFreeRTOSThread(long paStackSize = configMINIMAL_STACK_SIZE_FORTE);

      /*! \brief Stops and destroys thread.
       *
       *  Will stop the execution if running and destroy the thread including all system specific data.
       */
      ~CFreeRTOSThread() override;

      /*! \brief Sleep the calling thread
       *
       * @param pa_miliSeconds The miliseconds for the thread to sleep
       */
      static void sleepThread(unsigned int paMilliSeconds);

    protected:
      void setPriority(int paPriority) {
        DEVLOG_DEBUG(">>>>Thread: Set Priority: %d\n", paPriority);
        vTaskPrioritySet(getThreadHandle(), paPriority);
      }

    private:
      /*!\brief Function that is given to the system thread support that should be called for the thread.
       *
       * this function will call the run method of the thread instance.
       * FreeRTOS requirement that this has to static function! or it has to static_casted
       */
      static void threadFunction(void *paData);

      /*! \brief Creates a new thread.
       *
       *  With this function all the setup things for a new thread are done. The Thread created is initial
       *  suspended and with the start() method the execution of the code in the run() method is started.
       */
      virtual TThreadHandleType createThread(long paStackSize);

      /*! \brief Convert a stack size in bytes to a FreeRTOS stack depth in words.
       *
       * FreeRTOS expects the stack depth as the number of StackType_t words, not bytes.
       * @param paStackSize stack size in bytes
       * @return stack depth in StackType_t words
       */
      constexpr size_t getStackDepth() {
        return static_cast<size_t>(getStackSize()) / sizeof(StackType_t);
      }

      void deleteFreeRTOSTask();

      static const int scmForteTaskPriority;

      std::vector<StackType_t> mFreeRTOSStack;

      // use a functor instead a lambda, to avoid a "internal linkage warning" at compile-time.
      struct STCBDeleter {
          void operator()(StaticTask_t *paTask) const {
            vPortFree(paTask);
          }
      };

      std::unique_ptr<StaticTask_t, STCBDeleter> mTask;
      TaskHandle_t mFreeRTOSThreadHandle = nullptr;
  };

  typedef CFreeRTOSThread CThread; // allows that doxygen can generate better documenation
} // namespace forte::arch
