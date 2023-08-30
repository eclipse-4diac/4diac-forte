/*********************************************************************
* Copyright (c) 2017 fortiss GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XQUERY_XQUERYHANDLER_H_
#define XQUERY_XQUERYHANDLER_H_

#include <extevhan.h>
#include <forte_thread.h>
#include <singlet.h>
#include <fortelist.h>
#include <xqueryClientLayer.h>
#include <forte_sem.h>

// cppcheck-suppress noConstructor
class CXqueryHandler : public CExternalEventHandler, private CThread{
  DECLARE_HANDLER(CXqueryHandler)

  private:
    typedef CSinglyLinkedList<CXqueryClientLayer *> TXqueryFBContainer;
    TXqueryFBContainer mXqueryFBList;
    static CSyncObject smXqueryMutex;

    static forte::arch::CSemaphore mStateSemaphore;
    void resumeSuspend();
    void selfSuspend();

    char *result;
    char *info;

  public:
    void enableHandler() override;
    void disableHandler() override;
    void setPriority(int pa_prio) override;
    int getPriority() const override;

    int registerLayer(CXqueryClientLayer* paLayer);

  protected:
    void run() override;

};

#endif /* XQUERY_XQUERYHANDLER_H_ */
