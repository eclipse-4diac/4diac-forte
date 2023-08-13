/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _LOCALIZEDTEXT2LOCALIZEDTEXT_H_
#define _LOCALIZEDTEXT2LOCALIZEDTEXT_H_

#include <funcbloc.h>
#include <forte_localizedtext.h>

class FORTE_LocalizedText2LocalizedText: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_LocalizedText2LocalizedText)

private:
  static const CStringDictionary::TStringId scmDataInputNames[];
  static const CStringDictionary::TStringId scmDataInputTypeIds[];
  CIEC_LocalizedText &st_IN() {
    return *static_cast<CIEC_LocalizedText*>(getDI(0));
  };

  static const CStringDictionary::TStringId scmDataOutputNames[];
  static const CStringDictionary::TStringId scmDataOutputTypeIds[];
  CIEC_LocalizedText &st_OUT() {
    return *static_cast<CIEC_LocalizedText*>(getDO(0));
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


  void executeEvent(TEventID paEIID) override;

  void readInputData(TEventID paEIID) override;
  void writeOutputData(TEventID paEIID) override;

public:
  FUNCTION_BLOCK_CTOR(FORTE_LocalizedText2LocalizedText){
  };

  ~FORTE_LocalizedText2LocalizedText() override = default;

};

#endif //close the ifdef sequence from the beginning of the file

