/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "esfb.h"
#include "typelib.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "ATimeOut.h"

#include "../arch/timerha.h"

// cppcheck-suppress noConstructor
class FORTE_E_TimeOut : public CEventSourceFB{
  DECLARE_FIRMWARE_FB(FORTE_E_TimeOut)

  private:
    static const SAdapterInstanceDef scmAdapterInstances[];

    FORTE_ATimeOut& var_TimeOutSocket(){
      return *static_cast<FORTE_ATimeOut*>(mAdapters[0]);
    }
    ;
    static const int scmTimeOutSocketAdpNum = 0;
    static const SFBInterfaceSpec scmFBInterfaceSpec;

    bool mActive; //!> flag to indicate that the timed fb is currently active
    STimedFBListEntry mTimeListEntry; //!> The Timer list entry of this timed FB

    void executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET) override;

    void readInputData(TEventID paEI) override;
    void writeOutputData(TEventID paEO) override;

  public:
    EVENT_SOURCE_FUNCTION_BLOCK_CTOR(FORTE_E_TimeOut){
      mActive = false;
      mTimeListEntry.mTimeOut = 0;
      mTimeListEntry.mInterval = 0;
      mTimeListEntry.mNext = nullptr;
      mTimeListEntry.mType = e_SingleShot;
      mTimeListEntry.mTimedFB = this;
    };

  ~FORTE_E_TimeOut() override = default;

  EMGMResponse changeFBExecutionState(EMGMCommandType paCommand) override;

};

