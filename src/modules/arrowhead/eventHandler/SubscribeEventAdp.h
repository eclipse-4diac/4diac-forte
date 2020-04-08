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

#ifndef _SUBSCRIBEEVENTADP_H_
#define _SUBSCRIBEEVENTADP_H_

#include <adapter.h>
#include <typelib.h>
#include <forte_eventfilter.h>
#include <forte_wstring.h>

class FORTE_SubscribeEventAdp: public CAdapter{
  DECLARE_ADAPTER_TYPE(FORTE_SubscribeEventAdp)

private:
 private:
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
 public:
  CIEC_EventFilter &eventFilter() {
    return *static_cast<CIEC_EventFilter*>((isSocket()) ? getDO(0) : getDI(0));
  };

  CIEC_WSTRING &endpoint() {
    return *static_cast<CIEC_WSTRING*>((isSocket()) ? getDO(1) : getDI(1));
  };

 public:
  static const TEventID scm_nEventSubscribedID = 0;
  int Subscribed() {
    return m_nParentAdapterListEventID + scm_nEventSubscribedID;
  }
  static const TEventID scm_nEventUnsubscribedID = 1;
  int Unsubscribed() {
    return m_nParentAdapterListEventID + scm_nEventUnsubscribedID;
  }
 private:
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

 public:
  static const TEventID scm_nEventSubscribeID = 0;
  int Subscribe() {
    return m_nParentAdapterListEventID + scm_nEventSubscribeID;
  }
  static const TEventID scm_nEventUnsubscribeID = 1;
  int Unsubscribe() {
    return m_nParentAdapterListEventID + scm_nEventUnsubscribeID;
  }
 private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;

  static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

   FORTE_ADAPTER_DATA_ARRAY(2, 2, 0, 2, 0);

public:
  ADAPTER_CTOR(FORTE_SubscribeEventAdp){
  };

  virtual ~FORTE_SubscribeEventAdp(){};

};

#endif //close the ifdef sequence from the beginning of the file

