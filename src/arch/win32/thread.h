/*******************************************************************************
 * Copyright (c) 2010 - 2015 ACIN, Profactor GmbH, AIT, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer, Thomas Strasser - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _THREAD_H_
#define _THREAD_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../datatype.h"
#include "../../core/datatypes/forte_time.h"


/**  \ingroup FORTE-HAL 
 * \defgroup win32_hal PC FORTE Hardware Abstraction Layer
 *
 * \brief The FORTE-HAL implementation for normal win32
 *
 */

/*! \ingroup win32_hal
 * \brief The thread implementation for the win32 thread interface.
 *  
 */
class CWin32Thread{
  public:
    /*! \brief Constructor of the Thread class
     *
     *  Does all the necessary steps in order to get the thread running with the start()-method
     *  @param pa_nStackSize the Size of the stack the thread is allowed to use. 0 means use system default stack size.
     */
    CWin32Thread(long pa_nStackSize = 0);

    /*! \brief Stops and destroys thread.
     *
     *  Will stop the execution if running and destroy the thread including all system specific data.
     */
    virtual ~CWin32Thread();

    /*! \brief Indicates if the thread is allowed to execute.
     *
     *  This functions checks if the Thread is still executing user code in its run()-method.
     *  \return true if there the run method is active.
     */
    bool isAlive(void) const;

    //!Set the deadline of the thread.
    void setDeadline(const CIEC_TIME &m_roVal);

    //!Get the current deadline of the thread.
    const CIEC_TIME &getDeadline(void) const;

    /*! \brief starts the Thread
     *
     *  By calling this method the execution in the run()-Method will be started. If necessary additional data
     *  can be created here. Because of inheritance reasons the best place for executing create is in this method.
     */
    void start();

    /*! \brief Resumes a suspended Thread
     */
    void resumeSelfSuspend();

    /*! \brief Stops the execution of the thread
     *
     *  This function immediately stops the execution of the thread (setting m_bAlive to false) and waits till
     *  this is finished.
     */
    void end(void);

    /*! \brief Waits for the Thread to finish its execution.
     *
     *  This function waits till the execution in the thread decides to end the execution. Blocks the caller!!!
     */
    void join(void); // Waits

  protected:
    /*! \brief Flag that indicates if the Thread is alive.
     *
     *  This flag has two main purposes:
     *    -# indicate for other classes if the thread is still executing
     *    -# use in the run()-method to check if the thread is still allowed to execute (e.g. while(isAlive()) ).
     *       This is important for stopping and destroying threads.
     */
    bool m_bAlive;

    /*!\brief Function that is given to the system thread support that should be called for the thread.
     *
     * this function will call the run method of the thread instance.
     */
    //static void * threadFunction(void *arguments); 
    static DWORD WINAPI threadFunction(LPVOID arguments);

    /*! \brief Suspends the thread.
     *
     *  Suspends the execution of the thread until resumeSelfSuspend(), end(), or join() is called.
     */
    void selfSuspend(void);

  private:
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

    //!deadline the thread needs to be finish its execution. 0 means unconstrained.
    CIEC_TIME m_oDeadline;

    /*! \brief data needed for win32 scheduling system to identify the thread.
     */
    HANDLE m_nThreadHandle;

    /*! \brief the 'real' thread ID of the started thread
    */
    DWORD m_nThreadID;

    //! Semaphore to handle the self suspend mechanism required for FORTE-Threads more reliable
    HANDLE m_hSelfSuspendSemaphore;

    /*! \brief Size of the stack used by this thread.
     */
    long m_nStackSize;

    //we don't want that threads can be copied or assigned therefore the copy constructor and assignment operator are declared private
    //but not implemented
    CWin32Thread(const CWin32Thread&);
    CWin32Thread& operator = (const CWin32Thread &);
};

typedef CWin32Thread CThread;  //allows that doxygen can generate better documentation

#endif /*THREAD_H_*/

