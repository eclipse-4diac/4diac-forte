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

#ifndef _FIELDSTOSERVICEREQUESTFORM_H_
#define _FIELDSTOSERVICEREQUESTFORM_H_

#include <basicfb.h>
#include <forte_preferredprovider.h>
#include <forte_arrowheadservice.h>
#include <forte_arrowheadcloud.h>
#include <forte_arrowheadsystem.h>
#include <forte_servicerequestform.h>
#include <forte_wstring.h>
#include <forte_array.h>

class FORTE_FieldsToServiceRequestForm: public CBasicFB{
  DECLARE_FIRMWARE_FB(FORTE_FieldsToServiceRequestForm)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_ArrowheadSystem &requesterSystem() {
    return *static_cast<CIEC_ArrowheadSystem*>(getDI(0));
  };

  CIEC_ArrowheadCloud &requesterCloud() {
    return *static_cast<CIEC_ArrowheadCloud*>(getDI(1));
  };

  CIEC_ArrowheadService &requestedService() {
    return *static_cast<CIEC_ArrowheadService*>(getDI(2));
  };

  CIEC_WSTRING *orchestrationFlags() {
    return (CIEC_WSTRING*)(*static_cast<CIEC_ARRAY *>(getDI(3)))[0]; //the first element marks the start of the array
  };

  CIEC_ARRAY &orchestrationFlags_Array() {
    return *static_cast<CIEC_ARRAY*>(getDI(3));
  };

  CIEC_PreferredProvider *preferredProviders() {
    return (CIEC_PreferredProvider*)(*static_cast<CIEC_ARRAY *>(getDI(4)))[0]; //the first element marks the start of the array
  };

  CIEC_ARRAY &preferredProviders_Array() {
    return *static_cast<CIEC_ARRAY*>(getDI(4));
  };

  CIEC_WSTRING *requestedQoS() {
    return (CIEC_WSTRING*)(*static_cast<CIEC_ARRAY *>(getDI(5)))[0]; //the first element marks the start of the array
  };

  CIEC_ARRAY &requestedQoS_Array() {
    return *static_cast<CIEC_ARRAY*>(getDI(5));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_ServiceRequestForm &serviceRequestForm() {
    return *static_cast<CIEC_ServiceRequestForm*>(getDO(0));
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

   FORTE_BASIC_FB_DATA_ARRAY(1, 6, 1, 0, 0);
  void alg_REQ(void);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateREQ = 1;

  void enterStateSTART(void);
  void enterStateREQ(void);

  virtual void executeEvent(int pa_nEIID);

public:
  FORTE_FieldsToServiceRequestForm(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : 
       CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
              0, m_anFBConnData, m_anFBVarsData){
  };

  virtual ~FORTE_FieldsToServiceRequestForm(){};

};

#endif //close the ifdef sequence from the beginning of the file

