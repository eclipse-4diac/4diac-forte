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
  static const int scmTimeOutSocketAdpNum = 0;
  
  static const SAdapterInstanceDef scmAdapterInstances[];

  static const SFBInterfaceSpec scmFBInterfaceSpec;

  static const SCFB_FBInstanceData scmInternalFBs[];
  static const SCFB_FBParameter scmParamters[];
  static const SCFB_FBConnectionData scmEventConnections[];
  static const SCFB_FBFannedOutConnectionData scmFannedOutEventConnections[];
  static const SCFB_FBConnectionData scmDataConnections[];
  static const SCFB_FBFannedOutConnectionData scmFannedOutDataConnections[];
  static const SCFB_FBNData scmFBNData;

  void readInputData(TEventID paEIID) override;
  void writeOutputData(TEventID paEIID) override;
  void readInternal2InterfaceOutputData(TEventID paEOID) override;

public:
  FORTE_E_RTimeOut(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);

  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  FORTE_ARTimeOut &var_TimeOutSocket() {
    return *static_cast<FORTE_ARTimeOut*>(mAdapters[0]);
  };
  
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
};



