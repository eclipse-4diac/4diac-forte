/*******************************************************************************
 * Copyright (c) 2006 - 2014 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#pragma once

#include "forte/iec61499/events/timedfb.h"

/*! \brief Implementation of the E_DELAY FB.
 *
 * The E_DELAY block will delay the event propagation of the ec. In the current implementation it is not
 * a busy wait. Instead it is implemented like an event source. The main difference to an event source
 * is that it will not start in an own new ecexectuionthread (or background trhead of the resource). It
 * will use the EC-Thread of the EC that sent the start command.
 */
namespace forte::iec61499::events {
  class FORTE_E_DELAY : public CTimedFB {
      DECLARE_FIRMWARE_FB(FORTE_E_DELAY)
    private:
      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    public:
      FORTE_E_DELAY(const StringId paInstanceNameId, CFBContainer &paContainer);
      ~FORTE_E_DELAY() override = default;
  };

} // namespace forte::iec61499::events
