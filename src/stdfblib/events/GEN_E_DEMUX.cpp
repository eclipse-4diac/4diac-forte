/*******************************************************************************
 * Copyright (c) 2011 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 *                      2018 Johannes Kepler University
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Matthias Plasch
 *     - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *******************************************************************************/
#include "GEN_E_DEMUX.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_E_DEMUX_gen.cpp"
#endif
#include <stdio.h>

DEFINE_GENERIC_FIRMWARE_FB(GEN_E_DEMUX, g_nStringIdGEN_E_DEMUX)

const CStringDictionary::TStringId GEN_E_DEMUX::scm_anDataInputNames[] = { g_nStringIdK };
const CStringDictionary::TStringId GEN_E_DEMUX::scm_aunDIDataTypeIds[] = { g_nStringIdUINT };

const TForteInt16 GEN_E_DEMUX::scm_anEIWithIndexes[] = { 0 };
const TDataIOID GEN_E_DEMUX::scm_anEIWith[] = { 0, 255 };
const CStringDictionary::TStringId GEN_E_DEMUX::scm_anEventInputNames[] = { g_nStringIdEI };

GEN_E_DEMUX::GEN_E_DEMUX(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(pa_poSrcRes, pa_nInstanceNameId),
    m_anEventOutputNames(0){
}

GEN_E_DEMUX::~GEN_E_DEMUX(){
  delete[] m_anEventOutputNames;
}

void GEN_E_DEMUX::executeEvent(int pa_nEIID){
  if(scm_nEventEIID == pa_nEIID){
    if(K() < m_pstInterfaceSpec->m_nNumEOs){
      sendOutputEvent(K()); // the value of K corresponds to the output event ID;
    }
  }
}

SFBInterfaceSpecforGenerics *GEN_E_DEMUX::createInterfaceSpec(const char *paConfigString) {
  SFBInterfaceSpecforGenerics *interfaceSpec = 0;
  const char *acPos = strrchr(paConfigString, '_');

  if(0 != acPos){
    ++acPos;
    if('D' != *acPos){
      //we have an underscore and it is not the first underscore after E
      unsigned int nNumEOs = static_cast<unsigned int>(forte::core::util::strtoul(acPos, 0, 10));

      if(nNumEOs < CFunctionBlock::scm_nMaxInterfaceEvents){
        m_anEventOutputNames = new CStringDictionary::TStringId[nNumEOs];

        generateGenericInterfacePointNameArray("EO", m_anEventOutputNames,  nNumEOs);

        interfaceSpec = new SFBInterfaceSpecforGenerics(
            1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
            static_cast<TForteUInt8>(nNumEOs), m_anEventOutputNames, 0, 0,
            1, scm_anDataInputNames, scm_aunDIDataTypeIds,
            0, 0, 0);
      }
    }
  }
  return interfaceSpec;
}
