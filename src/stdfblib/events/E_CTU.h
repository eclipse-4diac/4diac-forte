/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_CTU_H_
#define _E_CTU_H_

#include <funcbloc.h>

class E_CTU : public CFunctionBlock{
  DECLARE_FIRMWARE_FB(E_CTU)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[], scm_aunDIDataTypeIds[];
  static const CStringDictionary::TStringId scm_anDataOutputNames[], scm_aunDODataTypeIds[];

  static const TEventID scm_nEventCUID = 0;
  static const TEventID scm_nEventRID = 1;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventCUOID = 0;
  static const TEventID scm_nEventROID = 1;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];


  virtual void executeEvent(int pa_nEIID);

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_FB_DATA_ARRAY(2,1,2, 0);
  
  CIEC_UINT& PV() {
     return *static_cast<CIEC_UINT*>(getDI(0));
  }
  CIEC_BOOL& Q() {
     return *static_cast<CIEC_BOOL*>(getDO(0));
  }
  CIEC_UINT& CV() {
     return *static_cast<CIEC_UINT*>(getDO(1));
  }


public:
  FUNCTION_BLOCK_CTOR(E_CTU){};
  virtual ~E_CTU(){};

};

#endif //_E_CTU_H_

