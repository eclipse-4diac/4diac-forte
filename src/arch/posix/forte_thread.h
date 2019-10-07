/*******************************************************************************
 * Copyright (c) 2006 - 2017 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Rene Smodic, Ingo Hegny, Martin Melik Merkiumians - initial API and implementation and/or initial documentation
 *  Alois Zoitl - extracted common functions to new base class CThreadBase
 *******************************************************************************/
#ifndef _FORTE_THREAD_H_
#define _FORTE_THREAD_H_

#include "../threadbase.h"
#include <forte_sync.h>
#include <pthread.h>

/**  \ingroup FORTE-HAL 
 * \defgroup posix_hal PC FORTE Hardware Abstraction Layer
 *
 * \brief The FORTE-HAL implementation for normal PCs
 *
 * It will mainly work under linux or linux similar environments like cygwin.
 */

/*! \ingroup posix_hal
 * \brief The thread implementation for the posix thread interface. 
 */
class CPosixThread : public forte::arch::CThreadBase<pthread_t> {
  public:
    /*! \brief Constructor of the Thread class
     *
     *  Does all the necessary steps in order to get the thread running with the start()-method
     *  @param paStackSize the Size of the stack the thread is allowed to use. 0 means use system default stack size.
     *      If you like to set this value it is best to use the form: PTHREAD_STACK_MIN + additional bytes you need.
     */
    explicit CPosixThread(long paStackSize = 0);

    /*! \brief Stops and destroys thread.
     *
     *  Will stop the execution if running and destroy the thread including all system specific data.
     */
    virtual ~CPosixThread();

    //!Set the deadline of the thread.
    void setDeadline(const CIEC_TIME &paVal);

    /*! \brief Sleep the calling thread
     *
     * @param paMilliSeconds The milliseconds for the thread to sleep
     */

    static void sleepThread(unsigned int paMilliSeconds);

  private:
    virtual TThreadHandleType createThread(long paStackSize);

    /*!\brief Function that is given to the system thread support that should be called for the thread.
     *
     * this function will call the run method of the thread instance.
     */
    static void * threadFunction(void *paArguments);

    //we don't want that threads can be copied or assigned therefore the copy constructor and assignment operator are declared private
    //but not implemented
    CPosixThread(const CPosixThread&);
    CPosixThread& operator = (const CPosixThread &);
};


typedef CPosixThread CThread;  //allows that doxygen can generate better documentation

#endif /*FORTE_THREAD_H_*/

