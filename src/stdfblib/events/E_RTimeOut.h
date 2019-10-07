/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_RTIMEOUT_H_
#define _E_RTIMEOUT_H_

#include <cfb.h>
#include <typelib.h>
#include "ARTimeOut.h"

class FORTE_E_RTimeOut: public CCompositeFB{
  DECLARE_FIRMWARE_FB(FORTE_E_RTimeOut)

private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const SAdapterInstanceDef scm_astAdapterInstances[];

  FORTE_ARTimeOut& TimeOutSocket() {
    return (*static_cast<FORTE_ARTimeOut*>(m_apoAdapters[0]));
  };
  static const int scm_nTimeOutSocketAdpNum = 0;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(0, 0, 0, 1);

  static const SCFB_FBInstanceData scm_astInternalFBs[];

  static const SCFB_FBConnectionData scm_astEventConnections[];

  static const SCFB_FBConnectionData scm_astDataConnections[];
  static const SCFB_FBNData scm_stFBNData;

public:
  COMPOSITE_FUNCTION_BLOCK_CTOR(FORTE_E_RTimeOut){
  };

  virtual ~FORTE_E_RTimeOut(){};

};

#endif //close the ifdef sequence from the beginning of the file

