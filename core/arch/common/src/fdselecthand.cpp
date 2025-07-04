/*******************************************************************************
 * Copyright (c) 2010 - 2024 ACIN, Profactor GmbH, fortiss GmbH, Jose Cabral
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "forte/arch/sockhand.h" //needs to be first pulls in the platform specific includes
#include "forte/arch/fdselecthand.h"
#include "forte/util/devlog.h"
#include "forte/devexec.h"
#include "forte/cominfra/commfb.h"
#include "forte/cominfra/comCallback.h"
#include "forte/util/criticalregion.h"

namespace forte::arch {
  CFDSelectHandler::CFDSelectHandler(CDeviceExecution &paDeviceExecution) : CExternalEventHandler(paDeviceExecution) {
    mConnectionListChanged = false;
  }

  CFDSelectHandler::~CFDSelectHandler() {
    this->end();
  }

  // single-threaded-network-code
  void CFDSelectHandler::run() {

    struct timeval tv;
    fd_set anFDSet;
    fd_set anFDSetMaster;

    TFileDescriptor nHighestFDID = scmInvalidFileDescriptor;
    int retval = 0;

    FD_ZERO(&anFDSetMaster);

    while (isAlive()) {
      // TODO: create method to prevent 100ms timeout on reconnection
      mSync.lock();
      if (true == mConnectionListChanged) {
        nHighestFDID = createFDSet(&anFDSetMaster);
      }
      anFDSet = anFDSetMaster;
      mSync.unlock();

      tv.tv_sec = 1; // TODO : To be set!
      tv.tv_usec = 0;

      if (scmInvalidFileDescriptor != nHighestFDID) {
        retval = select(static_cast<int>(nHighestFDID + 1), &anFDSet, nullptr, nullptr, &tv);
        if (!isAlive()) {
          // the thread has been closed in the meantime do not process any messages anymore
          return;
        }
      } else {
        retval = 0;
      }

      if (retval > 0) {
        mSync.lock();
        for (auto itRunner = mConnectionsList.begin(); itRunner != mConnectionsList.end();) {
          // need to retrieve the callee as the iterator may get invalid in the recvDat function below in case of
          // connection closing
          com_infra::CComCallback *callee = itRunner->mCallee;
          TFileDescriptor sockDes = itRunner->mSockDes;
          ++itRunner;

          if ((0 != FD_ISSET(sockDes, &anFDSet)) && (nullptr != callee)) {
            mSync.unlock();
            if (com_infra::e_Nothing != callee->recvData(&sockDes, 0)) {
              startNewEventChain(callee->getCommFB());
            }
            mSync.lock();
          }
        }
        mSync.unlock();
      } else {
        if (retval != 0) {
#ifdef WIN32
          DEVLOG_ERROR("Select failed: %d", WSAGetLastError());
#else
          DEVLOG_ERROR("Select failed: %s", strerror(errno));
#endif
        }
      }
    }
  }

  void CFDSelectHandler::addComCallback(TFileDescriptor paFD, com_infra::CComCallback *paComCallback) {
    {
      util::CCriticalRegion criticalRegion(mSync);
      TConnContType stNewNode = {paFD, paComCallback};
      mConnectionsList.push_front(stNewNode);
      mConnectionListChanged = true;
    }
    if (!isAlive()) {
      this->start();
    }
  }

  void CFDSelectHandler::removeComCallback(TFileDescriptor paFD) {
    util::CCriticalRegion criticalRegion(mSync);

    std::erase_if(mConnectionsList, [&paFD](TConnContType x) { return (x.mSockDes == paFD); });

    mConnectionListChanged = true;
  }

  CFDSelectHandler::TFileDescriptor CFDSelectHandler::createFDSet(fd_set *mFDSet) {
    TFileDescriptor nRetVal = scmInvalidFileDescriptor;
    FD_ZERO(mFDSet);
    for (auto itRunner : mConnectionsList) {
      FD_SET(itRunner.mSockDes, mFDSet);
      if (itRunner.mSockDes > nRetVal || scmInvalidFileDescriptor == nRetVal) {
        nRetVal = itRunner.mSockDes;
      }
    }
    mConnectionListChanged = false;
    return nRetVal;
  }
} // namespace forte::arch
