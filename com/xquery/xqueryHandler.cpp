/*********************************************************************
 * Copyright (c) 2017, 2025 fortiss GmbH, Monika Wenger
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 **********************************************************************/

#include "xqueryHandler.h"

#include "forte/cominfra/commfb.h"
#include "forte/util/criticalregion.h"
#include "forte/util/devlog.h"
#include "xqueryClientLayer.h"

extern "C" {
#include <basexdbc.h>
}

namespace forte::com_infra::xquery {

  arch::CSyncObject CXqueryHandler::smXqueryMutex = arch::CSyncObject();
  forte::arch::CSemaphore CXqueryHandler::mStateSemaphore = forte::arch::CSemaphore();

  CXqueryHandler::CXqueryHandler(CDeviceExecution &paDeviceExecution) : CExternalEventHandler(paDeviceExecution) {
    result = nullptr;
    info = nullptr;
  }

  CXqueryHandler::~CXqueryHandler() {
    if (isAlive()) {
      {
        util::CCriticalRegion sectionState(smXqueryMutex);
        setAlive(false);
        resumeSuspend();
      }
      free(result);
      free(info);
      end();
    }
  }

  void CXqueryHandler::enableHandler() {
    if (!isAlive()) {
      start();
    }
  }

  void CXqueryHandler::disableHandler() {
    if (isAlive()) {
      setAlive(false);
      resumeSuspend();
      end();
    }
  }

  int CXqueryHandler::registerLayer(CXqueryClientLayer *paLayer) {
    mXqueryFBList.emplace_back(paLayer);
    enableHandler();
    resumeSuspend();
    return 0;
  }

  void CXqueryHandler::run() {
    while (isAlive()) {
      if (mXqueryFBList.empty()) {
        selfSuspend();
      } else {
        auto it = mXqueryFBList.begin();
        CXqueryClientLayer *xc = *it;
        if (xc && xc->getSfd() > -1) {
          int rc = basex_execute(xc->getSfd(), xc->getCommand(), &result, &info);
          if (rc == -1) { // general (i/o or the like) error
            DEVLOG_ERROR("An error occured during execution of '%s'.\n", xc->getCommand());
            free(result);
            free(info);
          }
          if (rc > 0) { // database error while processing command
            DEVLOG_ERROR("Processing of '%s' failed.\n", xc->getCommand());
          } else {
            if (e_Nothing != xc->recvData(result, strlen(result))) {
              startNewEventChain(xc->getCommFB());
            }
          }
        } else {
          DEVLOG_ERROR("Connection seems to be lost, query not sent.\n");
        }
        mXqueryFBList.erase(mXqueryFBList.begin());
      }
    }
  }

  void CXqueryHandler::resumeSuspend() {
    mStateSemaphore.inc();
  }

  void CXqueryHandler::selfSuspend() {
    mStateSemaphore.waitIndefinitely();
  }

} // namespace forte::com_infra::xquery
