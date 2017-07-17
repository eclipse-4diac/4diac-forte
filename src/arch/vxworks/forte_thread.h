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
#ifndef _FORTE_THREAD_H_
#define _FORTE_THREAD_H_

#include "vxWorks.h"
#include "taskLib.h"
#include "../threadbase.h"
#include "forte_sync.h"

class CVxWorksThread : public forte::arch::CThreadBase {
  public:

    /*! \brief Constructor of the Thread class
     *
     *  Does all the necessary steps in order to get the thread running with the start()-method
     *  @param pa_nStackSize the Size in bytes of the stack the thread is allowed to use. 128 elements is the minimum, and each element has 4 bytes. A 0 sets the size to the minimum
     */
    explicit CVxWorksThread(long pa_nStackSize = 20000); //TODO: Check default value. Calling the mlpi connect makes the stack go up to 15796

    /*! \brief Stops and destroys thread.
     *
     *  Will stop the execution if running and destroy the thread including all system specific data.
     */
    virtual ~CVxWorksThread();

    //!Set the deadline of the thread.
    void setDeadline(const CIEC_TIME &pa_roVal);

    /*! \brief starts the Thread
     *
     *  By calling this method the execution in the run()-Method will be started.
     */

    /*! \brief Sleep the calling thread
     *
     * @param pa_miliSeconds The miliseconds for the thread to sleep
     */

    static void sleepThread(unsigned int pa_miliSeconds);

    void start(void);

    virtual void join(void);
  protected:

  private:

    /*!\brief Function that is given to the system thread support that should be called for the thread.
     *
     * this function will call the run method of the thread instance.
     */
    static void threadFunction(void *arguments);

    CSyncObject mJoinMutex;

    /*! \brief data needed for posix scheduling system to identify the thread.
     */
    TASK_ID mThreadID;
    /*! \brief Size of the stack used by this thread.
     */
    long mStackSize;

    //we don't want that threads can be copied or assigned therefore the copy constructor and assignment operator are declared private
    //but not implemented
    CVxWorksThread(const CVxWorksThread&);
    CVxWorksThread& operator = (const CVxWorksThread &);

};


typedef CVxWorksThread CThread;  //allows that doxygen can generate better documentation

#endif /*FORTE_THREAD_H_*/

