/*******************************************************************************
 * Copyright (c) 2013, 2023 Profactor GmbH, fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Michael Hofmann, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst - add readInputData and writeOutputData
 *******************************************************************************/
#ifndef _STEST_END_H_
#define _STEST_END_H_

#include <funcbloc.h>
#include <mgmcmdstruct.h>

class FORTE_STEST_END: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_STEST_END)

private:
  static const TEventID scmEventREQID = 0;
  static const TForteInt16 scmEIWithIndexes[];
  static const CStringDictionary::TStringId scmEventInputNames[];

  static const TForteInt16 scmEOWithIndexes[];
  static const SFBInterfaceSpec scmFBInterfaceSpec;


  void executeEvent(TEventID paEIID) override;

  void readInputData(TEventID) override {
  }

  void writeOutputData(TEventID) override {
  }

public:
  FUNCTION_BLOCK_CTOR(FORTE_STEST_END){
  };

  ~FORTE_STEST_END() override = default;

};

#endif //close the ifdef sequence from the beginning of the file

