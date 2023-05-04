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

#pragma once

#include "cfb.h"
#include "typelib.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "ARTimeOut.h"


class FORTE_E_RTimeOut: public CCompositeFB {
  DECLARE_FIRMWARE_FB(FORTE_E_RTimeOut)

private:
  static const int scm_nTimeOutSocketAdpNum = 0;
  
  static const SAdapterInstanceDef scm_astAdapterInstances[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  static const SCFB_FBInstanceData scm_astInternalFBs[];
  static const SCFB_FBParameter scm_astParamters[];
  static const SCFB_FBConnectionData scm_astEventConnections[];
  static const SCFB_FBFannedOutConnectionData scm_astFannedOutEventConnections[];
  static const SCFB_FBConnectionData scm_astDataConnections[];
  static const SCFB_FBFannedOutConnectionData scm_astFannedOutDataConnections[];
  static const SCFB_FBNData scm_stFBNData;

  void readInputData(size_t pa_nEIID) override;
  void writeOutputData(size_t pa_nEIID) override;

public:
  FORTE_E_RTimeOut(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  FORTE_ARTimeOut &var_TimeOutSocket() {
    return *static_cast<FORTE_ARTimeOut*>(m_apoAdapters[0]);
  };
  
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
};



