/*******************************************************************************
 * Copyright (c) 2010 - 2017 ACIN, Profactor GmbH, AIT, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer, Thomas Strasser - initial API and
 *      implementation and/or initial documentation
 *  Alois Zoitl - extracted common functions to new base class CThreadBase
 *******************************************************************************/
#ifndef _FORTE_THREAD_H_
#define _FORTE_THREAD_H_

#define WIN32_LEAN_AND_MEAN
#include "./threadbase.h"
#include <windows.h>

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
class CWin32Thread : public forte::arch::CThreadBase <HANDLE>{
  public:
    /*! \brief Constructor of the Thread class
     *
     *  Does all the necessary steps in order to get the thread running with the start()-method
     *  @param pa_nStackSize the Size of the stack the thread is allowed to use. 0 means use system default stack size.
     */
    explicit CWin32Thread(long paStackSize = 0);

    /*! \brief Stops and destroys thread.
     *
     *  Will stop the execution if running and destroy the thread including all system specific data.
     */
    virtual ~CWin32Thread();

    //!Set the deadline of the thread.
    void setDeadline(const CIEC_TIME &mVal);

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
    //static void * threadFunction(void *arguments); 
    static DWORD WINAPI threadFunction(LPVOID paArguments);

};

typedef CWin32Thread CThread;  //allows that doxygen can generate better documentation

#endif /*FORTE_THREAD_H_*/

