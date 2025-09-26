/*******************************************************************************
 * Copyright (c) 2025 Alexander Trojnin github.com/trojninalex
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alexander Trojnin - initial API and implementation and/or initial documentation
 *  Malte Grave - Adapt the CAN handler to the new 4diac-FORTE structure
 *******************************************************************************/

#include "forte/cominfra/comCallback.h"
#include "forte/cominfra/commfb.h"
#include "forte/devexec.h"
#include "forte/util/criticalregion.h"
#include "forte/util/devlog.h"

#include "CANHandler.h"

using namespace forte::arch;
using namespace forte::util;

namespace forte::com_infra {
  CCANHandler::CCANHandler(CDeviceExecution &paDeviceExecution) : CExternalEventHandler(paDeviceExecution) {
    mConnectionListChanged = false;
  }

  CCANHandler::~CCANHandler() {
    mConnectionsList.clear();

    this->end();
  }

  void CCANHandler::addComCallback(CFDSelectHandler::TFileDescriptor paFD, CComCallback *paCanLayer) {
    DEVLOG_DEBUG("[CAN Handler] Callback handler added\n");

    util::CCriticalRegion criticalRegion(mSync);
    TConnContType stNewNode = {paFD, paCanLayer};

    mConnectionsList.push_back(stNewNode);
    mConnectionListChanged = true;

    if (!isAlive()) {
      this->start();
    }
  }

  void CCANHandler::removeComCallback(CFDSelectHandler::TFileDescriptor paFD) {
    util::CCriticalRegion criticalRegion(mSync);

    auto itRunner(mConnectionsList.begin());
    auto itRefNode(mConnectionsList.end());
    auto itEnd(mConnectionsList.end());

    while (itRunner != itEnd) {
      if (itRunner->mSockDes == paFD) {
        if (itRefNode != itEnd) {
          mConnectionsList.erase(itRefNode);
        }
        break;
      }

      itRefNode = itRunner;
      ++itRunner;
    }

    mConnectionListChanged = true;
  }

  CFDSelectHandler::TFileDescriptor CCANHandler::createFDSet(fd_set *m_panFDSet) {
    CFDSelectHandler::TFileDescriptor nRetVal = CFDSelectHandler::scmInvalidFileDescriptor;
    FD_ZERO(m_panFDSet);

    auto itEnd(mConnectionsList.end());
    for (auto itRunner = mConnectionsList.begin(); itRunner != itEnd; ++itRunner) {
      FD_SET(itRunner->mSockDes, m_panFDSet);
      if (itRunner->mSockDes > nRetVal || CFDSelectHandler::scmInvalidFileDescriptor == nRetVal) {
        nRetVal = itRunner->mSockDes;
      }
    }
    mConnectionListChanged = false;
    return nRetVal;
  }

  void CCANHandler::run() {
    while (isAlive()) {
      auto itEnd(mConnectionsList.end());
      for (auto itRunner = mConnectionsList.begin(); itRunner != itEnd; ++itRunner) {
        memset(&frame, 0, sizeof(can_frame));

        if (read(itRunner->mSockDes, &frame, sizeof(struct can_frame)) > 0) {
          forte::com_infra::CComCallback *called = itRunner->mCalled;

          if (forte::com_infra::e_Nothing != called->recvData(&frame, frame.can_dlc)) {
            CExternalEventHandler::startNewEventChain(called->getCommFB());
          }
        }
      }

      CThread::sleepThread(1);
    }
  }
} // namespace forte::com_infra
