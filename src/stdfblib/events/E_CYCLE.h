/*******************************************************************************
 * Copyright (c) 2005 - 2014 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_CYCLE_H_
#define _E_CYCLE_H_

#ifndef FMU

#include "../timedfb.h"

/*! \brief Implementation of the E_CYCLE FB.
 */
class E_CYCLE : public CTimedFB{
  DECLARE_FIRMWARE_FB(E_CYCLE)
private:
public:
	E_CYCLE(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : CTimedFB( pa_nInstanceNameId, pa_poSrcRes){
    m_stTimeListEntry.m_eType = e_Periodic;
  };
	virtual ~E_CYCLE() {};
};

#else

#include <cfb.h>
#include <typelib.h>
#include <forte_time.h>

class E_CYCLE: public CCompositeFB{
  DECLARE_FIRMWARE_FB(E_CYCLE)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_TIME &DT() {
    return *static_cast<CIEC_TIME*>(getDI(0));
  };

  static const TEventID scm_nEventSTARTID = 0;
  static const TEventID scm_nEventSTOPID = 1;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventEOID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(1, 1, 0, 0);

  static const SCFB_FBInstanceData scm_astInternalFBs[];

  static const SCFB_FBConnectionData scm_astEventConnections[];

  static const SCFB_FBFannedOutConnectionData scm_astFannedOutEventConnections[];

  static const SCFB_FBConnectionData scm_astDataConnections[];
  static const SCFB_FBNData scm_stFBNData;

public:
  COMPOSITE_FUNCTION_BLOCK_CTOR(E_CYCLE){
  };

  virtual ~E_CYCLE(){};

};

#endif

#endif /*E_CYCLE_H_*/
