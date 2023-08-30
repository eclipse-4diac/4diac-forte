/*******************************************************************************
 * Copyright (c) 2005 - 2014 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Rene Smodic, Gerhard Ebenhofer, Martin Melik Merkumians,
 *   Matthias Plasch
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_RESTART_H_
#define _E_RESTART_H_

#include "../core/esfb.h"
#include "../core/resource.h"
#include <forte_sem.h>

/*! \brief Implementation of the E_RESTART FB.
 */
// cppcheck-suppress noConstructor
class E_RESTART : public CEventSourceFB{
  DECLARE_FIRMWARE_FB(E_RESTART)
private:
  static const SFBInterfaceSpec scmFBInterfaceSpec;


  static const TEventID csmCOLDID = 0;
  static const TEventID csmWARMID = 1;
  static const TEventID csmSTOPID = 2;

  static const CStringDictionary::TStringId scmEONameIds[];

  // semaphore to ensure proper handling of STOP execution state change
  forte::arch::CSemaphore mSuspendSemaphore;

  TEventID mEventToSend;

  void executeEvent(TEventID paEIID) override;

  void readInputData(TEventID paEI) override;
  void writeOutputData(TEventID paEO) override;

public:
  EVENT_SOURCE_FUNCTION_BLOCK_CTOR(E_RESTART),
        mEventToSend(cgInvalidEventID) {
    setEventChainExecutor(paSrcRes->getResourceEventExecution());
  }
  ~E_RESTART() override = default;

  EMGMResponse changeFBExecutionState(EMGMCommandType paCommand) override;
};

#endif /*E_RESTART_H_*/
