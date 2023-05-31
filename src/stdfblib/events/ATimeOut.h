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

#include "adapter.h"
#include "typelib.h"
#include "forte_time.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_ATimeOut: public CAdapter {
  DECLARE_ADAPTER_TYPE(FORTE_ATimeOut)

private:
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  public:
    static const TEventID scm_nEventTimeOutID = 0;
  
  private:
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];
  public:
    static const TEventID scm_nEventSTARTID = 0;
    static const TEventID scm_nEventSTOPID = 1;
  
  private:
  static const TDataIOID scm_anEOWith[]; 
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;
  
  static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

public:
  CIEC_TIME &var_DT() {
    return *static_cast<CIEC_TIME*>((isSocket()) ? getDO(0) : getDI(0));
  }
  

  TEventID evt_TimeOut() {
    return m_nParentAdapterListEventID + scm_nEventTimeOutID;
  }
  
  TEventID evt_START() {
    return m_nParentAdapterListEventID + scm_nEventSTARTID;
  }
  
  TEventID evt_STOP() {
    return m_nParentAdapterListEventID + scm_nEventSTOPID;
  }
  

public:
  FORTE_ATimeOut(CStringDictionary::TStringId pa_anAdapterInstanceName, CResource *pa_poSrcRes, bool pa_bIsPlug) :
      CAdapter(pa_poSrcRes, &scm_stFBInterfaceSpecSocket, pa_anAdapterInstanceName, &scm_stFBInterfaceSpecPlug, pa_bIsPlug) {
   };

  virtual ~FORTE_ATimeOut() = default;
};



