/*******************************************************************************
 * Copyright (c) 2006 - 2015 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl, Rene Smodic, Ingo Hegny, Martin Melik Merkiumians - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _THREAD_H_
#define _THREAD_H_

#include <semaphore.h>
#include <pthread.h>
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
class CPosixThread {
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

    /*! \brief Indicates if the thread is allowed to execute.
     *
     *  This functions checks if the Thread is still executing user code in its run()-method.
     *  \return true if there the run method is active.
     */
    bool isAlive(void) const {
      return m_bAlive;
    }
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
     *  This function immediately stops the execution of the thread (setting m_bAlive to false) and waits till
     *  this is finished.
     */
    void end(void){
      m_bAlive = false;
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

    void setAlive(bool paAlive){
    	m_bAlive = paAlive;
    }

  private:
    /*!\brief Function that is given to the system thread support that should be called for the thread.
     *
     * this function will call the run method of the thread instance.
     */
    static void * threadFunction(void *arguments);

    /*! \brief Abstract method for the code to execute in the thread.
     *
     *  This thread class has to provide means that the code a inheriting class will add to the run()-method will
     *  be executed in a separated thread regarding the creator of the CThread class.
     *
     *  The inheriting class has to fulfill the following rules when using the run method:
     *    - To end the thread execution simple leave the run()-method
     *    - In order to allow the deletion and stopping of the thread add frequent checks to isAlive and end the
     *      execution if isAlive() returns false.
     */
    virtual void run() = 0;

    /*! \brief Flag that indicates if the Thread is alive.
     *
     *  This flag has two main purposes:
     *    -# indicate for other classes if the thread is still executing
     *    -# use in the run()-method to check if the thread is still allowed to execute (e.g. while(isAlive()) ).
     *       This is important for stopping and destroying threads.
     */
    bool m_bAlive;

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

