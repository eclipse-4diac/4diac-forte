/*******************************************************************************
 * Copyright (c) 2014 - 2015 Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Matthias Plasch, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "GEN_VALUES2ARRAY.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_VALUES2ARRAY_gen.cpp"
#endif

#include <stdio.h>

DEFINE_GENERIC_FIRMWARE_FB(GEN_VALUES2ARRAY, g_nStringIdGEN_VALUES2ARRAY)

const CStringDictionary::TStringId GEN_VALUES2ARRAY::scm_anDataOutputNames[] = { g_nStringIdOUT };

const TForteInt16 GEN_VALUES2ARRAY::scm_anEIWithIndexes[] = { 0 };
const CStringDictionary::TStringId GEN_VALUES2ARRAY::scm_anEventInputNames[] = { g_nStringIdREQ };

const TDataIOID GEN_VALUES2ARRAY::scm_anEOWith[] = { 0, 255 };
const TForteInt16 GEN_VALUES2ARRAY::scm_anEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId GEN_VALUES2ARRAY::scm_anEventOutputNames[] = { g_nStringIdCNF };

GEN_VALUES2ARRAY::GEN_VALUES2ARRAY(const CStringDictionary::TStringId pa_nInstanceNameId,
    CResource *pa_poSrcRes) :
    CFunctionBlock(pa_poSrcRes, 0, pa_nInstanceNameId, 0, 0){
}

GEN_VALUES2ARRAY::~GEN_VALUES2ARRAY(){
  delete[] m_anDataInputNames;
  delete[] m_anDataInputTypeIds;
  delete[] m_anDataOutputTypeIds;
  delete[] m_anEIWith;
}

void GEN_VALUES2ARRAY::executeEvent(int pa_nEIID){
  switch (pa_nEIID){
    case scm_nEventREQID:

      for(int input_index = 0; input_index < m_nDInputs; input_index++){
        //copy input values to array
        OUT_Array()[input_index]->saveAssign(*getDI(input_index));
      }

      sendOutputEvent(scm_nEventCNFID);

      break;
  }
}

bool GEN_VALUES2ARRAY::configureFB(const char *pa_acConfigString){
  bool bRetVal = false;

  m_nConfiguredFBTypeNameId = CStringDictionary::getInstance().insert(pa_acConfigString);
  const char *dNumberPos = strchr(pa_acConfigString, '_');

  if(0 != dNumberPos){
    ++dNumberPos;
    //get position of a second underscore
    const char *dTypePos = strchr(dNumberPos, '_');

    if(0 != dTypePos){
      //there is a number and a data type of inputs within the typename
      m_nDInputs = forte::core::util::strtoul(dNumberPos, 0, 10);

      unsigned int nLen = strlen(++dTypePos);
      if(nLen < cg_nIdentifierLength){
        char dTypeName[cg_nIdentifierLength + 1];

        strncpy(dTypeName, dTypePos, nLen);
        dTypeName[nLen] = '\0';
        //get the data type id
        m_ValueTypeID = CStringDictionary::getInstance().getId(dTypeName);
        //DEVLOG_DEBUG("ValueType: %s, DIs: %d;\n", dTypeName, m_nDInputs);
      }
    }
    else{
      m_ValueTypeID = CStringDictionary::scm_nInvalidStringId;
      m_nDInputs = 0;
    }
  }
  else{
    return false;
  }

  if(m_ValueTypeID != CStringDictionary::scm_nInvalidStringId && m_nDInputs >= 2){
    //create the data inputs
    m_anDataInputNames = new CStringDictionary::TStringId[m_nDInputs];
    m_anDataInputTypeIds = new CStringDictionary::TStringId[m_nDInputs];

    char diNames[cg_nIdentifierLength] = { "IN_" };
    for(int di = 0; di < m_nDInputs; di = di + 1){
#ifdef WIN32
      _snprintf(&(diNames[3]), 7 - 3, "%i", di+1);
#else
      snprintf(&(diNames[3]), 7 - 3, "%i", di + 1);
#endif
      m_anDataInputNames[di] = CStringDictionary::getInstance().insert(diNames);
      m_anDataInputTypeIds[di] = m_ValueTypeID;
    }

    //create data output type
    m_anDataOutputTypeIds = new CStringDictionary::TStringId[3];
    m_anDataOutputTypeIds[0] = g_nStringIdARRAY;
    m_anDataOutputTypeIds[1] = m_nDInputs;
    m_anDataOutputTypeIds[2] = m_ValueTypeID;

    //get input with-indices for the data vars
    m_anEIWith = new TDataIOID[m_nDInputs + 1];

    //in-withs
    for(int in_with = 0; in_with < m_nDInputs + 1; in_with = in_with + 1){
      if(in_with == m_nDInputs){
        //set end separator of with
        m_anEIWith[in_with] = 255;
      }
      else{
        m_anEIWith[in_with] = static_cast<TDataIOID>(in_with);
      }
    }

    //create the interface Specification
    SFBInterfaceSpecforGenerics *pstInterfaceSpec =
        new SFBInterfaceSpecforGenerics(1, scm_anEventInputNames, m_anEIWith, scm_anEIWithIndexes, 1, scm_anEventOutputNames,
            scm_anEOWith, scm_anEOWithIndexes, static_cast<TForteUInt8>(m_nDInputs), m_anDataInputNames, m_anDataInputTypeIds, 1, scm_anDataOutputNames,
            m_anDataOutputTypeIds);

    TForteByte *acFBConnData =
        new TForteByte[genFBConnDataSize(pstInterfaceSpec->m_nNumEOs, pstInterfaceSpec->m_nNumDIs, pstInterfaceSpec->m_nNumDOs)];
    TForteByte *acFBVarsData =
        new TForteByte[genFBVarsDataSize(pstInterfaceSpec->m_nNumDIs, pstInterfaceSpec->m_nNumDOs)];

    setupFBInterface(pstInterfaceSpec, acFBConnData, acFBVarsData, true);
    bRetVal = true;
  }

  return bRetVal;
}

