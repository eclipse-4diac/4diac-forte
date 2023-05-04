/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
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


class FORTE_ARTimeOut: public CAdapter {
  DECLARE_ADAPTER_TYPE(FORTE_ARTimeOut)

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
  

  int evt_TimeOut() {
    return m_nParentAdapterListEventID + scm_nEventTimeOutID;
  }
  
  int evt_START() {
    return m_nParentAdapterListEventID + scm_nEventSTARTID;
  }
  
  int evt_STOP() {
    return m_nParentAdapterListEventID + scm_nEventSTOPID;
  }
  

private:
  FORTE_ADAPTER_DATA_ARRAY(1, 2, 0, 1, 0);

public:
  FORTE_ARTimeOut(CStringDictionary::TStringId pa_anAdapterInstanceName, CResource *pa_poSrcRes, bool pa_bIsPlug) :
      CAdapter(pa_poSrcRes, &scm_stFBInterfaceSpecSocket, pa_anAdapterInstanceName, &scm_stFBInterfaceSpecPlug, pa_bIsPlug, m_anFBConnData, m_anFBVarsData) {	
   };

  virtual ~FORTE_ARTimeOut() = default;
};



