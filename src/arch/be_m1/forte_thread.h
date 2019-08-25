/*******************************************************************************
 * Copyright (c) 2012, 2016, 2017 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Alois Zoitl - extracted common functions to new base class CThreadBase
 *******************************************************************************/
#ifndef _FORTE_THREAD_H_
#define _FORTE_THREAD_H_

#include <vxWorks.h>
#include <sysLib.h>
#include <BeTypes.h>
#include <mtypes.h>
#include <BETask.hpp>
#include "../datatype.h"
#include "../threadbase.h"

/**  \ingroup FORTE-HAL
 * \defgroup bem1_hal Bachmann electronic M1 FORTE Hardware Abstraction Layer
 *
 * \brief The FORTE-HAL implementation for normal Bachmann electronic M1 PLCs
 */

/*! \ingroup bem1_hal
 * \brief The thread implementation for the task class provided by Bachmann electronic.
 *
 * TODO implement priority assignment for realtime event chains.
 */
class CBEThread : public forte::arch::CThreadBase<int, 0, CBEThread>, private BETask{

  public:
    /*! \brief Constructor of the Thread class
     *
     *  Does all the neccessary steps in order to get the thread running with the start()-method
     */
    CBEThread();

    /*! \brief Stops and destroys thread.
     *
     *  Will stop the execution if running and destroy the thread including all system specific data.
     */
    virtual ~CBEThread();

    void setDeadline(TForteUInt32 paVal); //!Set the deadline of the thread.

    virtual void Main(VOID);

    /*! \brief Sleep the calling thread
     *
     * @param paMilliSeconds The milliseconds for the thread to sleep
     */
    static void sleepThread(unsigned int paMilliSeconds);

    static void deleteThread(int);

  private:
    virtual TThreadHandleType createThread(long paStackSize);

};

typedef CBEThread CThread; //allows that doxygen can generate better documentation

#endif /*FORTE_THREAD_H_*/

