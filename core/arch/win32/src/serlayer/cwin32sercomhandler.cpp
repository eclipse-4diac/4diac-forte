/*******************************************************************************
 * Copyright (c) 2013, 2014 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "cwin32sercomhandler.h"
#include "cwin32sercomlayer.h"
#include "forte/util/criticalregion.h"
#include "forte/cominfra/basecommfb.h"

namespace forte::arch {
  CWin32SerComHandler::CWin32SerComHandler(CDeviceExecution &paDeviceExecution) :
      CExternalEventHandler(paDeviceExecution) {
  }

  CWin32SerComHandler::~CWin32SerComHandler() {
    this->end();
  }

  void CWin32SerComHandler::registerSerComLayer(CWin32SerComLayer *paComLayer) {
    {
      util::CCriticalRegion region(mSync);
      mComLayerList.push_back(paComLayer);
    }
    if (!isAlive()) {
      this->start();
    }
    mSem.inc();
  }

  void CWin32SerComHandler::unregisterSerComLayer(CWin32SerComLayer *paComLayer) {
    util::CCriticalRegion region(mSync);
    mComLayerList.erase(std::remove(mComLayerList.begin(), mComLayerList.end(), paComLayer), mComLayerList.end());
  }

  void CWin32SerComHandler::run() {
    while (isAlive()) {

      if (mComLayerList.empty()) {
        mSem.waitIndefinitely();
      }
      if (!isAlive()) {
        break;
      }

      util::CCriticalRegion region(mSync);
      for (CWin32SerComLayer *it : mComLayerList) {
        if (forte::com_infra::e_Nothing != it->recvData(0, 0)) {
          startNewEventChain(it->getCommFB());
        }
      }
      Sleep(1000);
    }
  }
} // namespace forte::arch
