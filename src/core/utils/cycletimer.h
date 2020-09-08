/*******************************************************************************
 * Copyright (c) 2020 TU Wien ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Peter Gsellmann, Martin Melik Merkumians - initial implementation
 *******************************************************************************/
#include "timerha.h"
#include "forte_thread.h"
#include "forte_sem.h"
#include "eceta.h"
#include "ecetplc.h"

#ifndef SRC_CORE_UTILS_CYCLETIMER_H_
#define  SRC_CORE_UTILS_CYCLETIMER_H_
/**
 * Watchdog timer class for the IEC 61131-3 cyclic execution.
 */
class CCycleTimer : public CThread {
  public:
    /**
     * Constructor
     * @param paThread To be monitored thread
     * @param paPreset Preset cycle time of IEC 61131-3 thread
     */
    CCycleTimer(CEventChainExecutionThread61131 *paThread, TForteInt32 paPreset);
    /**
     * Destructor
     */
    ~CCycleTimer();
    /**
     * Enters the run routine of the watchdog timer thread
     */
    void run();

  private:
    /**
     * Cycle time
     */
    const uint_fast64_t cycleTime;
    /**
     * Timer Handler
     */
    friend class CTimerHandler;
    /**
     * Associated execution thread
     */
    CEventChainExecutionThread61131 *mThread;
};

#endif /*SRC_CORE_UTILS_CYCLETIMER_H_*/
