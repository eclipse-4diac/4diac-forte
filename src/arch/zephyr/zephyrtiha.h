/************************************************************************************
 Copyright (c) 2023 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef SRC_ARCH_ZEPHYR_ZEPHYRTIHA_H_
#define SRC_ARCH_ZEPHYR_ZEPHYRTIHA_H_

#include "../timerha.h"

#include <zephyr/kernel.h>

/*! \brief External event handler for the Zephyr Timer.
 * \ingroup ZEPHYR-HAL
 *
 */
class CZephyrTimerHandler : public CTimerHandler {
  public:

    explicit CZephyrTimerHandler(CDeviceExecution& paDeviceExecution);

    ~CZephyrTimerHandler() override;

    /*!\brief Enables this event source
     *
     */
    void enableHandler() override;
    /*!\brief Disable this event source
     */
    void disableHandler() override;
    /*!\brief Sets the priority of the event source
     *
     * \param paPriority new priority of the event source
     */
    void setPriority(int paPriority) override;
    /*!\brief Get the current priority of the event source
     *
     * \return current priority
     */
    int getPriority() const override;

  private:

    struct k_timer timer;
    k_thread_stack_t* stack{ 0 };
    struct k_thread* thread{ 0 };
    static void thread_fn(void* arg1, void* arg2, void* arg3);
    k_tid_t thread_id;
};

#endif /* SRC_ARCH_ZEPHYR_ZEPHYRTIHA_H_ */
