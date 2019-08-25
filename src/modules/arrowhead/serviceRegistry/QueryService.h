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

#ifndef _QUERYSERVICE_H_
#define _QUERYSERVICE_H_

#include <cfb.h>
#include <typelib.h>
#include <forte_servicequeryform.h>
#include <forte_serviceregistryentry.h>
#include <forte_wstring.h>
#include <forte_array.h>
#include "QueryServicesAdp.h"

class FORTE_QueryService: public CCompositeFB{
  DECLARE_FIRMWARE_FB(FORTE_QueryService)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_ServiceQueryForm &serviceQueryForm() {
    return *static_cast<CIEC_ServiceQueryForm*>(getDI(0));
  };

  CIEC_WSTRING &address() {
    return *static_cast<CIEC_WSTRING*>(getDI(1));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_ServiceRegistryEntry *serviceEntries() {
    return (CIEC_ServiceRegistryEntry*)(*static_cast<CIEC_ARRAY *>(getDO(0)))[0]; //the first element marks the start of the array
  };

  CIEC_ARRAY &serviceEntries_Array() {
    return *static_cast<CIEC_ARRAY*>(getDO(0));
  };

  static const TEventID scm_nEventqueryID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventqueriedID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SAdapterInstanceDef scm_astAdapterInstances[];

  FORTE_QueryServicesAdp& queryServices() {
    return (*static_cast<FORTE_QueryServicesAdp*>(m_apoAdapters[0]));
  };
  static const int scm_nqueryServicesAdpNum = 0;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(1, 2, 1, 1);

  static const SCFB_FBConnectionData scm_astEventConnections[];

  static const SCFB_FBConnectionData scm_astDataConnections[];
  static const SCFB_FBNData scm_stFBNData;

public:
  COMPOSITE_FUNCTION_BLOCK_CTOR(FORTE_QueryService){
  };

  virtual ~FORTE_QueryService(){};

};

#endif //close the ifdef sequence from the beginning of the file

