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

#ifndef _REGISTERSERVICE_H_
#define _REGISTERSERVICE_H_

#include <cfb.h>
#include <typelib.h>
#include <forte_serviceregistryentry.h>
#include <forte_wstring.h>
#include "RegisterServiceAdp.h"

class FORTE_RegisterService: public CCompositeFB{
  DECLARE_FIRMWARE_FB(FORTE_RegisterService)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_ServiceRegistryEntry &serviceRegistryEntry() {
    return *static_cast<CIEC_ServiceRegistryEntry*>(getDI(0));
  };

  CIEC_WSTRING &endpoint() {
    return *static_cast<CIEC_WSTRING*>(getDI(1));
  };

  static const TEventID scm_nEventregisterID = 0;
  static const TEventID scm_nEventunregisterID = 1;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventdoneRegisterID = 0;
  static const TEventID scm_nEventdoneUnregisterID = 1;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SAdapterInstanceDef scm_astAdapterInstances[];

  FORTE_RegisterServiceAdp& registerService() {
    return (*static_cast<FORTE_RegisterServiceAdp*>(m_apoAdapters[0]));
  };
  static const int scm_nregisterServiceAdpNum = 0;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(2, 2, 0, 1);

  static const SCFB_FBConnectionData scm_astEventConnections[];

  static const SCFB_FBConnectionData scm_astDataConnections[];
  static const SCFB_FBNData scm_stFBNData;

public:
  COMPOSITE_FUNCTION_BLOCK_CTOR(FORTE_RegisterService){
  };

  virtual ~FORTE_RegisterService(){};

};

#endif //close the ifdef sequence from the beginning of the file

