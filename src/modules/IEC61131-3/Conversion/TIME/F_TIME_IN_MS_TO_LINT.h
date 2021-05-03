/*******************************************************************************
 * Copyright (c) 2019 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef _F_TIME_IN_MS_TO_LINT_H_
#define _F_TIME_IN_MS_TO_LINT_H_

#include <simplefb.h>
#include <forte_lint.h>
#include <forte_time.h>

class FORTE_F_TIME_IN_MS_TO_LINT: public CSimpleFB{
  DECLARE_FIRMWARE_FB(FORTE_F_TIME_IN_MS_TO_LINT)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_TIME &st_IN() {
    return *static_cast<CIEC_TIME*>(getDI(0));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_LINT &st_OUT() {
    return *static_cast<CIEC_LINT*>(getDO(0));
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

   FORTE_FB_DATA_ARRAY(1, 1, 1, 0);
  void alg_REQ(void);

public:
  FORTE_F_TIME_IN_MS_TO_LINT(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : 
       CSimpleFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
              0, m_anFBConnData, m_anFBVarsData){
  };

  virtual ~FORTE_F_TIME_IN_MS_TO_LINT(){};

};

#endif //close the ifdef sequence from the beginning of the file

