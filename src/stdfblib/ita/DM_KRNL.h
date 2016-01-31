/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Gerhard Ebenhofer,
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _DM_KRNL_H_
#define _DM_KRNL_H_

#include <cfb.h>
#include <forte_wstring.h>
#include <forte_bool.h>

class DM_KRNL: public CCompositeFB{
  DECLARE_FIRMWARE_FB(DM_KRNL)

private:
  static const CStringDictionary::TStringId scm_aunDataInputNameIds[];
  static const CStringDictionary::TStringId scm_aunDataOutputNameIds[];
  static const CStringDictionary::TStringId scm_aunDataTypeIds[];

  static const TEventID scm_nEventINITID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_aunEventInputNameIds[];

  static const TEventID scm_nEventINITOID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_aunEventOutputNameIds[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_FB_DATA_ARRAY(1,2,2, 0);

  static const SCFB_FBInstanceData scm_astInternalFBs[];
  static const SCFB_FBConnectionData scm_astEventConnections[];
  static const SCFB_FBConnectionData scm_astDataConnections[];
  static const SCFB_FBNData scm_stFBNData;



public:
  COMPOSITE_FUNCTION_BLOCK_CTOR(DM_KRNL){};
  virtual ~DM_KRNL();

};

#endif //close the ifdef sequence from the beginning of the file

