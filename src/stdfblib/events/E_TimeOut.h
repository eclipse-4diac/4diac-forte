/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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

    bool m_bActive; //!> flag to indicate that the timed fb is currently active
    STimedFBListEntry m_stTimeListEntry; //!> The Timer list entry of this timed FB

    virtual void executeEvent(int pa_nEIID);

  public:
    EVENT_SOURCE_FUNCTION_BLOCK_CTOR(FORTE_E_TimeOut){
    m_bActive = false;
    m_stTimeListEntry.m_stTimeOut.m_nLowerValue = 0;
    m_stTimeListEntry.m_stTimeOut.m_nUpperValue = 0;
    m_stTimeListEntry.m_nInterval = 0;
    m_stTimeListEntry.m_pstNext = 0;
    m_stTimeListEntry.m_eType = e_SingleShot;
    m_stTimeListEntry.m_poTimedFB = this;
  };

  virtual ~FORTE_E_TimeOut(){};

  virtual EMGMResponse changeFBExecutionState(EMGMCommandType pa_unCommand);

};

#endif //_E_TIMEOUT_H_
