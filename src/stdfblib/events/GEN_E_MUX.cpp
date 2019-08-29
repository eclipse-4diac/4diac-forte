/*******************************************************************************
 * Copyright (c) 2011 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 *                      2018 Johannes Kepler University
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
 *******************************************************************************/
#include "GEN_E_MUX.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_E_MUX_gen.cpp"
#endif
#include <stdio.h>

DEFINE_GENERIC_FIRMWARE_FB(GEN_E_MUX, g_nStringIdGEN_E_MUX);

const CStringDictionary::TStringId GEN_E_MUX::scm_anDataOutputNames[] = { g_nStringIdK };
const CStringDictionary::TStringId GEN_E_MUX::scm_aunDODataTypeIds[] = { g_nStringIdUINT };

const TForteInt16 GEN_E_MUX::scm_anEOWithIndexes[] = { 0 };
const TDataIOID GEN_E_MUX::scm_anEOWith[] = { 0, 255 };
const CStringDictionary::TStringId GEN_E_MUX::scm_anEventOutputNames[] = { g_nStringIdEO };

GEN_E_MUX::GEN_E_MUX(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(paSrcRes, paInstanceNameId), m_anEventInputNames(0){
}

GEN_E_MUX::~GEN_E_MUX(){
  delete[] m_anEventInputNames;
}

void GEN_E_MUX::executeEvent(int paEIID){
  if(paEIID < m_pstInterfaceSpec->m_nNumEIs){
    K() = static_cast<TForteUInt16>(paEIID);
    sendOutputEvent(scm_nEventEOID);
  }
}

bool GEN_E_MUX::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec){
  const char *acPos = strrchr(paConfigString, '_');

  if(0 != acPos){
    ++acPos;
    if('M' != *acPos){
      //we have an underscore and it is not the first underscore after E
      paInterfaceSpec.m_nNumEIs = static_cast<TForteUInt8>(forte::core::util::strtoul(acPos, 0, 10));

      if(paInterfaceSpec.m_nNumEIs < CFunctionBlock::scm_nMaxInterfaceEvents && paInterfaceSpec.m_nNumEIs >= 2){
        m_anEventInputNames = new CStringDictionary::TStringId[paInterfaceSpec.m_nNumEIs];

        generateGenericInterfacePointNameArray("EI", m_anEventInputNames, paInterfaceSpec.m_nNumEIs);

        paInterfaceSpec.m_aunEINames = m_anEventInputNames;
        paInterfaceSpec.m_anEIWith = 0;
        paInterfaceSpec.m_anEIWithIndexes = 0;
        paInterfaceSpec.m_nNumEOs = 1;
        paInterfaceSpec.m_aunEONames = scm_anEventOutputNames;
        paInterfaceSpec.m_anEOWith = scm_anEOWith;
        paInterfaceSpec.m_anEOWithIndexes = scm_anEOWithIndexes;
        paInterfaceSpec.m_nNumDIs = 0;
        paInterfaceSpec.m_aunDINames = 0;
        paInterfaceSpec.m_aunDIDataTypeNames = 0;
        paInterfaceSpec.m_nNumDOs = 1;
        paInterfaceSpec.m_aunDONames = scm_anDataOutputNames;
        paInterfaceSpec.m_aunDODataTypeNames = scm_aunDODataTypeIds;
        return true;
      }
      else{
        if(paInterfaceSpec.m_nNumEIs >= CFunctionBlock::scm_nMaxInterfaceEvents){
          DEVLOG_ERROR("Cannot configure FB-Instance E_MUX_%d. Number of event inputs exceeds maximum of %d.\n", paInterfaceSpec.m_nNumEIs, CFunctionBlock::scm_nMaxInterfaceEvents);
        }
        else{
          DEVLOG_ERROR("Cannot configure FB-Instance E_MUX_%d. Number of event inputs smaller than minimum of 2.\n", paInterfaceSpec.m_nNumEIs);
        }
      }
    }
  }
  return false;
}
