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

#ifndef _TEST_CONDITION_H_
#define _TEST_CONDITION_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_sync.h>

class FORTE_TEST_CONDITION: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_TEST_CONDITION)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &check() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  };

  static const TEventID scm_nEventREQID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventCNFID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

    static unsigned int smExecutedTests;
    static unsigned int smFailedTests;

    static bool smfinalReportPrinted;


   FORTE_FB_DATA_ARRAY(1, 1, 0, 0);

  void executeEvent(int pa_nEIID);

  CSyncObject mFinalReportMutex;

public:
  FUNCTION_BLOCK_CTOR(FORTE_TEST_CONDITION){
  };

  virtual ~FORTE_TEST_CONDITION();

};

#endif //close the ifdef sequence from the beginning of the file

