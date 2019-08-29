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

#ifndef _QUERYSERVICESADP_H_
#define _QUERYSERVICESADP_H_

#include <adapter.h>
#include <typelib.h>
#include <forte_servicequeryform.h>
#include <forte_serviceregistryentry.h>
#include <forte_wstring.h>
#include <forte_array.h>

class FORTE_QueryServicesAdp: public CAdapter{
  DECLARE_ADAPTER_TYPE(FORTE_QueryServicesAdp)

private:
 private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
 public:
  CIEC_ServiceRegistryEntry *serviceEntries() {
    return (CIEC_ServiceRegistryEntry*)(*static_cast<CIEC_ARRAY *>((isSocket()) ? getDI(0) : getDO(0)))[0]; //the first element marks the start of the array
  };

  CIEC_ARRAY &serviceEntries_Array() {
    return *static_cast<CIEC_ARRAY*>((isSocket()) ? getDI(0) : getDO(0));
  };

 private:
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
 public:
  CIEC_ServiceQueryForm &serviceQueryForm() {
    return *static_cast<CIEC_ServiceQueryForm*>((isSocket()) ? getDO(0) : getDI(0));
  };

  CIEC_WSTRING &endpoint() {
    return *static_cast<CIEC_WSTRING*>((isSocket()) ? getDO(1) : getDI(1));
  };

 public:
  static const TEventID scm_nEventqueriedID = 0;
  int queried() {
    return m_nParentAdapterListEventID + scm_nEventqueriedID;
  }
 private:
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

 public:
  static const TEventID scm_nEventqueryID = 0;
  int query() {
    return m_nParentAdapterListEventID + scm_nEventqueryID;
  }
 private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;

  static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

   FORTE_ADAPTER_DATA_ARRAY(1, 1, 1, 2, 0);

public:
  ADAPTER_CTOR(FORTE_QueryServicesAdp){
  };

  virtual ~FORTE_QueryServicesAdp(){};

};

#endif //close the ifdef sequence from the beginning of the file

