/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _QUERYSERVICEHTTP_H_
#define _QUERYSERVICEHTTP_H_

#include <cfb.h>
#include <typelib.h>
#include "QueryServicesAdp.h"

class FORTE_QueryServiceHTTP: public CCompositeFB{
  DECLARE_FIRMWARE_FB(FORTE_QueryServiceHTTP)

private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const SAdapterInstanceDef scm_astAdapterInstances[];

  FORTE_QueryServicesAdp& queryServices() {
    return (*static_cast<FORTE_QueryServicesAdp*>(m_apoAdapters[0]));
  };
  static const int scm_nqueryServicesAdpNum = 0;
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
  COMPOSITE_FUNCTION_BLOCK_CTOR(FORTE_QueryServiceHTTP){
  };

  virtual ~FORTE_QueryServiceHTTP(){};

};

#endif //close the ifdef sequence from the beginning of the file

