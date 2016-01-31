/*******************************************************************************
 * Copyright (c) 2011 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Matthias Plasch
 *     - initial API and implementation and/or initial documentation
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

GEN_E_MUX::GEN_E_MUX(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CFunctionBlock(pa_poSrcRes, 0, pa_nInstanceNameId, 0, 0){
}

GEN_E_MUX::~GEN_E_MUX(){
  delete[] m_anEventInputNames;
}

void GEN_E_MUX::executeEvent(int pa_nEIID){
  if(pa_nEIID < m_pstInterfaceSpec->m_nNumEIs){
    K() = pa_nEIID;
    sendOutputEvent(scm_nEventEOID);
  }
}

bool GEN_E_MUX::configureFB(const char *pa_acConfigString){
  bool bRetVal = false;

  m_nConfiguredFBTypeNameId = CStringDictionary::getInstance().insert(pa_acConfigString);

  const char *acPos = strrchr(pa_acConfigString, '_');

  if(0 != acPos){
    ++acPos;
    if('M' != *acPos){
      //we have an underscore and it is not the first underscore after E
      unsigned int nNumEIs = static_cast<unsigned int>(forte::core::util::strtoul(acPos, 0, 10));

      if(nNumEIs < CFunctionBlock::scm_nMaxInterfaceEvents && nNumEIs >= 2){
        m_anEventInputNames = new CStringDictionary::TStringId[nNumEIs];

        generateGenericInterfacePointNameArray("EI", m_anEventInputNames,  nNumEIs);

        SFBInterfaceSpecforGenerics *pstInterfaceSpec = new SFBInterfaceSpecforGenerics(
            static_cast<TForteUInt8>(nNumEIs), m_anEventInputNames, 0, 0,
            1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
            0, 0, 0,
            1, scm_anDataOutputNames, scm_aunDODataTypeIds);

        TForteByte *acFBConnData = new TForteByte[genFBConnDataSize(1, 0, pstInterfaceSpec->m_nNumDOs)];
        TForteByte *acFBVarsData = new TForteByte[genFBVarsDataSize(0, pstInterfaceSpec->m_nNumDOs)];

        setupFBInterface(pstInterfaceSpec, acFBConnData, acFBVarsData, true);
        bRetVal = true;
      }
	  else{
		  if(nNumEIs >= CFunctionBlock::scm_nMaxInterfaceEvents){
			  DEVLOG_ERROR("Cannot configure FB-Instance E_MUX_%d. Number of event inputs exceeds maximum of %d.\n", nNumEIs, CFunctionBlock::scm_nMaxInterfaceEvents);
		  }
		  else{
			  DEVLOG_ERROR("Cannot configure FB-Instance E_MUX_%d. Number of event inputs smaller than minimum of 2.\n", nNumEIs);
		  }  
	  }
    }
  }
  return bRetVal;
}
