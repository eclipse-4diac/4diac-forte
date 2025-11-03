/*******************************************************************************
 * Copyright (c) 2005, 2025 ACIN, Profactor GmbH, fortiss GmbH, Hit robot group
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Thomas Strasser, Rene Smodic, Gerhard Ebenhofer, Ingo Hegny
 *    Micheal Hofmann
 *      - initial implementation and rework communication infrastructure
 *    zhaoxin
 *      -  fix that external event queue becomes event locker after it is full
 *******************************************************************************/
#include "forte/ecet.h"
#include "forte/ecetfactory.h"
#include "forte/util/criticalregion.h"
#include "forte/util/devlog.h"

using namespace forte::literals;

namespace forte {
  namespace {
    [[maybe_unused]] const EcetFactory::EntryImpl<CEventChainExecutionThread> entry("default"_STRID);
  }

  CEventChainExecutionThread::CEventChainExecutionThread() : arch::CThread() {
    clear();
  }

  CEventChainExecutionThread::~CEventChainExecutionThread() = default;

  void CEventChainExecutionThread::run() {
    while (isAlive()) { // thread is allowed to execute
      mainRun();
    }
  }

  void CEventChainExecutionThread::onAliveChanged(bool paNewValue) {
    if (!paNewValue) {
      resumeSelfSuspend();
    }
  }

  void CEventChainExecutionThread::mainRun() {
    if (externalEventOccured()) {
      transferExternalEvents();
    }
    TEventEntry *event = mEventList.pop();
    if (nullptr == event) {
      mProcessingEvents = false;
      selfSuspend();
      mProcessingEvents = true; // set this flag here to true as well in case the suspend just went through and
                                // processing was not finished
    } else {
      event->getFB().receiveInputEvent(event->getPortId(), this);
    }
  }

  void CEventChainExecutionThread::clear() {
    mEventList.clear();

    {
      util::CCriticalRegion criticalRegion(mExternalEventListSync);
      mExternalEventList.clear();
    }
  }

  void CEventChainExecutionThread::transferExternalEvents() {
    util::CCriticalRegion criticalRegion(mExternalEventListSync);
    // this while is built in a way that it checks also if we got here by accident
    while (!mExternalEventList.isEmpty()) {
      addEventEntry(*mExternalEventList.pop());
    }
  }

  void CEventChainExecutionThread::startEventChain(TEventEntry paEventToAdd) {
    FORTE_TRACE("CEventChainExecutionThread::startEventChain\n");
    bool inserted;
    {
      util::CCriticalRegion criticalRegion(mExternalEventListSync);
      inserted = mExternalEventList.push(paEventToAdd);
    } // End critical region
    if (inserted) {
      mProcessingEvents = true;
      resumeSelfSuspend();
    } else {
      DEVLOG_ERROR("External event queue is full, external event dropped! Function Block: %s\n",
                   paEventToAdd.getFB().getFullQualifiedApplicationInstanceName('.').c_str());
    }
  }

  void CEventChainExecutionThread::changeExecutionState(EMGMCommandType paCommand) {
    switch (paCommand) {
      case EMGMCommandType::Start:
        if (!isAlive()) {
          // only start the thread when we are not already running
          start();
        }
        break;
      case EMGMCommandType::Kill: clear(); [[fallthrough]];
      case EMGMCommandType::Stop: stop(); break;
      default: break;
    }
  }

  void CEventChainExecutionThread::logFullEventQueue(TEventEntry paEventToAdd) {
    const SFBInterfaceSpec &interfaceSpec = paEventToAdd.getFB().getFBInterfaceSpec();
    if (paEventToAdd.getPortId() >= interfaceSpec.getNumEIs()) {
      DEVLOG_ERROR("Event queue is full, event dropped! Function Block: %s, event num: %d (maxnum: %d)\n",
                   paEventToAdd.getFB().getFullQualifiedApplicationInstanceName('.').c_str(), paEventToAdd.getPortId(),
                   interfaceSpec.getNumEIs() - 1);
    } else {
      DEVLOG_ERROR("Event queue is full, event dropped! Function Block: %s, event: %s\n",
                   paEventToAdd.getFB().getFullQualifiedApplicationInstanceName('.').c_str(),
                   interfaceSpec.getEINameId(paEventToAdd.getPortId()).data());
    }
  }

} // namespace forte
