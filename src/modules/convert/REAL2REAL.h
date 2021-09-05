/*******************************************************************************
 * Copyright (c) 2006 - 2011 Profactor GmbH, ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny,
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _REAL2REAL_H_
#define _REAL2REAL_H_

#include <funcbloc.h>

class REAL2REAL: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(REAL2REAL)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[], scm_aunDIDataTypeIds[];
  static const CStringDictionary::TStringId scm_anDataOutputNames[], scm_aunDODataTypeIds[];
  static const TEventID scm_nEventREQID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventCNFID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_FB_DATA_ARRAY(1,1,1, 0);

   virtual void executeEvent(int pa_nEIID);

   CIEC_REAL& st_IN() {
     return *static_cast<CIEC_REAL*>(getDI(0));
   }

   CIEC_REAL& st_OUT() {
     return *static_cast<CIEC_REAL*>(getDO(0));
   }


public:
  FUNCTION_BLOCK_CTOR(REAL2REAL) {};
  virtual ~REAL2REAL();

};

#endif //_REAL2REAL_H_

