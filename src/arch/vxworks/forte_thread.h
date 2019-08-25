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

#include "vxWorks.h"
#include "taskLib.h"
#include "../threadbase.h"

class CVxWorksThread : public forte::arch::CThreadBase<TASK_ID, TASK_ID_ERROR> {
  public:

    /*! \brief Constructor of the Thread class
     *
     *  Does all the necessary steps in order to get the thread running with the start()-method
     *  @param paStackSize the Size in bytes of the stack the thread is allowed to use. 128 elements is the minimum, and each element has 4 bytes. A 0 sets the size to the minimum
     */
    explicit CVxWorksThread(long paStackSize = 20000); //TODO: Check default value. Calling the mlpi connect makes the stack go up to 15796

    /*! \brief Stops and destroys thread.
     *
     *  Will stop the execution if running and destroy the thread including all system specific data.
     */
    virtual ~CVxWorksThread();

    //!Set the deadline of the thread.
    void setDeadline(const CIEC_TIME &paVal);

    /*! \brief Sleep the calling thread
     *
     * @param paMilliSeconds The milliseconds for the thread to sleep
     */

    static void sleepThread(unsigned int paMilliSeconds);

  protected:

  private:
    static const int scmInitialTaskPriority = 25;

    virtual TThreadHandleType createThread(long paStackSize);

    /*!\brief Function that is given to the system thread support that should be called for the thread.
     *
     * this function will call the run method of the thread instance.
     */
    static void threadFunction(void *paArguments);

};


typedef CVxWorksThread CThread;  //allows that doxygen can generate better documentation

#endif /*FORTE_THREAD_H_*/
