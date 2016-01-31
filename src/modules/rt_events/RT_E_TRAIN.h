/*******************************************************************************
 * Copyright (c) 2006 - 2014 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _RT_E_TRAIN_H_
#define _RT_E_TRAIN_H_

#include <cfb.h>
#include <typelib.h>
#include <forte_time.h>
#include <forte_uint.h>

class FORTE_RT_E_TRAIN: public CCompositeFB{
  DECLARE_FIRMWARE_FB(FORTE_RT_E_TRAIN)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_TIME &DT() {
    return *static_cast<CIEC_TIME*>(getDI(0));
  };

  CIEC_UINT &N() {
    return *static_cast<CIEC_UINT*>(getDI(1));
  };

  CIEC_TIME &Deadline() {
    return *static_cast<CIEC_TIME*>(getDI(2));
  };

  CIEC_TIME &WCET() {
    return *static_cast<CIEC_TIME*>(getDI(3));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_UINT &CV() {
    return *static_cast<CIEC_UINT*>(getDO(0));
  };

  static const TEventID scm_nEventSTARTID = 0;
  static const TEventID scm_nEventSTOPID = 1;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventEOID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(1, 4, 1, 0);

  static const SCFB_FBInstanceData scm_astInternalFBs[];

  static const SCFB_FBConnectionData scm_astEventConnections[];

  static const SCFB_FBConnectionData scm_astDataConnections[];
  static const SCFB_FBNData scm_stFBNData;

public:
  COMPOSITE_FUNCTION_BLOCK_CTOR(FORTE_RT_E_TRAIN){
  };

  virtual ~FORTE_RT_E_TRAIN(){};

};

#endif //close the ifdef sequence from the beginning of the file

