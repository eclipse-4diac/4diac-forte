/*******************************************************************************
 * Copyright (c) 2010 - 2016 ACIN, Profactor GmbH, AIT, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer, Thomas Strasser - initial API and
 *      implementation and/or initial documentation
 *  Alois Zoitl - extracted common functions to new base class CThreadBase
 *******************************************************************************/
#ifndef _THREAD_H_
#define _THREAD_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "./threadbase.h"
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
class CWin32Thread : public forte::arch::CThreadBase{
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

    /*! \brief Stops the execution of the thread
     *
     *  This function immediately stops the execution of the thread (setting alive to false) and waits till
     *  this is finished.
     */
    virtual void end(void);

    /*! \brief Waits for the Thread to finish its execution.
     *
     *  This function waits till the execution in the thread decides to end the execution. Blocks the caller!!!
     */
    void join(void); // Waits

  protected:

    /*!\brief Function that is given to the system thread support that should be called for the thread.
     *
     * this function will call the run method of the thread instance.
     */
    //static void * threadFunction(void *arguments); 
    static DWORD WINAPI threadFunction(LPVOID arguments);


  private:
    //!deadline the thread needs to be finish its execution. 0 means unconstrained.
    CIEC_TIME m_oDeadline;

    /*! \brief data needed for win32 scheduling system to identify the thread.
     */
    HANDLE m_nThreadHandle;

    /*! \brief the 'real' thread ID of the started thread
    */
    DWORD m_nThreadID;

    /*! \brief Size of the stack used by this thread.
     */
    long m_nStackSize;

};

typedef CWin32Thread CThread;  //allows that doxygen can generate better documentation

#endif /*THREAD_H_*/

