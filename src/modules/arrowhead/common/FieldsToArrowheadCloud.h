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

#ifndef _FIELDSTOARROWHEADCLOUD_H_
#define _FIELDSTOARROWHEADCLOUD_H_

#include <basicfb.h>
#include <forte_bool.h>
#include <forte_arrowheadcloud.h>
#include <forte_dint.h>
#include <forte_wstring.h>

class FORTE_FieldsToArrowheadCloud: public CBasicFB{
  DECLARE_FIRMWARE_FB(FORTE_FieldsToArrowheadCloud)

private:
  static const CStringDictionary::TStringId scmDataInputNames[];
  static const CStringDictionary::TStringId scmDataInputTypeIds[];
  CIEC_WSTRING &Operator() {
    return *static_cast<CIEC_WSTRING*>(getDI(0));
  };

  CIEC_WSTRING &cloudName() {
    return *static_cast<CIEC_WSTRING*>(getDI(1));
  };

  CIEC_WSTRING &address() {
    return *static_cast<CIEC_WSTRING*>(getDI(2));
  };

  CIEC_DINT &port() {
    return *static_cast<CIEC_DINT*>(getDI(3));
  };

  CIEC_WSTRING &gatekeeperServiceURI() {
    return *static_cast<CIEC_WSTRING*>(getDI(4));
  };

  CIEC_WSTRING &authenticationInfo() {
    return *static_cast<CIEC_WSTRING*>(getDI(5));
  };

  CIEC_BOOL &secure() {
    return *static_cast<CIEC_BOOL*>(getDI(6));
  };

  static const CStringDictionary::TStringId scmDataOutputNames[];
  static const CStringDictionary::TStringId scmDataOutputTypeIds[];
  CIEC_ArrowheadCloud &arrowheadCloud() {
    return *static_cast<CIEC_ArrowheadCloud*>(getDO(0));
  };

  static const TEventID scmEventREQID = 0;
  static const TForteInt16 scmEIWithIndexes[];
  static const TDataIOID scmEIWith[];
  static const CStringDictionary::TStringId scmEventInputNames[];

  static const TEventID scmEventCNFID = 0;
  static const TForteInt16 scmEOWithIndexes[];
  static const TDataIOID scmEOWith[];
  static const CStringDictionary::TStringId scmEventOutputNames[];

  static const SFBInterfaceSpec scmFBInterfaceSpec;


  void alg_REQ();
  static const TForteInt16 scmStateSTART = 0;
  static const TForteInt16 scmStateREQ = 1;

  void enterStateSTART(CEventChainExecutionThread *const paECET);
  void enterStateREQ(CEventChainExecutionThread *const paECET);

  virtual void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

public:
  FORTE_FieldsToArrowheadCloud(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) : 
       CBasicFB(paContainer, scmFBInterfaceSpec, paInstanceNameId,
              0){
  };

  ~FORTE_FieldsToArrowheadCloud() override = default;

};

#endif //close the ifdef sequence from the beginning of the file

