/*******************************************************************************
 * Copyright (c) 2019, 2023 fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *   Martin Jobst - add readInputData and writeOutputData
 *******************************************************************************/

#ifndef _TEST_CONDITION_H_
#define _TEST_CONDITION_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_sync.h>

class FORTE_TEST_CONDITION: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_TEST_CONDITION)

private:
  static const CStringDictionary::TStringId scmDataInputNames[];
  static const CStringDictionary::TStringId scmDataInputTypeIds[];
  CIEC_BOOL &check() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  };

  static const TEventID scmEventREQID = 0;
  static const TForteInt16 scmEIWithIndexes[];
  static const TDataIOID scmEIWith[];
  static const CStringDictionary::TStringId scmEventInputNames[];

  static const TEventID scmEventCNFID = 0;
  static const CStringDictionary::TStringId scmEventOutputNames[];

  static const SFBInterfaceSpec scmFBInterfaceSpec;

    static unsigned int smExecutedTests;
    static unsigned int smFailedTests;

    static bool smfinalReportPrinted;



  void executeEvent(TEventID paEIID) override;

  void readInputData(TEventID paEI) override;
  void writeOutputData(TEventID paEO) override;

  CSyncObject mFinalReportMutex;

public:
  FUNCTION_BLOCK_CTOR(FORTE_TEST_CONDITION){
  };

  ~FORTE_TEST_CONDITION() override;

};

#endif //close the ifdef sequence from the beginning of the file

