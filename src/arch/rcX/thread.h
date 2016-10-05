/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _THREAD_H_
#define _THREAD_H_

#include <rX_Includes.h>
#include "../threadbase.h"
#include <sync.h>

class CrcXThread : public forte::arch::CThreadBase {
  public:

    /*! \brief Constructor of the Thread class
     *
     *  Does all the necessary steps in order to get the thread running with the start()-method
     *  @param pa_nStackSize the Size in bytes of the stack the thread is allowed to use. 128 elements is the minimum, and each element has 4 bytes. A 0 sets the size to the minimum
     */
    explicit CrcXThread(long pa_nStackSize = (128 * 4));

    /*! \brief Stops and destroys thread.
     *
     *  Will stop the execution if running and destroy the thread including all system specific data.
     */
    virtual ~CrcXThread();

    //!Set the deadline of the thread.
    void setDeadline(const CIEC_TIME &pa_roVal);

    /*! \brief starts the Thread
     *
     *  By calling this method the execution in the run()-Method will be started.
     */
    void start(void);

    virtual void join(void);
  protected:

  private:

    /*! \brief Instance defined for the task
     *
     *  Increment task number to identify the different tasks.
     */
    static UINT taskInstance;

    /*!\brief Function that is given to the system thread support that should be called for the thread.
     *
     * this function will call the run method of the thread instance.
     */
    static void * threadFunction(void *arguments);

    CPCSyncObject mJoinMutex;

    /*! \brief data needed for posix scheduling system to identify the thread.
     */
    RX_HANDLE m_stThreadID;
    /*! \brief Size of the stack used by this thread.
     */
    long m_nStackSize;

    /*! \brief Pointer to the memory to be used for this thread stack
     *
     *  This pointer is only not 0 if m_nStackSize is not 0
     */
    void* m_pacStack;

    //we don't want that threads can be copied or assigned therefore the copy constructor and assignment operator are declared private
    //but not implemented
    CrcXThread(const CrcXThread&);
    CrcXThread& operator = (const CrcXThread &);

};


typedef CrcXThread CThread;  //allows that doxygen can generate better documentation

#endif /*THREAD_H_*/

