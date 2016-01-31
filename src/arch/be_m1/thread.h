/*******************************************************************************
 * Copyright (c) 2012 ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _THREAD_H_
#define _THREAD_H_

#include <vxWorks.h>
#include <sysLib.h>
#include <BeTypes.h>
#include <mtypes.h>
#include <BETask.hpp>
#include "../datatype.h"

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
class CBEThread : public BETask{

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
    TForteUInt32 getDeadline(void); //!Get the current deadline of the thread.

    virtual void Main(VOID);

    /*! \brief starts the Thread
     *
     *  By calling this method the execution in the run()-Method will be started. If necessary additional data
     *  can be created here. Because of inheritance resons the best place for executing create is in this method.
     *  For details ask AZ.
     */
    void start(void);

    /*! \brief Resumes a suspended Thread
     *
     *
     */
    void resumeSelfSuspend(void);

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
    void join(void); // Waits
  protected:
    /*! \brief Suspends the thread.
     *
     *  Suspends the execution of the thread until resumeSelfSuspend(), end(), or join() is called.
     */
    void selfSuspend(void);

  private:
    /*! \brief Abstract method for the code to execute in the thread.
     *
     *  This thread class has to provide means that the code a inheriting class will add to the run()-method will
     *  be executed in a seperated thread regarding the creator of the CThread class.
     *
     *  The inheriting class has to fullfill the folloing rules when using the run method:
     *    - To end the thread execution simple leave the run()-method
     *    - In order to allow the deletion and stopping of the thread add frequent checks to isAlive and end the
     *      execution if isAlive() returns false.
     *
     *  \return true if thread execution ended successfull
     */
    virtual void run() = 0;
    
    /*! \brief Destroies the thread
     *
     *  This function destroies all the data structures created in the creation phase. All used memory is freed.
     */
    bool destroy(void);

    /*! \brief Flag that indicates if the Thread is alive.
     *
     *  This flag has two main purposes:
     *    -# indicate for other classes if the thread is still executing
     *    -# use in the run()-method to check if the thread is still allowed to execute (e.g. while(isAlive()) ).
     *       This is important for stopping and destroying threads.
     */
    bool m_bAlive;

    TForteUInt32 m_nDeadline; //!deadline the thread needs to be finish its exectuion. 0 means unconstraint.

};

#endif /*THREAD_H_*/

