/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _CFB_TEST_H_
#define _CFB_TEST_H_

#include <cfb.h>
#include <typelib.h>
#include <forte_bool.h>

class FORTE_CFB_TEST: public CCompositeFB{
  DECLARE_FIRMWARE_FB(FORTE_CFB_TEST)

private:
  static const CStringDictionary::TStringId scmDataInputNames[];
  static const CStringDictionary::TStringId scmDataInputTypeIds[];
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  };

  static const CStringDictionary::TStringId scmDataOutputNames[];
  static const CStringDictionary::TStringId scmDataOutputTypeIds[];
  CIEC_BOOL &SR() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  };

  static const TEventID scmEventSETID = 0;
  static const TEventID scmEventRESETID = 1;
  static const TForteInt16 scmEIWithIndexes[];
  static const TDataIOID scmEIWith[];
  static const CStringDictionary::TStringId scmEventInputNames[];

  static const TEventID scmEventCNFID = 0;
  static const TEventID scmEventCHANGEDID = 1;
  static const TForteInt16 scmEOWithIndexes[];
  static const TDataIOID scmEOWith[];
  static const CStringDictionary::TStringId scmEventOutputNames[];

  static const SFBInterfaceSpec scmFBInterfaceSpec;


  static const SCFB_FBInstanceData scmInternalFBs[];

  static const SCFB_FBConnectionData scmEventConnections[];

  static const SCFB_FBFannedOutConnectionData scmFannedOutEventConnections[];

  static const SCFB_FBConnectionData scmDataConnections[];

  static const SCFB_FBFannedOutConnectionData scmFannedOutDataConnections[];
  static const SCFB_FBNData scmFBNData;

  void readInputData(TEventID paEI) override;
  void writeOutputData(TEventID paEO) override;
  void readInternal2InterfaceOutputData(TEventID paEOID) override;

public:
  COMPOSITE_FUNCTION_BLOCK_CTOR(FORTE_CFB_TEST){
  };

  ~FORTE_CFB_TEST() override = default;

};

#endif //close the ifdef sequence from the beginning of the file

