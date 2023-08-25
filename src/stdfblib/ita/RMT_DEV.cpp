/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Rene Smodic, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "RMT_DEV.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RMT_DEV_gen.cpp"
#endif
#include <stringdict.h>

const CStringDictionary::TStringId RMT_DEV::scmDINameIds[] = { g_nStringIdMGR_ID };
const CStringDictionary::TStringId RMT_DEV::scmDIDataTypeIds[] = {g_nStringIdWSTRING};

const SFBInterfaceSpec RMT_DEV::scmFBInterfaceSpec = {
  0, nullptr, nullptr, nullptr,
  0, nullptr, nullptr, nullptr,
  1, scmDINameIds, scmDIDataTypeIds,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

RMT_DEV::RMT_DEV() :
  CDevice(&scmFBInterfaceSpec, CStringDictionary::scmInvalidStringId),
      MGR(g_nStringIdMGR, this){
}

bool RMT_DEV::initialize() {
  if(!CDevice::initialize()) {
    return false;
  }
  MGR.initialize();
  MGR_ID().fromString("localhost:61499");

  //we nee to manually crate this interface2internal connection as the MGR is not managed by device
  mDConnMGR_ID.setSource(this, 0);
  mDConnMGR_ID.connect(&MGR, g_nStringIdMGR_ID);
  return true;
}

RMT_DEV::~RMT_DEV() = default;

int RMT_DEV::startDevice(){
  CDevice::startDevice();
  MGR.changeFBExecutionState(EMGMCommandType::Start);
  return 0;
}

EMGMResponse RMT_DEV::changeFBExecutionState(EMGMCommandType paCommand){
  EMGMResponse eRetVal = CDevice::changeFBExecutionState(paCommand);
  if((EMGMResponse::Ready == eRetVal) && (EMGMCommandType::Kill == paCommand)){
    MGR.changeFBExecutionState(EMGMCommandType::Kill);
  }
  return eRetVal;
}

void RMT_DEV::setMGR_ID(const char * const paConn){
  MGR_ID().fromString(paConn);
}
