/*******************************************************************************
 * Copyright (c) 2006 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Rene Smodic, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _THREAD_H_
#define _THREAD_H_

#include "../datatype.h"
#include <threadx/tx_api.h>
#include <../devlog.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../core/datatypes/forte_time.h"

#define CThread CTXThread  //allows that doxygen can generate better documenation
class CTXThread;
typedef CTXThread *TCTXThreadPtr;

/**  \ingroup FORTE-HAL
 * \defgroup netos_hal NET+OS Hardware Abstraction Layer
 *
 * \brief The FORTE-HAL implemenation for the NET+OS operating system
 *
 *
 */

/*! \ingroup netos_hal
 * \brief Multithreading implementation for NET+OS.
 *
 */
class CTXThread {
  public:
    /*! \brief Constructor of the Thread class
     *
     *  Does all the neccessary steps in order to get the thread running with the start()-method
     *  @param pa_nStackSize    the Size of the stack the thread is allowed to use. 0 means use system default stack size.
     *  @param pa_acThreadName  the name of the thread
     */
    CTXThread(long pa_nStackSize = 32384, char* pa_acThreadName = "Event Execution Thread");

    /*! \brief Stops and destroys thread.
     *
     *  Will stop the execution if running and destroy the thread including all system specific data.
     */
    virtual ~CTXThread();

    /*! \brief Indicates if the thread is allowed to execute.
     *
     *  This functions checks if the Thread is still executing user code in its run()-method.
     *  \return true if there the run method is acitve.
     */
    bool isAlive(void) const {
      return m_bAlive;
    }

    //!Set the deadline of the thread.
    void setDeadline(const CIEC_TIME &pa_roVal);
    //!Get the current deadline of the thread.
    const CIEC_TIME &getDeadline(void) const {
      return m_oDeadLine;
    }

    /*! \brief starts the Thread
     *
     *  By calling this method the execution in the run()-Method will be started. If necessary additional data
     *  can be created here. Because of inheritance resons the best place for executing create is in this method.
     *  For details ask AZ.
     */
    void start(void) {
      create();
    }

    /*! \brief Resumes a suspended Thread
     *
     *
     */
    void resumeSelfSuspend(void) {
      // if the thread is not created yet, we do this here
      tx_semaphore_put(&m_stSemaphore);
    }

    /*! \brief Stops the execution of the thread
     *
     *  This function imidiatly stops the execution of the thread (seting m_bAlive to false) and waits till
     *  this is finished.
     */
    void end(void);

    /*! \brief Waits for the Thread to finish its execution.
     *
     *  This function waits till the execution in the thread decides to end the execution. Blocks the caller!!!
     */
    void join(void);
  protected:

    /*! \brief Suspends the thread.
     *
     *  Suspends the execution of the thread until resumeSelfSuspend(), end(), or join() is called.
     */
    void selfSuspend(void) {
      tx_semaphore_get(&m_stSemaphore, TX_WAIT_FOREVER);
    }

  private:
    /*!\brief Funciton that is given to the system thread support that should be called for the thread.
     *
     * this function will call the run method of the thread instance.
     */
    static void threadFunction(ULONG data);

    /*! \brief Abstract method for the code to execute in the thread.
     *
     *  This thread class has to provide means that the code a inheriting class will add to the run()-method will
     *  be executed in a seperated thread regarding the creator of the CThread class.
     *
     *  The inheriting class has to fullfill the folloing rules when using the run method:
     *    - To end the thread execution simple leave the run()-method
     *    - In order to allow the deletion and stopping of the thread add frequent checks to isAlive and end the
     *      execution if isAlive() returns false.
     */
    virtual void run() = 0;

    void setPriority(UINT pa_nPriority) {
      UINT nOldPrio;
      tx_thread_priority_change(&m_stThread, pa_nPriority, &nOldPrio);
    }

    /*! \brief Creates a new thread.
     *
     *  With this function all the setup things for a new thread are done. The Thread created is initialy
     *  suspended and with the start() method the execution of the code in the run() method is started.
     */
    bool create(void);

    /*! \brief Destroies the thread
     *
     *  This function destroies all the data structures created in the creation phase. All used memory is freed.
     */
    bool destroy(void);

    void printseminfo(TX_SEMAPHORE *pa_stSemaphore);

    //!deadline the thread needs to be finish its execution. 0 means unconstrained.
    CIEC_TIME m_oDeadLine;

    /*! \brief data neede for ecos to identify the thread.
     */
    TX_THREAD m_stThread;

    TX_MUTEX m_stMutex; //!< used for join function

    TX_SEMAPHORE m_stSemaphore; //!< used for resume/suspend

    /*! \brief Size of the stack used by this thread.
     */
    long m_nStackSize;
    void *m_cStack;
    static const char *csm_cName;

    char *m_acThreadName;

    static const int scm_nThreadListSize;
    static TCTXThreadPtr sm_aoThreadList[27];

    /*! \brief Flag that indicates if the Thread is alive.
     *
     *  This flag has two main purposes:
     *    -# indicate for other classes if the thread is still executing
     *    -# use in the run()-method to check if the thread is still allowed to execute (e.g. while(isAlive()) ).
     *       This is important for stopping and destroying threads.
     */
    bool m_bAlive;

    //we don't want that threads can be copied or assigned therefore the copy constructor and assignment operator are declared private
    //but not implemented
    CTXThread(const CTXThread&);
    CTXThread& operator = (const CTXThread &);

};

#endif /*THREAD_H_*/

