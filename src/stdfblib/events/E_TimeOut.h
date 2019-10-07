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
#ifndef _E_TIMEOUT_H_
#define _E_TIMEOUT_H_

#include <esfb.h>
#include <typelib.h>
#include "../arch/timerha.h"
#include "ATimeOut.h"

// cppcheck-suppress noConstructor
class FORTE_E_TimeOut : public CEventSourceFB{
  DECLARE_FIRMWARE_FB(FORTE_E_TimeOut)

  private:
    static const TForteInt16 scm_anEOWithIndexes[];
    static const SAdapterInstanceDef scm_astAdapterInstances[];

    FORTE_ATimeOut& TimeOutSocket(){
      return (*static_cast<FORTE_ATimeOut*>(m_apoAdapters[0]));
    }
    ;
    static const int scm_nTimeOutSocketAdpNum = 0;
    static const SFBInterfaceSpec scm_stFBInterfaceSpec;

    FORTE_FB_DATA_ARRAY(0, 0, 0, 1)
    ;

    bool mActive; //!> flag to indicate that the timed fb is currently active
    STimedFBListEntry mTimeListEntry; //!> The Timer list entry of this timed FB

    virtual void executeEvent(int pa_nEIID);

  public:
    EVENT_SOURCE_FUNCTION_BLOCK_CTOR(FORTE_E_TimeOut){
      mActive = false;
      mTimeListEntry.mTimeOut = 0;
      mTimeListEntry.mInterval = 0;
      mTimeListEntry.mNext = 0;
      mTimeListEntry.mType = e_SingleShot;
      mTimeListEntry.mTimedFB = this;
    };

  virtual ~FORTE_E_TimeOut(){};

  virtual EMGMResponse changeFBExecutionState(EMGMCommandType pa_unCommand);

};

#endif //_E_TIMEOUT_H_
