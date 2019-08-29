/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _REGISTERSERVICEHTTP_H_
#define _REGISTERSERVICEHTTP_H_

#include <cfb.h>
#include <typelib.h>
#include "RegisterServiceAdp.h"

class FORTE_RegisterServiceHTTP: public CCompositeFB{
  DECLARE_FIRMWARE_FB(FORTE_RegisterServiceHTTP)

private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const SAdapterInstanceDef scm_astAdapterInstances[];

  FORTE_RegisterServiceAdp& registerService() {
    return (*static_cast<FORTE_RegisterServiceAdp*>(m_apoAdapters[0]));
  };
  static const int scm_nregisterServiceAdpNum = 0;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(0, 0, 0, 1);

  static const SCFB_FBInstanceData scm_astInternalFBs[];

  static const SCFB_FBParameter scm_astParamters[];

  static const SCFB_FBConnectionData scm_astEventConnections[];

  static const SCFB_FBFannedOutConnectionData scm_astFannedOutEventConnections[];

  static const SCFB_FBConnectionData scm_astDataConnections[];

  static const SCFB_FBFannedOutConnectionData scm_astFannedOutDataConnections[];
  static const SCFB_FBNData scm_stFBNData;

public:
  COMPOSITE_FUNCTION_BLOCK_CTOR(FORTE_RegisterServiceHTTP){
  };

  virtual ~FORTE_RegisterServiceHTTP(){};

};

#endif //close the ifdef sequence from the beginning of the file

