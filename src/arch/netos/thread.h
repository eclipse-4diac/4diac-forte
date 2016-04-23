/*******************************************************************************
 * Copyright (c) 2006 - 2016 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Rene Smodic, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *  Alois Zoitl - extracted common functions to new base class CThreadBase
 *******************************************************************************/
#ifndef _THREAD_H_
#define _THREAD_H_

#include "../datatype.h"
#include "../threadbase.h"
#include <threadx/tx_api.h>
#include <../devlog.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
class CTXThread : public forte::arch::CThreadBase {
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

    //!Set the deadline of the thread.
    void setDeadline(const CIEC_TIME &pa_roVal);

    /*! \brief starts the Thread
     *
     *  By calling this method the execution in the run()-Method will be started. If necessary additional data
     *  can be created here. Because of inheritance resons the best place for executing create is in this method.
     *  For details ask AZ.
     */
    void start(void) {
      create();
    }

    virtual void join(void);
  protected:

  private:
    /*!\brief Funciton that is given to the system thread support that should be called for the thread.
     *
     * this function will call the run method of the thread instance.
     */
    static void threadFunction(ULONG data);

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

    /*! \brief data neede for ecos to identify the thread.
     */
    TX_THREAD m_stThread;

    TX_MUTEX m_stMutex; //!< used for join function

    /*! \brief Size of the stack used by this thread.
     */
    long m_nStackSize;
    void *m_cStack;
    static const char *csm_cName;

    char *m_acThreadName;

    static const int scm_nThreadListSize;
    static TCTXThreadPtr sm_aoThreadList[27];

    //we don't want that threads can be copied or assigned therefore the copy constructor and assignment operator are declared private
    //but not implemented
    CTXThread(const CTXThread&);
    CTXThread& operator = (const CTXThread &);

};

#endif /*THREAD_H_*/

