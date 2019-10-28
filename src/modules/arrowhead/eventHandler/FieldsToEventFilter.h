/*******************************************************************************
 * Copyright (c) 2019, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _FIELDSTOEVENTFILTER_H_
#define _FIELDSTOEVENTFILTER_H_

#include <basicfb.h>
#include <forte_date_and_time.h>
#include <forte_bool.h>
#include <forte_arrowheadsystem.h>
#include <forte_eventfilter.h>
#include <forte_wstring.h>
#include <forte_array.h>

class FORTE_FieldsToEventFilter: public CBasicFB{
  DECLARE_FIRMWARE_FB(FORTE_FieldsToEventFilter)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_WSTRING &eventType() {
    return *static_cast<CIEC_WSTRING*>(getDI(0));
  };

  CIEC_ArrowheadSystem &consumer() {
    return *static_cast<CIEC_ArrowheadSystem*>(getDI(1));
  };

  CIEC_ArrowheadSystem *sources() {
    return (CIEC_ArrowheadSystem*)(*static_cast<CIEC_ARRAY *>(getDI(2)))[0]; //the first element marks the start of the array
  };

  CIEC_ARRAY &sources_Array() {
    return *static_cast<CIEC_ARRAY*>(getDI(2));
  };

  CIEC_DATE_AND_TIME &startDate() {
    return *static_cast<CIEC_DATE_AND_TIME*>(getDI(3));
  };

  CIEC_DATE_AND_TIME &endDate() {
    return *static_cast<CIEC_DATE_AND_TIME*>(getDI(4));
  };

  CIEC_WSTRING *filterMetadata() {
    return (CIEC_WSTRING*)(*static_cast<CIEC_ARRAY *>(getDI(5)))[0]; //the first element marks the start of the array
  };

  CIEC_ARRAY &filterMetadata_Array() {
    return *static_cast<CIEC_ARRAY*>(getDI(5));
  };

  CIEC_WSTRING &notifyUri() {
    return *static_cast<CIEC_WSTRING*>(getDI(6));
  };

  CIEC_BOOL &matchMetadata() {
    return *static_cast<CIEC_BOOL*>(getDI(7));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_EventFilter &eventFilter() {
    return *static_cast<CIEC_EventFilter*>(getDO(0));
  };

  static const TEventID scm_nEventREQID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventCNFID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_BASIC_FB_DATA_ARRAY(1, 8, 1, 0, 0);
  void alg_REQ(void);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateREQ = 1;

  void enterStateSTART(void);
  void enterStateREQ(void);

  virtual void executeEvent(int pa_nEIID);

public:
  FORTE_FieldsToEventFilter(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : 
       CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
              0, m_anFBConnData, m_anFBVarsData){
  };

  virtual ~FORTE_FieldsToEventFilter(){};

};

#endif //close the ifdef sequence from the beginning of the file

