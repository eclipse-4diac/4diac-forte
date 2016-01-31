/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Matthias Plasch, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "GEN_ARRAY2VALUES.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_ARRAY2VALUES_gen.cpp"
#endif

#include <stdio.h>

DEFINE_GENERIC_FIRMWARE_FB(GEN_ARRAY2VALUES, g_nStringIdGEN_ARRAY2VALUES)

const CStringDictionary::TStringId GEN_ARRAY2VALUES::scm_anDataInputNames[] = { g_nStringIdIN };

const TForteInt16 GEN_ARRAY2VALUES::scm_anEIWithIndexes[] = { 0 };
const TDataIOID GEN_ARRAY2VALUES::scm_anEIWith[] = { 0, 255 };
const CStringDictionary::TStringId GEN_ARRAY2VALUES::scm_anEventInputNames[] = { g_nStringIdREQ };

const TForteInt16 GEN_ARRAY2VALUES::scm_anEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId GEN_ARRAY2VALUES::scm_anEventOutputNames[] = { g_nStringIdCNF };

GEN_ARRAY2VALUES::GEN_ARRAY2VALUES(const CStringDictionary::TStringId pa_nInstanceNameId,
    CResource *pa_poSrcRes) :
    CFunctionBlock(pa_poSrcRes, 0, pa_nInstanceNameId, 0, 0){
}

GEN_ARRAY2VALUES::~GEN_ARRAY2VALUES(){
  delete[] m_anDataOutputNames;
  delete[] m_anDataInputTypeIds;
  delete[] m_anDataOutputTypeIds;
  delete[] m_anEOWith;
}

void GEN_ARRAY2VALUES::executeEvent(int pa_nEIID){
  switch (pa_nEIID){
    case scm_nEventREQID:

      for(int output_index = 0; output_index < m_nDOutputs; output_index++){
        //copy input values to array
        getDO(output_index)->saveAssign(*(IN_Array()[output_index]));
      }

      sendOutputEvent(scm_nEventCNFID);

      break;
  }
}

bool GEN_ARRAY2VALUES::configureFB(const char *pa_acConfigString){
  bool bRetVal = false;

  m_nConfiguredFBTypeNameId = CStringDictionary::getInstance().insert(pa_acConfigString);
  const char *dTypePos = 0;
  const char *dNumberPos = strchr(pa_acConfigString, '_');

  if(0 != dNumberPos){
    ++dNumberPos;
    //get position of a second underscore
    dTypePos = strchr(dNumberPos, '_');

    if(0 != dTypePos){
      //there is a number and a data type of inputs within the typename
      m_nDOutputs = forte::core::util::strtoul(dNumberPos, 0, 10);

      int nLen = strlen(++dTypePos);
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
      m_nDOutputs = 0;
    }
  }
  else{
    return false;
  }

  if(m_ValueTypeID != CStringDictionary::scm_nInvalidStringId && m_nDOutputs >= 2){
    //create the data outputs
    m_anDataOutputNames = new CStringDictionary::TStringId[m_nDOutputs];
    m_anDataOutputTypeIds = new CStringDictionary::TStringId[m_nDOutputs];

    char doNames[cg_nIdentifierLength] = { "OUT_" };
    for(int doIndex = 0; doIndex < m_nDOutputs; doIndex = doIndex + 1){
#ifdef WIN32
      _snprintf(&(doNames[4]), 8 - 4, "%i", doIndex+1);
#else
      snprintf(&(doNames[4]), 8 - 4, "%i", doIndex + 1);
#endif
      m_anDataOutputNames[doIndex] = CStringDictionary::getInstance().insert(doNames);
      m_anDataOutputTypeIds[doIndex] = m_ValueTypeID;
    }

    //create data input type
    m_anDataInputTypeIds = new CStringDictionary::TStringId[3];
    m_anDataInputTypeIds[0] = g_nStringIdARRAY;
    m_anDataInputTypeIds[1] = m_nDOutputs;
    m_anDataInputTypeIds[2] = m_ValueTypeID;

    //get input with-indices for the data vars
    m_anEOWith = new TDataIOID[m_nDOutputs + 1];

    //in-withs
    for(int out_with = 0; out_with < m_nDOutputs + 1; out_with = out_with + 1){
      if(out_with == m_nDOutputs){
        //set end separator of with
        m_anEOWith[out_with] = 255;
      }
      else{
        m_anEOWith[out_with] = static_cast<TDataIOID>(out_with);
      }
    }

    //create the interface Specification
    SFBInterfaceSpecforGenerics *pstInterfaceSpec =
        new SFBInterfaceSpecforGenerics(1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 1, scm_anEventOutputNames, m_anEOWith, scm_anEOWithIndexes, 1, scm_anDataInputNames, m_anDataInputTypeIds, m_nDOutputs, m_anDataOutputNames, m_anDataOutputTypeIds);

    TForteByte *acFBConnData =
        new TForteByte[genFBConnDataSize(pstInterfaceSpec->m_nNumEOs, pstInterfaceSpec->m_nNumDIs, pstInterfaceSpec->m_nNumDOs)];
    TForteByte *acFBVarsData =
        new TForteByte[genFBVarsDataSize(pstInterfaceSpec->m_nNumDIs, pstInterfaceSpec->m_nNumDOs)];

    setupFBInterface(pstInterfaceSpec, acFBConnData, acFBVarsData, true);
    bRetVal = true;
  }

  return bRetVal;
}

