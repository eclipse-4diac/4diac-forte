/*********************************************************************
 * Copyright (c) 2017, 2025 fortiss GmbH, Monika Wenger
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 **********************************************************************/

#pragma once

#include "forte/extevhan.h"
#include "forte/arch/forte_thread.h"
#include "forte/util/singlet.h"
#include "forte/arch/forte_sem.h"
#include "xqueryClientLayer.h"
#include <vector>

// cppcheck-suppress noConstructor
class CXqueryHandler : public CExternalEventHandler,
                       public RegisterExternalEventHandler<CXqueryHandler>,
                       private CThread {
  public:
    explicit CXqueryHandler(CDeviceExecution &paDeviceExecution);
    ~CXqueryHandler() override;

  private:
    using TXqueryFBContainer = std::vector<CXqueryClientLayer *>;
    TXqueryFBContainer mXqueryFBList;
    static CSyncObject smXqueryMutex;
    static CSemaphore mStateSemaphore;
    void resumeSuspend();
    void selfSuspend();
    char *result;
    char *info;

  public:
    void enableHandler() override;
    void disableHandler() override;
    int registerLayer(CXqueryClientLayer *paLayer);

  protected:
    void run() override;
};
