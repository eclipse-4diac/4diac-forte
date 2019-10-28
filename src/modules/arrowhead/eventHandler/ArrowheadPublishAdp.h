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

#ifndef _ARROWHEADPUBLISHADP_H_
#define _ARROWHEADPUBLISHADP_H_

#include <adapter.h>
#include <typelib.h>
#include <forte_publishevent.h>
#include <forte_wstring.h>

class FORTE_ArrowheadPublishAdp: public CAdapter{
  DECLARE_ADAPTER_TYPE(FORTE_ArrowheadPublishAdp)

private:
 private:
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
 public:
  CIEC_PublishEvent &publishEvent() {
    return *static_cast<CIEC_PublishEvent*>((isSocket()) ? getDO(0) : getDI(0));
  };

  CIEC_WSTRING &endpoint() {
    return *static_cast<CIEC_WSTRING*>((isSocket()) ? getDO(1) : getDI(1));
  };

 public:
  static const TEventID scm_nEventpublishedID = 0;
  int published() {
    return m_nParentAdapterListEventID + scm_nEventpublishedID;
  }
 private:
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

 public:
  static const TEventID scm_nEventpublishID = 0;
  int publish() {
    return m_nParentAdapterListEventID + scm_nEventpublishID;
  }
 private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;

  static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

   FORTE_ADAPTER_DATA_ARRAY(1, 1, 0, 2, 0);

public:
  ADAPTER_CTOR(FORTE_ArrowheadPublishAdp){
  };

  virtual ~FORTE_ArrowheadPublishAdp(){};

};

#endif //close the ifdef sequence from the beginning of the file

