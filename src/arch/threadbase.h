/*******************************************************************************
 * Copyright (c) 2016, 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef _THREADBASE_H_
#define _THREADBASE_H_

#include "../core/datatypes/forte_time.h"
#include <forte_sync.h>

namespace forte {
  namespace arch {

    template <typename TThreadHandle, TThreadHandle nullHandle = 0 >
    class CThreadBase{
      public:

        typedef TThreadHandle TThreadHandleType;

        /*! \brief Indicates if the thread is allowed to execute.
         *
         *  This functions checks if the Thread is still executing user code in its run()-method.
         *  \return true if there the run method is active.
         */
        bool isAlive(void) const{
          return mAlive;
        }

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
        void end();

        /*! \brief Waits for the Thread to finish its execution.
         *
         *  This function waits till the execution in the thread decides to end the execution. Blocks the caller!!!
         */
        virtual void join();

        //!Get the current deadline of the thread.
        const CIEC_TIME &getDeadline(void) const {
          return mDeadline;
        }

      protected:
        CThreadBase(long paStackSize);

        virtual ~CThreadBase();

        /*! \brief set the alive flag for this flag
         */
        void setAlive(bool paVal) {
          mAlive = paVal;
        }

        /*! \brief Helper method to run the thread.
         *
         * This method prepares the given thread and then runs it. This method will also handle everything needed for
         * the join implementation.
         *
         * This method is to be called by the system specific thread function.
         */
        static void runThread(CThreadBase *paThread);

        //!deadline the thread needs to be finish its execution. 0 means unconstrained.
        CIEC_TIME mDeadline;


      private:
        /*! \brief Abstract method for the code to execute in the thread.
         *
         *  This thread class has to provide means that the code a inheriting class will add to the run()-method will
         *  be executed in a separated thread regarding the creator of the CThread class.
         *
         *  The inheriting class has to fulfill the following rules when using the run method:
         *    - To end the thread execution simple leave the run()-method
         *    - In order to allow the deletion and stopping of the thread add frequent checks to isAlive() and end the
         *      execution if isAlive() returns false.
         */
        virtual void run() = 0;

        /*! \brief create the thread and return a handle to it
         *
         * @return handle to the newly created thread
         */
        virtual TThreadHandleType createThread(long paStackSize) = 0;

        //! Mutex for implementing a generic join functionality. For a stable functionality this mutex must be locked during thread creation.
        CSyncObject mJoinMutex;

        TThreadHandle mThreadHandle;

        /*! \brief Size of the stack used by this thread.
         */
        long mStackSize;

        /*! \brief Flag that indicates if the Thread is alive.
         *
         *  This flag has two main purposes:
         *    -# indicate for other classes if the thread is still executing
         *    -# use in the run()-method to check if the thread is still allowed to execute (e.g. while(isAlive()) ).
         *       This is important for stopping and destroying threads.
         */
        volatile  bool mAlive;

        //we don't want that threads can be copied or assigned therefore the copy constructor and assignment operator are declared private
        //but not implemented
        CThreadBase(const CThreadBase&);
        CThreadBase& operator = (const CThreadBase &);
    };

  } /* namespace arch */
} /* namespace forte */

#include "threadbase.tpp"

#endif /* SRC_ARCH_THREADBASE_H_ */
