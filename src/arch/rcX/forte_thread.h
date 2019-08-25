/*******************************************************************************
 * Copyright (c) 2016, 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _FORTE_THREAD_H_
#define _FORTE_THREAD_H_

#include <rX_Includes.h>
#include "../threadbase.h"

class CrcXThread : public forte::arch::CThreadBase<RX_HANDLE, 0, CrcXThread> {
  public:

    /*! \brief Constructor of the Thread class
     *
     *  Does all the necessary steps in order to get the thread running with the start()-method
     *  @param paStackSize the Size in bytes of the stack the thread is allowed to use. 128 elements is the minimum, and each element has 4 bytes. A 0 sets the size to the minimum
     */
    explicit CrcXThread(long paStackSize = (300 * 4));

    /*! \brief Stops and destroys thread.
     *
     *  Will stop the execution if running and destroy the thread including all system specific data.
     */
    virtual ~CrcXThread();

    virtual void join(void);

    //!Set the deadline of the thread.
    void setDeadline(const CIEC_TIME &paVal);

    /*! \brief Sleep the calling thread
     *
     * @param paMilliSeconds The milliseconds for the thread to sleep
     */
    static void sleepThread(unsigned int paMilliSeconds);

    static void deleteThread(RX_HANDLE paThreadHandle);

  private:

    /*! \brief Instance defined for the task
     *
     *  Increment task number to identify the different tasks.
     */
    static UINT smTaskInstance;

    /*! \brief Instance defined for the task
     *
     *  Increment task number to identify the different tasks.
     */
    static RX_TASK_TOKEN smTaskToken;

    /*!\brief Function that is given to the system thread support that should be called for the thread.
     *
     * this function will call the run method of the thread instance.
     */
    static void threadFunction(void *paArguments);

    virtual TThreadHandleType createThread(long paStackSize);

};


typedef CrcXThread CThread;  //allows that doxygen can generate better documentation

#endif /*FORTE_THREAD_H_*/

