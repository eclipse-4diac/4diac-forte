/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Rene Smodic, Thomas Strasser,
 *   Martin Melik Merkumians, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "RMT_RES.h"

USE_STRING_ID(COLD);
USE_STRING_ID(EO);
USE_STRING_ID(ID);
USE_STRING_ID(INIT);
USE_STRING_ID(MGR);
USE_STRING_ID(MGR_FF);
USE_STRING_ID(MGR_ID);
USE_STRING_ID(Q);
USE_STRING_ID(QI);
USE_STRING_ID(R);
USE_STRING_ID(RMT_RES);
USE_STRING_ID(S);
USE_STRING_ID(START);
USE_STRING_ID(STOP);
USE_STRING_ID(WARM);
USE_STRING_ID(WSTRING);


#include "../../core/ecet.h"

DEFINE_FIRMWARE_FB(RMT_RES, STRID(RMT_RES));

const CStringDictionary::TStringId RMT_RES::scmVarInputNameIds[] = {STRID(MGR_ID)};
const CStringDictionary::TStringId RMT_RES::scmDIDataTypeIds[] = {STRID(WSTRING)};


const SFBInterfaceSpec RMT_RES::scmFBInterfaceSpec = {
  0, nullptr, nullptr, nullptr, nullptr,
  0, nullptr, nullptr, nullptr, nullptr,
  1, scmVarInputNameIds, scmDIDataTypeIds,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};


RMT_RES::RMT_RES(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paDevice):
       CResource(paDevice, scmFBInterfaceSpec, paInstanceNameId),
       var_MGR_ID(""),
       conn_MGR_ID(nullptr),
       fb_START(STRID(START), *this),
       fb_MGR_FF(STRID(MGR_FF), *this),
       fb_MGR(STRID(MGR), *this) {
}

bool RMT_RES::initialize() {
  if(!CResource::initialize()) {
    return false;
  }

  forte::core::SManagementCMD command;

  command.mFirstParam.push_back(STRID(START));
  command.mFirstParam.push_back(STRID(COLD));
  command.mSecondParam.push_back(STRID(MGR_FF));
  command.mSecondParam.push_back(STRID(S));
  createConnection(command);

  command.mFirstParam.clear();
  command.mFirstParam.push_back(STRID(START));
  command.mFirstParam.push_back(STRID(WARM));
  command.mSecondParam.clear();
  command.mSecondParam.push_back(STRID(MGR_FF));
  command.mSecondParam.push_back(STRID(S));
  createConnection(command);

  command.mFirstParam.clear();
  command.mFirstParam.push_back(STRID(START));
  command.mFirstParam.push_back(STRID(STOP));
  command.mSecondParam.clear();
  command.mSecondParam.push_back(STRID(MGR_FF));
  command.mSecondParam.push_back(STRID(R));
  createConnection(command);

  command.mFirstParam.clear();
  command.mFirstParam.push_back(STRID(MGR_FF));
  command.mFirstParam.push_back(STRID(EO));
  command.mSecondParam.clear();
  command.mSecondParam.push_back(STRID(MGR));
  command.mSecondParam.push_back(STRID(INIT));
  createConnection(command);

  command.mFirstParam.clear();
  command.mFirstParam.push_back(STRID(MGR_FF));
  command.mFirstParam.push_back(STRID(Q));
  command.mSecondParam.clear();
  command.mSecondParam.push_back(STRID(MGR));
  command.mSecondParam.push_back(STRID(QI));
  createConnection(command);

  command.mFirstParam.clear();
  command.mFirstParam.push_back(STRID(MGR_ID));
  command.mSecondParam.clear();
  command.mSecondParam.push_back(STRID(MGR));
  command.mSecondParam.push_back(STRID(ID));
  createConnection(command);
  return true;
}

RMT_RES::~RMT_RES() = default;

void RMT_RES::joinResourceThread() const {
  getResourceEventExecution()->joinEventChainExecutionThread();
}

CIEC_ANY *RMT_RES::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_MGR_ID;
  }
  return nullptr;
}

CDataConnection **RMT_RES::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_MGR_ID;
  }
  return nullptr;
}

