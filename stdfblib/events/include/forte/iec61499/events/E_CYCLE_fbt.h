/*******************************************************************************
 * Copyright (c) 2005, 2023 ACIN, Profactor GmbH, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *     - initial API and implementation and/or initial documentation
 *   Alois Zoitl  - Reworked to new timer handler interface
 *******************************************************************************/
#ifndef _E_CYCLE_H_
#define _E_CYCLE_H_

#include "forte/iec61499/events/timedfb.h"

/*! \brief Implementation of the E_CYCLE FB.
 */
namespace forte::iec61499::events {
  class FORTE_E_CYCLE : public CTimedFB {
      DECLARE_FIRMWARE_FB(FORTE_E_CYCLE)
    public:
      FORTE_E_CYCLE(const forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :
          CTimedFB(paInstanceNameId, paContainer) {
      }

      ~FORTE_E_CYCLE() override = default;

    private:
      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;
  };
} // namespace forte::iec61499::events

#endif /*E_CYCLE_H_*/
