/*******************************************************************************
 * Copyright (c) 2006 - 2016 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl, Rene Smodic, Ingo Hegny, Martin Melik Merkiumians - initial API and implementation and/or initial documentation
 *  Alois Zoitl - extracted common functions to new base class CThreadBase
 *******************************************************************************/
#ifndef _THREAD_H_
#define _THREAD_H_

#include <semaphore.h>
#include <pthread.h>
#include "../threadbase.h"
#include "../datatype.h"
#include "../../core/datatypes/forte_time.h"
#include <sync.h>

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
class CPosixThread : public forte::arch::CThreadBase {
  public:
    /*! \brief Constructor of the Thread class
     *
     *  Does all the necessary steps in order to get the thread running with the start()-method
     *  @param pa_nStackSize the Size of the stack the thread is allowed to use. 0 means use system default stack size.
     *      If you like to set this value it is best to use the form: PTHREAD_STACK_MIN + additional bytes you need.
     */
    CPosixThread(long pa_nStackSize = 0);

    /*! \brief Stops and destroys thread.
     *
     *  Will stop the execution if running and destroy the thread including all system specific data.
     */
    virtual ~CPosixThread();

    //!Set the deadline of the thread.
    void setDeadline(const CIEC_TIME &pa_roVal);

    //!Get the current deadline of the thread.
    const CIEC_TIME &getDeadline(void) const {
      return m_oDeadline;
    }

    /*! \brief starts the Thread
     *
     *  By calling this method the execution in the run()-Method will be started.
     */
    void start(void);

    /*! \brief Resumes a suspended Thread
     *
     *
     */
    void resumeSelfSuspend(void);

    /*! \brief Stops the execution of the thread
     *
     *  This function immediately stops the execution of the thread (setting alive to false) and waits till
     *  this is finished.
     */
    void end(void){
      setAlive(false);
      resumeSelfSuspend();
      join();
    }

    /*! \brief Waits for the Thread to finish its execution.
     *
     *  This function waits till the execution in the thread decides to end the execution. Blocks the caller!!!
     */
    void join(void); // Waits
  protected:
    /*! \brief Suspends the thread.
     *
     *  Suspends the execution of the thread until resumeSelfSuspend(), end(), or join() is called.
     */
    void selfSuspend();

  private:
    /*!\brief Function that is given to the system thread support that should be called for the thread.
     *
     * this function will call the run method of the thread instance.
     */
    static void * threadFunction(void *arguments);

    //!deadline the thread needs to be finish its execution. 0 means unconstrained.
    CIEC_TIME m_oDeadline;

    sem_t m_stSuspendSemaphore;

    CPCSyncObject mJoinMutex;

    /*! \brief data needed for posix scheduling system to identify the thread.
     */
    pthread_t m_stThreadID;
    /*! \brief Size of the stack used by this thread.
     */
    long m_nStackSize;

    /*! \brief Pointer to the memory to be used for this thread'm_stSuspendSemaphore stack
     *
     *  This pointer is only not 0 if m_nStackSize is not 0
     */
    char *m_pacStack;

    //we don't want that threads can be copied or assigned therefore the copy constructor and assignment operator are declared private
    //but not implemented
    CPosixThread(const CPosixThread&);
    CPosixThread& operator = (const CPosixThread &);

};


typedef CPosixThread CThread;  //allows that doxygen can generate better documentation

#endif /*THREAD_H_*/

