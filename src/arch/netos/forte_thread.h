/*******************************************************************************
 * Copyright (c) 2006 - 2017 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Rene Smodic, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *  Alois Zoitl - extracted common functions to new base class CThreadBase
 *******************************************************************************/
#ifndef _FORTE_THREAD_H_
#define _FORTE_THREAD_H_

#include "../datatype.h"
#include "../threadbase.h"
#include <threadx/tx_api.h>


/**  \ingroup FORTE-HAL
 * \defgroup netos_hal NET+OS Hardware Abstraction Layer
 *
 * \brief The FORTE-HAL implemenation for the NET+OS operating system
 */

class CTXThread;
typedef CTXThread CThread; //allows that doxygen can generate better documenation
typedef CTXThread *TCTXThreadPtr;

/*! \ingroup netos_hal
 * \brief Multithreading implementation for NET+OS.
 *
 */
class CTXThread : public forte::arch::CThreadBase<ULONG> {
  public:
    /*! \brief Constructor of the Thread class
     *
     *  Does all the neccessary steps in order to get the thread running with the start()-method
     *  @param paStackSize    the Size of the stack the thread is allowed to use. 0 means use system default stack size.
     *  @param paThreadName  the name of the thread
     */
    CTXThread(long paStackSize = 32384, char* paThreadName = "Event Execution Thread");

    /*! \brief Stops and destroys thread.
     *
     *  Will stop the execution if running and destroy the thread including all system specific data.
     */
    virtual ~CTXThread();

    //!Set the deadline of the thread.
    void setDeadline(const CIEC_TIME &paVal);

    /*! \brief Sleep the calling thread
     *
     * @param paMilliSeconds The milliseconds for the thread to sleep
     */
    static void sleepThread(unsigned int paMilliSeconds);
  private:
    /*!\brief Function that is given to the system thread support that should be called for the thread.
     *
     * this function will call the run method of the thread instance.
     */
    static void threadFunction(ULONG paData);

    virtual TThreadHandleType createThread(long paStackSize);

    void setPriority(UINT paPriority) {
      UINT oldPriority;
      tx_thread_priority_change(&getThreadHandle(), paPriority, &oldPriority);
    }

    TX_THREAD mThreadData;

    char *mThreadName;

    static const int scmThreadListSize;
    static TCTXThreadPtr smThreadList[27];

};

#endif /*FORTE_THREAD_H_*/
