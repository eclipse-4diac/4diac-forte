/*******************************************************************************
 * Copyright (c) 2005 - 2013 ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte/extevhan.h"
#include "forte/devexec.h"
#include "forte/util/devlog.h"

namespace forte {
  constinit std::vector<std::function<std::unique_ptr<CExternalEventHandler>(CDeviceExecution &)>>
      CExternalEventHandler::smEventHandlerFactories;

  CExternalEventHandler::CExternalEventHandler(CDeviceExecution &paDeviceExecution) :
      mDeviceExecution(paDeviceExecution) {
  }

  void CExternalEventHandler::startNewEventChain(CEventSourceFB *paECStartFB) {
    FORTE_TRACE("Starting EC\n");
    mDeviceExecution.startNewEventChain(paECStartFB);
  }
} // namespace forte
