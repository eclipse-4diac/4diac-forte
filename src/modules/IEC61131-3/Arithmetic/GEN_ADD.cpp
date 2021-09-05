/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH, fortiss GmbH
 *                      2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *******************************************************************************/

#include "GEN_ADD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_ADD_gen.cpp"
#endif

#include <anyhelper.h>
#include <ctype.h>
#include <stdio.h>
#include <forte_printer.h>

DEFINE_GENERIC_FIRMWARE_FB(GEN_ADD, g_nStringIdGEN_ADD)

const CStringDictionary::TStringId GEN_ADD::scm_anDataOutputNames[] = { g_nStringIdOUT };

const CStringDictionary::TStringId GEN_ADD::scm_anDataOutputTypeIds[] = { g_nStringIdANY_MAGNITUDE };

const TForteInt16 GEN_ADD::scm_anEIWithIndexes[] = { 0 };
const CStringDictionary::TStringId GEN_ADD::scm_anEventInputNames[] = { g_nStringIdREQ };

const TDataIOID GEN_ADD::scm_anEOWith[] = { 0, 255 };
const TForteInt16 GEN_ADD::scm_anEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId GEN_ADD::scm_anEventOutputNames[] = { g_nStringIdCNF };

GEN_ADD::GEN_ADD(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(pa_poSrcRes, pa_nInstanceNameId), m_anDataInputNames(0), m_anDataInputTypeIds(0), m_anEIWith(0), m_nDInputs(0){
}

GEN_ADD::~GEN_ADD(){
  delete[] m_anDataInputNames;
  delete[] m_anDataInputTypeIds;
  delete[] m_anEIWith;
}

void GEN_ADD::executeEvent(int paEIID){
  switch (paEIID){
    case scm_nEventREQID:

      anyMagnitudeFBHelper<GEN_ADD>(st_OUT().getDataTypeID(), *this);
      sendOutputEvent(scm_nEventCNFID);

      break;
  }
}

bool GEN_ADD::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec){
  const char *pcPos = strrchr(paConfigString, '_');

  if(0 != pcPos){
    pcPos++;
    //we have an underscore and it is the first underscore after AND
    m_nDInputs = static_cast<unsigned int>(forte::core::util::strtoul(pcPos, 0, 10));
    DEVLOG_DEBUG("DIs: %d;\n", m_nDInputs);
  }
  else{
    return false;
  }

  if(m_nDInputs < 2){
    return false;
  }

  //now the number of needed eventInputs and dataOutputs are available in the integer array
  //create the eventInputs
  if(m_nDInputs < CFunctionBlock::scm_nMaxInterfaceEvents){

    //create the data inputs
    m_anDataInputNames = new CStringDictionary::TStringId[m_nDInputs];
    m_anDataInputTypeIds = new CStringDictionary::TStringId[m_nDInputs];

    char diNames[cg_nIdentifierLength] = { "IN" };

    for(size_t di = 0; di < m_nDInputs; ++di){
      forte_snprintf(&(diNames[2]), 5 - 2, "%i", di + 1);
      m_anDataInputNames[di] = CStringDictionary::getInstance().insert(diNames);
      m_anDataInputTypeIds[di] = g_nStringIdANY_MAGNITUDE;
    }

    //now create the WITH constructs...
    //the With-Indexes for the data variables
    m_anEIWith = new TDataIOID[m_nDInputs + 1]; //for inputs + '255' separators at the list end

    //in-withs
    for(size_t in_with = 0; in_with < m_nDInputs + 1; ++in_with){
      if(in_with == m_nDInputs){
        //set end separator of with
        m_anEIWith[in_with] = scmWithListDelimiter;
      }
      else{
        m_anEIWith[in_with] = static_cast<TDataIOID>(in_with);
      }
    }

    //set the resulting the interface Specification
    paInterfaceSpec.m_nNumEIs = 1;
    paInterfaceSpec.m_aunEINames = scm_anEventInputNames;
    paInterfaceSpec.m_anEIWith = m_anEIWith;
    paInterfaceSpec.m_anEIWithIndexes = scm_anEIWithIndexes;
    paInterfaceSpec.m_nNumEOs = 1;
    paInterfaceSpec.m_aunEONames = scm_anEventOutputNames;
    paInterfaceSpec.m_anEOWith = scm_anEOWith;
    paInterfaceSpec.m_anEOWithIndexes = scm_anEOWithIndexes;
    paInterfaceSpec.m_nNumDIs = static_cast<TForteUInt8>(m_nDInputs);
    paInterfaceSpec.m_aunDINames = m_anDataInputNames;
    paInterfaceSpec.m_aunDIDataTypeNames = m_anDataInputTypeIds;
    paInterfaceSpec.m_nNumDOs = 1;
    paInterfaceSpec.m_aunDONames = scm_anDataOutputNames;
    paInterfaceSpec.m_aunDODataTypeNames = scm_anDataOutputTypeIds;
    return true;
  }
  return false;
}
