/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _ORCHESTRATORREQUESTADP_H_
#define _ORCHESTRATORREQUESTADP_H_

#include <adapter.h>
#include <typelib.h>
#include <forte_orchestrationform.h>
#include <forte_servicerequestform.h>
#include <forte_wstring.h>
#include <forte_array.h>

class FORTE_OrchestratorRequestAdp: public CAdapter{
  DECLARE_ADAPTER_TYPE(FORTE_OrchestratorRequestAdp)

private:
 private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
 public:
  CIEC_OrchestrationForm *orchestrationResponse() {
    return (CIEC_OrchestrationForm*)(*static_cast<CIEC_ARRAY *>((isSocket()) ? getDI(0) : getDO(0)))[0]; //the first element marks the start of the array
  };

  CIEC_ARRAY &orchestrationResponse_Array() {
    return *static_cast<CIEC_ARRAY*>((isSocket()) ? getDI(0) : getDO(0));
  };

 private:
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
 public:
  CIEC_ServiceRequestForm &serviceRequestForm() {
    return *static_cast<CIEC_ServiceRequestForm*>((isSocket()) ? getDO(0) : getDI(0));
  };

  CIEC_WSTRING &endpoint() {
    return *static_cast<CIEC_WSTRING*>((isSocket()) ? getDO(1) : getDI(1));
  };

 public:
  static const TEventID scm_nEventresponseReceivedID = 0;
  int responseReceived() {
    return m_nParentAdapterListEventID + scm_nEventresponseReceivedID;
  }
 private:
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

 public:
  static const TEventID scm_nEventrequestOrchestatorID = 0;
  int requestOrchestator() {
    return m_nParentAdapterListEventID + scm_nEventrequestOrchestatorID;
  }
 private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;

  static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

   FORTE_ADAPTER_DATA_ARRAY(1, 1, 1, 2, 0);

public:
  ADAPTER_CTOR(FORTE_OrchestratorRequestAdp){
  };

  virtual ~FORTE_OrchestratorRequestAdp(){};

};

#endif //close the ifdef sequence from the beginning of the file

