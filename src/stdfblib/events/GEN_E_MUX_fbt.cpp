/*******************************************************************************
 * Copyright (c) 2011, 2023 ACIN, Profactor GmbH, fortiss GmbH
 *                          Johannes Kepler University
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Matthias Plasch
 *     - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *    Martin Jobst - add generic readInputData and writeOutputData
 *******************************************************************************/
#include "GEN_E_MUX_fbt.h"

USE_STRING_ID(EO);
USE_STRING_ID(Event);
USE_STRING_ID(GEN_E_MUX);
USE_STRING_ID(K);
USE_STRING_ID(UINT);

#include <stdio.h>

#include "resource.h"
#include "criticalregion.h"

DEFINE_GENERIC_FIRMWARE_FB(GEN_E_MUX, STRID(GEN_E_MUX));

const CStringDictionary::TStringId GEN_E_MUX::scmDataOutputNames[] = { STRID(K) };
const CStringDictionary::TStringId GEN_E_MUX::scmDODataTypeIds[] = { STRID(UINT) };

const CStringDictionary::TStringId GEN_E_MUX::scmEventOutputNames[] = { STRID(EO) };
const CStringDictionary::TStringId GEN_E_MUX::scmEventOutputTypeIds[] = { STRID(Event) };

GEN_E_MUX::GEN_E_MUX(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId) {
}

void GEN_E_MUX::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  if(paEIID < getFBInterfaceSpec().mNumEIs){
    K() = CIEC_UINT(static_cast<TForteUInt16>(paEIID));
    sendOutputEvent(scmEventEOID, paECET);
  }
}

void GEN_E_MUX::readInputData(TEventID) {
}

void GEN_E_MUX::writeOutputData(TEventID) {
  writeData(0, *mDOs[0], mDOConns[0]);
}

bool GEN_E_MUX::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec){
  const char *acPos = strrchr(paConfigString, '_');

  if(nullptr != acPos){
    ++acPos;
    if('M' != *acPos){
      //we have an underscore and it is not the first underscore after E
      paInterfaceSpec.mNumEIs = static_cast<TEventID>(forte::core::util::strtoul(acPos, nullptr, 10));

      if(paInterfaceSpec.mNumEIs < CFunctionBlock::scmMaxInterfaceEvents && paInterfaceSpec.mNumEIs >= 2){
        mEventInputNames = std::make_unique<CStringDictionary::TStringId[]>(paInterfaceSpec.mNumEIs);

        generateGenericInterfacePointNameArray("EI", mEventInputNames.get(), paInterfaceSpec.mNumEIs);

        paInterfaceSpec.mEINames = mEventInputNames.get();
        paInterfaceSpec.mNumEOs = 1;
        paInterfaceSpec.mEONames = scmEventOutputNames;
        paInterfaceSpec.mEITypeNames = nullptr;
        paInterfaceSpec.mEOTypeNames = scmEventOutputTypeIds;
        paInterfaceSpec.mNumDIs = 0;
        paInterfaceSpec.mDINames = nullptr;
        paInterfaceSpec.mDIDataTypeNames = nullptr;
        paInterfaceSpec.mNumDOs = 1;
        paInterfaceSpec.mDONames = scmDataOutputNames;
        paInterfaceSpec.mDODataTypeNames = scmDODataTypeIds;
        return true;
      }
      else{
        if(paInterfaceSpec.mNumEIs >= CFunctionBlock::scmMaxInterfaceEvents){
          DEVLOG_ERROR("Cannot configure FB-Instance E_MUX_%d. Number of event inputs exceeds maximum of %d.\n", paInterfaceSpec.mNumEIs, CFunctionBlock::scmMaxInterfaceEvents);
        }
        else{
          DEVLOG_ERROR("Cannot configure FB-Instance E_MUX_%d. Number of event inputs smaller than minimum of 2.\n", paInterfaceSpec.mNumEIs);
        }
      }
    }
  }
  return false;
}
