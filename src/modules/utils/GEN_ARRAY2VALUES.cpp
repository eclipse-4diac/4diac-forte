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
#include "GEN_ARRAY2VALUES.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_ARRAY2VALUES_gen.cpp"
#endif

#include <stdio.h>
#include <forte_printer.h>

DEFINE_GENERIC_FIRMWARE_FB(GEN_ARRAY2VALUES, g_nStringIdGEN_ARRAY2VALUES)

const CStringDictionary::TStringId GEN_ARRAY2VALUES::scm_anDataInputNames[] = { g_nStringIdIN };

const TForteInt16 GEN_ARRAY2VALUES::scm_anEIWithIndexes[] = { 0 };
const TDataIOID GEN_ARRAY2VALUES::scm_anEIWith[] = { 0, 255 };
const CStringDictionary::TStringId GEN_ARRAY2VALUES::scm_anEventInputNames[] = { g_nStringIdREQ };

const TForteInt16 GEN_ARRAY2VALUES::scm_anEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId GEN_ARRAY2VALUES::scm_anEventOutputNames[] = { g_nStringIdCNF };

GEN_ARRAY2VALUES::GEN_ARRAY2VALUES(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(paSrcRes, paInstanceNameId), m_anDataOutputNames(0), m_anDataOutputTypeIds(0), m_anDataInputTypeIds(0), m_anEOWith(0), mDOutputs(0), mValueTypeID(CStringDictionary::scm_nInvalidStringId){
}

GEN_ARRAY2VALUES::~GEN_ARRAY2VALUES(){
  delete[] m_anDataOutputNames;
  delete[] m_anDataInputTypeIds;
  delete[] m_anDataOutputTypeIds;
  delete[] m_anEOWith;
}

void GEN_ARRAY2VALUES::executeEvent(int paEIID){
  switch (paEIID){
    case scm_nEventREQID:

      for(size_t output_index = 0; output_index < mDOutputs; output_index++) {
        //copy input values to array
        getDO(static_cast<unsigned int>(output_index))->saveAssign(*(IN_Array()[static_cast<TForteUInt16>(output_index)]));
      }

      sendOutputEvent(scm_nEventCNFID);

      break;
  }
}

bool GEN_ARRAY2VALUES::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec){
  const char *dNumberPos = strchr(paConfigString, '_');

  if(0 != dNumberPos){
    ++dNumberPos;
    //get position of a second underscore
    const char *dTypePos = strchr(dNumberPos, '_');

    if(0 != dTypePos){
      //there is a number and a data type of inputs within the typename
      mDOutputs = static_cast<size_t>(forte::core::util::strtoul(dNumberPos, 0, 10));
      mValueTypeID = CStringDictionary::getInstance().getId(++dTypePos);
    }
    else{
      mValueTypeID = CStringDictionary::scm_nInvalidStringId;
      mDOutputs = 0;
    }
  }
  else{
    return false;
  }

  if(mValueTypeID != CStringDictionary::scm_nInvalidStringId && mDOutputs >= 2){
    //create the data outputs
    m_anDataOutputNames = new CStringDictionary::TStringId[mDOutputs];
    m_anDataOutputTypeIds = new CStringDictionary::TStringId[mDOutputs];

    char doNames[cg_nIdentifierLength] = { "OUT_" };
    for(size_t doIndex = 0; doIndex < mDOutputs; ++doIndex){
      forte_snprintf(&(doNames[4]), 8 - 4, "%i", doIndex + 1);
      m_anDataOutputNames[doIndex] = CStringDictionary::getInstance().insert(doNames);
      m_anDataOutputTypeIds[doIndex] = mValueTypeID;
    }

    //create data input type
    m_anDataInputTypeIds = new CStringDictionary::TStringId[3];
    m_anDataInputTypeIds[0] = g_nStringIdARRAY;
    m_anDataInputTypeIds[1] = static_cast<CStringDictionary::TStringId>(mDOutputs);
    m_anDataInputTypeIds[2] = mValueTypeID;

    //get input with-indices for the data vars
    m_anEOWith = new TDataIOID[mDOutputs + 1];

    //in-withs
    for(size_t out_with = 0; out_with < mDOutputs; ++out_with){
      m_anEOWith[out_with] = static_cast<TDataIOID>(out_with);
    }
    m_anEOWith[mDOutputs] = scmWithListDelimiter;

    //create the interface Specification
    paInterfaceSpec.m_nNumEIs = 1;
    paInterfaceSpec.m_aunEINames = scm_anEventInputNames;
    paInterfaceSpec.m_anEIWith = scm_anEIWith;
    paInterfaceSpec.m_anEIWithIndexes = scm_anEIWithIndexes;
    paInterfaceSpec.m_nNumEOs = 1;
    paInterfaceSpec.m_aunEONames = scm_anEventOutputNames;
    paInterfaceSpec.m_anEOWith = m_anEOWith;
    paInterfaceSpec.m_anEOWithIndexes = scm_anEOWithIndexes;
    paInterfaceSpec.m_nNumDIs = 1;
    paInterfaceSpec.m_aunDINames = scm_anDataInputNames;
    paInterfaceSpec.m_aunDIDataTypeNames = m_anDataInputTypeIds;
    paInterfaceSpec.m_nNumDOs = static_cast<TForteUInt8>(mDOutputs);
    paInterfaceSpec.m_aunDONames = m_anDataOutputNames;
    paInterfaceSpec.m_aunDODataTypeNames = m_anDataOutputTypeIds;
    return true;
  }

  return false;
}

