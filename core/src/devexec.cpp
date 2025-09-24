/*******************************************************************************
 * Copyright (c) 2005 - 2012 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Thomas Strasser, Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte/devexec.h"

#include "forte/esfb.h"
#include "forte/ecet.h"
#include "forte/extevhan.h"
#include "forte/timerha.h"
#include "forte/util/devlog.h"
#include "forte/device.h"
#include "forte/timerhandlerfactory.h"

namespace forte {
  CDeviceExecution::CDeviceExecution(CDevice &paDevice) : mDevice(paDevice) {
    mRegisteredEventHandlers.reserve(CExternalEventHandler::getFactories().size() + 1);
    mRegisteredEventHandlers.push_back(TimerHandlerFactory::create(*this));
    for (auto &factory : CExternalEventHandler::getFactories()) {
      mRegisteredEventHandlers.push_back(factory(*this));
    }
    getTimer().enableHandler();
  }

  CDeviceExecution::~CDeviceExecution() {
    // FIXME: deleting a handler should disable it automatically
    disableHandlers();
  }

  void CDeviceExecution::startNewEventChain(CEventSourceFB *paECStartFB) const {
    // maybe in the future here has to be added something for handling priority adaption and stuff like this.
    if (nullptr != paECStartFB) {
      CEventChainExecutionThread *poEventChainExecutor = paECStartFB->getEventChainExecutor();
      if (nullptr != poEventChainExecutor) {
        poEventChainExecutor->startEventChain(*paECStartFB->getEventSourceEventEntry());
      } else {
        DEVLOG_ERROR(
            "[CDeviceExecution] Couldn't start new event chain because the event has no CEventChainExecutionThread");
      }
    } else {
      DEVLOG_ERROR("[CDeviceExecution] Couldn't start new event chain because the event source was null");
    }
  }

  CExternalEventHandler *CDeviceExecution::getExtEvHandler(size_t paIdentifer) const {
    return mRegisteredEventHandlers[paIdentifer].get();
  }

  CTimerHandler &CDeviceExecution::getTimer() const {
    return *static_cast<CTimerHandler *>(mRegisteredEventHandlers[0].get());
  }

  void CDeviceExecution::disableHandlers() {
    for (auto &handler : mRegisteredEventHandlers) {
      handler->disableHandler();
    }
  }

  CDevice &CDeviceExecution::getDevice() {
    return mDevice;
  }
} // namespace forte
