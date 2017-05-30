/*******************************************************************************
 * Copyright (c) 2012, 2016 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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

#define CThread CBEThread  //allows that doxygen can generate better documentation

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
class CBEThread : public forte::arch::CThreadBase, private BETask{

  public:
    /*! \brief Constructor of the Thread class
     *
     *  Does all the neccessary steps in order to get the thread running with the start()-method
     */
    CBEThread();
    char * mTskName;

    /*! \brief Stops and destroys thread.
     *
     *  Will stop the execution if running and destroy the thread including all system specific data.
     */
    virtual ~CBEThread();

    /*! \brief Indicates if the thread is allowed to execute.
     *
     *  This functions checks if the Thread is still executing user code in its run()-method.
     *  \return true if there the run method is acitve.
     */
    bool isAlive(void);

    void setDeadline(TForteUInt32 pa_nVal); //!Set the deadline of the thread.

    virtual void Main(VOID);

    /*! \brief starts the Thread
     *
     *  By calling this method the execution in the run()-Method will be started. If necessary additional data
     *  can be created here. Because of inheritance resons the best place for executing create is in this method.
     *  For details ask AZ.
     */
    void start(void);

    /*! \brief Sleep the calling thread
     *
     * @param pa_miliSeconds The miliseconds for the thread to sleep
     */

    static void sleepThread(unsigned int pa_miliSeconds);

    virtual void join(void);

  private:
    
    /*! \brief Destroies the thread
     *
     *  This function destroies all the data structures created in the creation phase. All used memory is freed.
     */
    bool destroy(void);

};

#endif /*FORTE_THREAD_H_*/

