/*******************************************************************************
 * Copyright (c) 2006 - 2017 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Carolyn Oates - initial API and implementation and/or initial documentation
 *   Alois Zoitl - extracted common functions to new base class CThreadBase
 *******************************************************************************/
#ifndef _FORTE_THREAD_H_
#define _FORTE_THREAD_H_

#include <cyg/kernel/kapi.h>
#include "../datatype.h"
#include "../threadbase.h"
#include "../devlog.h"

class CECOSThread;
typedef CECOSThread CThread;  //allows that doxygen can generate better documentation
typedef CECOSThread *TCECOSThreadPtr;

/**  \defgroup ECOS-HAL eCos FORTE Hardware Abstraction Layer
 *\ingroup FORTE-HAL
 * \brief The FORTE-HAL implementation for the eCos operating system.
 *
 * 
 */
/*! \ingroup ECOS-HAL
 * \brief This class is a wrapper class the eCos multitasking support 
 */
class CECOSThread : public forte::arch::CThreadBase<cyg_handle_t, 0, CECOSThread> {
  public:
    /*! \brief Constructor of the Thread class
     *
     *  Does all the necessary steps in order to get the thread running with the start()-method
     *  @param paStackSize the Size of the stack the thread is allowed to use. this class will
     *         allocate the stack size in bytes from the heap
     */
    explicit CECOSThread(long paStackSize = CYGNUM_HAL_STACK_SIZE_TYPICAL);  // may need adjustment for small platforms

    /*! \brief Stops and destroys thread.
     *
     *  Will stop the execution if running and destroy the thread including all system specific data.
     */
    virtual ~CECOSThread();

    //!Set the deadline of the thread.
    void setDeadline(const CIEC_TIME &paVal);

    /*! \brief Sleep the calling thread
     *
     * @param pamilliSeconds The milliseconds for the thread to sleep
     */
    static void sleepThread(unsigned int paMilliSeconds);

    static void deleteThread(cyg_handle_t paThreadHandle);

  protected:
    void setPriority(cyg_priority_t paPriority) {
      DEVLOG_DEBUG(">>>>Thread: Set Priority: %d\n", paPriority);
      cyg_thread_set_priority(getThreadHandle(), paPriority);
    }

  private:
    /*!\brief Function that is given to the system thread support that should be called for the thread.
     *
     * this function will call the run method of the thread instance.
     */
    static void threadFunction(cyg_addrword_t paData);

    virtual TThreadHandleType createThread(long paStackSize);

    static const int scmThreadListSize = 27;
    static TCECOSThreadPtr smThreadList[scmThreadListSize];
    static CSyncObject smThreadListLock;

    /*! \brief data needed for ecos to identify the thread.
     */
    cyg_thread mThread;
};

#endif /*FORTE_THREAD_H_*/
