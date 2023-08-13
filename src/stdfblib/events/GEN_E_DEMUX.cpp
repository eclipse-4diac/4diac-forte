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
#include "GEN_E_DEMUX.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_E_DEMUX_gen.cpp"
#endif
#include <stdio.h>

#include "resource.h"
#include "criticalregion.h"

DEFINE_GENERIC_FIRMWARE_FB(GEN_E_DEMUX, g_nStringIdGEN_E_DEMUX)

const CStringDictionary::TStringId GEN_E_DEMUX::scm_anDataInputNames[] = { g_nStringIdK };
const CStringDictionary::TStringId GEN_E_DEMUX::scm_aunDIDataTypeIds[] = { g_nStringIdUINT };

const CStringDictionary::TStringId GEN_E_DEMUX::scm_anEventInputNames[] = { g_nStringIdEI };

GEN_E_DEMUX::GEN_E_DEMUX(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(paSrcRes, paInstanceNameId), m_anEventOutputNames(nullptr){
}

GEN_E_DEMUX::~GEN_E_DEMUX(){
  delete[] m_anEventOutputNames;
}

void GEN_E_DEMUX::executeEvent(TEventID paEIID){
  if(scm_nEventEIID == paEIID && static_cast<CIEC_UINT::TValueType>(K()) < mInterfaceSpec->m_nNumEOs) {
    sendOutputEvent(static_cast<CIEC_UINT::TValueType>(K())); // the value of K corresponds to the output event ID;
  }
}

void GEN_E_DEMUX::readInputData(TEventID) {
  RES_DATA_CON_CRITICAL_REGION();
  readData(0, *mDIs[0], mDIConns[0]);
}

void GEN_E_DEMUX::writeOutputData(TEventID) {
}

bool GEN_E_DEMUX::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec){
  const char *acPos = strrchr(paConfigString, '_');

  if(nullptr != acPos){
    ++acPos;
    if('D' != *acPos){
      //we have an underscore and it is not the first underscore after E
      paInterfaceSpec.m_nNumEOs = static_cast<TEventID>(forte::core::util::strtoul(acPos, nullptr, 10));

      if(paInterfaceSpec.m_nNumEOs < CFunctionBlock::scm_nMaxInterfaceEvents){
        m_anEventOutputNames = new CStringDictionary::TStringId[paInterfaceSpec.m_nNumEOs];

        generateGenericInterfacePointNameArray("EO", m_anEventOutputNames, paInterfaceSpec.m_nNumEOs);

        paInterfaceSpec.m_nNumEIs = 1;
        paInterfaceSpec.m_aunEINames = scm_anEventInputNames;
        paInterfaceSpec.m_aunEONames = m_anEventOutputNames;
        paInterfaceSpec.m_nNumDIs = 1;
        paInterfaceSpec.m_aunDINames = scm_anDataInputNames;
        paInterfaceSpec.m_aunDIDataTypeNames = scm_aunDIDataTypeIds;
        paInterfaceSpec.m_nNumDOs = 0;
        paInterfaceSpec.m_aunDONames = nullptr;
        paInterfaceSpec.m_aunDODataTypeNames = nullptr;
        return true;
      }
    }
  }
  return false;
}
