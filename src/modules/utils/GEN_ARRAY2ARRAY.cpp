/*******************************************************************************
 * Copyright (c) 2014 - 2015 Profactor GmbH, fortiss GmbH
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
#include "GEN_ARRAY2ARRAY.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_ARRAY2ARRAY_gen.cpp"
#endif

DEFINE_GENERIC_FIRMWARE_FB(GEN_ARRAY2ARRAY, g_nStringIdGEN_ARRAY2ARRAY)

const CStringDictionary::TStringId GEN_ARRAY2ARRAY::scm_anDataInputNames[] = { g_nStringIdIN };
const CStringDictionary::TStringId GEN_ARRAY2ARRAY::scm_anDataOutputNames[] = { g_nStringIdOUT };

const TForteInt16 GEN_ARRAY2ARRAY::scm_anEIWithIndexes[] = { 0 };
const TDataIOID GEN_ARRAY2ARRAY::scm_anEIWith[] = { 0, 255 };
const CStringDictionary::TStringId GEN_ARRAY2ARRAY::scm_anEventInputNames[] = { g_nStringIdREQ };

const TDataIOID GEN_ARRAY2ARRAY::scm_anEOWith[] = { 0, 255 };
const TForteInt16 GEN_ARRAY2ARRAY::scm_anEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId GEN_ARRAY2ARRAY::scm_anEventOutputNames[] = { g_nStringIdCNF };

GEN_ARRAY2ARRAY::GEN_ARRAY2ARRAY(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(paSrcRes, paInstanceNameId), m_anDataInputTypeIds(0), m_anDataOutputTypeIds(0), m_ValueTypeID(CStringDictionary::scm_nInvalidStringId), m_nArrayLength(0){
}

GEN_ARRAY2ARRAY::~GEN_ARRAY2ARRAY(){
  delete[] m_anDataInputTypeIds;
  delete[] m_anDataOutputTypeIds;
}

void GEN_ARRAY2ARRAY::executeEvent(int paEIID){
  switch (paEIID){
    case scm_nEventREQID:

      OUT_Array().setValue(IN_Array());

      sendOutputEvent(scm_nEventCNFID);

      break;
  }
}

bool GEN_ARRAY2ARRAY::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec){
  const char *dNumberPos = strchr(paConfigString, '_');

  if(0 != dNumberPos){
    ++dNumberPos;
    //get position of a second underscore
    const char *dTypePos = strchr(dNumberPos, '_');

    if(0 != dTypePos){
      //there is a number and a data type of inputs within the typename
      m_nArrayLength = static_cast<unsigned int>(forte::core::util::strtoul(dNumberPos, 0, 10));
      m_ValueTypeID = CStringDictionary::getInstance().getId(++dTypePos);
    }
    else{
      m_ValueTypeID = CStringDictionary::scm_nInvalidStringId;
      m_nArrayLength = 0;
    }
  }
  else{
    return false;
  }

  if(m_ValueTypeID != CStringDictionary::scm_nInvalidStringId && m_nArrayLength >= 1){
    //create data input type
    m_anDataInputTypeIds = new CStringDictionary::TStringId[3];
    m_anDataInputTypeIds[0] = g_nStringIdARRAY;
    m_anDataInputTypeIds[1] = m_nArrayLength;
    m_anDataInputTypeIds[2] = m_ValueTypeID;

    //create data output type
    m_anDataOutputTypeIds = new CStringDictionary::TStringId[3];
    m_anDataOutputTypeIds[0] = g_nStringIdARRAY;
    m_anDataOutputTypeIds[1] = m_nArrayLength;
    m_anDataOutputTypeIds[2] = m_ValueTypeID;

    //create the interface Specification
    paInterfaceSpec.m_nNumEIs = 1;
    paInterfaceSpec.m_aunEINames = scm_anEventInputNames;
    paInterfaceSpec.m_anEIWith = scm_anEIWith;
    paInterfaceSpec.m_anEIWithIndexes = scm_anEIWithIndexes;
    paInterfaceSpec.m_nNumEOs = 1;
    paInterfaceSpec.m_aunEONames = scm_anEventOutputNames;
    paInterfaceSpec.m_anEOWith = scm_anEOWith;
    paInterfaceSpec.m_anEOWithIndexes = scm_anEOWithIndexes;
    paInterfaceSpec.m_nNumDIs = 1;
    paInterfaceSpec.m_aunDINames = scm_anDataInputNames;
    paInterfaceSpec.m_aunDIDataTypeNames = m_anDataInputTypeIds;
    paInterfaceSpec.m_nNumDOs = 1;
    paInterfaceSpec.m_aunDONames = scm_anDataOutputNames;
    paInterfaceSpec.m_aunDODataTypeNames = m_anDataOutputTypeIds;
    return true;
  }

  return false;
}

