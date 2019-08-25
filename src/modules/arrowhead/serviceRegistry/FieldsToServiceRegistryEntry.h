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

#ifndef _FIELDSTOSERVICEREGISTRYENTRY_H_
#define _FIELDSTOSERVICEREGISTRYENTRY_H_

#include <basicfb.h>
#include <forte_arrowheadservice.h>
#include <forte_bool.h>
#include <forte_arrowheadsystem.h>
#include <forte_dint.h>
#include <forte_serviceregistryentry.h>
#include <forte_wstring.h>

class FORTE_FieldsToServiceRegistryEntry: public CBasicFB{
  DECLARE_FIRMWARE_FB(FORTE_FieldsToServiceRegistryEntry)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_ArrowheadService &providedService() {
    return *static_cast<CIEC_ArrowheadService*>(getDI(0));
  };

  CIEC_ArrowheadSystem &provider() {
    return *static_cast<CIEC_ArrowheadSystem*>(getDI(1));
  };

  CIEC_WSTRING &serviceURI() {
    return *static_cast<CIEC_WSTRING*>(getDI(2));
  };

  CIEC_DINT &version() {
    return *static_cast<CIEC_DINT*>(getDI(3));
  };

  CIEC_BOOL &udp() {
    return *static_cast<CIEC_BOOL*>(getDI(4));
  };

  CIEC_DINT &ttl() {
    return *static_cast<CIEC_DINT*>(getDI(5));
  };

  CIEC_WSTRING &metadata() {
    return *static_cast<CIEC_WSTRING*>(getDI(6));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_ServiceRegistryEntry &serviceRegistryEntry() {
    return *static_cast<CIEC_ServiceRegistryEntry*>(getDO(0));
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

   FORTE_BASIC_FB_DATA_ARRAY(1, 7, 1, 0, 0);
  void alg_REQ(void);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateREQ = 1;

  void enterStateSTART(void);
  void enterStateREQ(void);

  virtual void executeEvent(int pa_nEIID);

public:
  FORTE_FieldsToServiceRegistryEntry(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : 
       CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
              0, m_anFBConnData, m_anFBVarsData){
  };

  virtual ~FORTE_FieldsToServiceRegistryEntry(){};

};

#endif //close the ifdef sequence from the beginning of the file

