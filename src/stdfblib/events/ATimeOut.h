/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _ATIMEOUT_H_
#define _ATIMEOUT_H_

#include <adapter.h>
#include <typelib.h>

class FORTE_ATimeOut: public CAdapter{
  DECLARE_ADAPTER_TYPE(FORTE_ATimeOut)

private:
 private:
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
 public:
  CIEC_TIME &DT() {
    return *static_cast<CIEC_TIME*>((isSocket()) ? getDO(0) : getDI(0));
  };

 public:
  static const TEventID scm_nEventTimeOutID = 0;
    int TimeOut() const {
    return m_nParentAdapterListEventID + scm_nEventTimeOutID;
  }
 private:
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

 public:
  static const TEventID scm_nEventSTARTID = 0;
    int START() const {
    return m_nParentAdapterListEventID + scm_nEventSTARTID;
  }
  static const TEventID scm_nEventSTOPID = 1;
    int STOP() const {
    return m_nParentAdapterListEventID + scm_nEventSTOPID;
  }
 private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;

  static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

   FORTE_ADAPTER_DATA_ARRAY(1, 2, 0, 1, 0);

public:
  ADAPTER_CTOR(FORTE_ATimeOut){
  };

  virtual ~FORTE_ATimeOut(){};

};

#endif //close the ifdef sequence from the beginning of the file

