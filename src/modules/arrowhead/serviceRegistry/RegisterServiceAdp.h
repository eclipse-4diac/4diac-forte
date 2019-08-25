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

#ifndef _REGISTERSERVICEADP_H_
#define _REGISTERSERVICEADP_H_

#include <adapter.h>
#include <typelib.h>
#include <forte_serviceregistryentry.h>
#include <forte_wstring.h>

class FORTE_RegisterServiceAdp: public CAdapter{
  DECLARE_ADAPTER_TYPE(FORTE_RegisterServiceAdp)

private:
 private:
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
 public:
  CIEC_ServiceRegistryEntry &serviceRegistryEntry() {
    return *static_cast<CIEC_ServiceRegistryEntry*>((isSocket()) ? getDO(0) : getDI(0));
  };

  CIEC_WSTRING &endpoint() {
    return *static_cast<CIEC_WSTRING*>((isSocket()) ? getDO(1) : getDI(1));
  };

 public:
  static const TEventID scm_nEventdoneRegisterID = 0;
  int doneRegister() {
    return m_nParentAdapterListEventID + scm_nEventdoneRegisterID;
  }
  static const TEventID scm_nEventdoneUnregisterID = 1;
  int doneUnregister() {
    return m_nParentAdapterListEventID + scm_nEventdoneUnregisterID;
  }
 private:
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

 public:
  static const TEventID scm_nEventregisterServiceID = 0;
  int registerService() {
    return m_nParentAdapterListEventID + scm_nEventregisterServiceID;
  }
  static const TEventID scm_nEventunregisterServiceID = 1;
  int unregisterService() {
    return m_nParentAdapterListEventID + scm_nEventunregisterServiceID;
  }
 private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;

  static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

   FORTE_ADAPTER_DATA_ARRAY(2, 2, 0, 2, 0);

public:
  ADAPTER_CTOR(FORTE_RegisterServiceAdp){
  };

  virtual ~FORTE_RegisterServiceAdp(){};

};

#endif //close the ifdef sequence from the beginning of the file

