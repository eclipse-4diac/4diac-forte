/*******************************************************************************
 * Copyright (c) 2014 - 2015 Profactor GmbH, fortiss GmbH
 *                      2018 Johannes Kepler University
 *               2023 Martin Erich Jobst
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
 *   Martin Jobst
 *     - refactor for ANY variant
 *     - add generic readInputData and writeOutputData
 *******************************************************************************/
#include "GEN_VALUES2ARRAY.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_VALUES2ARRAY_gen.cpp"
#endif

#include <stdio.h>
#include "forte_printer.h"
#include "resource.h"
#include "criticalregion.h"

DEFINE_GENERIC_FIRMWARE_FB(GEN_VALUES2ARRAY, g_nStringIdGEN_VALUES2ARRAY)

const CStringDictionary::TStringId GEN_VALUES2ARRAY::scm_anDataOutputNames[] = { g_nStringIdOUT };

const CStringDictionary::TStringId GEN_VALUES2ARRAY::scm_anEventInputNames[] = { g_nStringIdREQ };

const CStringDictionary::TStringId GEN_VALUES2ARRAY::scm_anEventOutputNames[] = { g_nStringIdCNF };

GEN_VALUES2ARRAY::GEN_VALUES2ARRAY(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(paSrcRes, paInstanceNameId), m_anDataInputNames(nullptr), m_anDataInputTypeIds(nullptr), m_anDataOutputTypeIds(nullptr), m_nDInputs(0), m_ValueTypeID(CStringDictionary::CStringDictionary::scm_nInvalidStringId){
}

GEN_VALUES2ARRAY::~GEN_VALUES2ARRAY(){
  delete[] m_anDataInputNames;
  delete[] m_anDataInputTypeIds;
  delete[] m_anDataOutputTypeIds;
}

void GEN_VALUES2ARRAY::executeEvent(TEventID paEIID){
  switch (paEIID){
    case scm_nEventREQID:

      for(unsigned int input_index = 0; input_index < m_nDInputs; input_index++){
        //copy input values to array
        OUT_Array()[input_index].setValue(*getDI(input_index));
      }

      sendOutputEvent(scm_nEventCNFID);

      break;
  }
}

void GEN_VALUES2ARRAY::readInputData(TEventID) {
  RES_DATA_CON_CRITICAL_REGION();
  for(TPortId i = 0; i < mInterfaceSpec->m_nNumDIs; ++i) {
    readData(i, *mDIs[i], mDIConns[i]);
  }
}

void GEN_VALUES2ARRAY::writeOutputData(TEventID) {
  RES_DATA_CON_CRITICAL_REGION();
  writeData(0, *mDOs[0], mDOConns[0]);
}

bool GEN_VALUES2ARRAY::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec){
  const char *dNumberPos = strchr(paConfigString, '_');

  if(nullptr != dNumberPos){
    ++dNumberPos;
    //get position of a second underscore
    const char *dTypePos = strchr(dNumberPos, '_');

    if(nullptr != dTypePos){
      //there is a number and a data type of inputs within the typename
      m_nDInputs = static_cast<unsigned int>(forte::core::util::strtoul(dNumberPos, nullptr, 10));
      m_ValueTypeID = CStringDictionary::getInstance().getId(++dTypePos);
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
    for(unsigned int di = 0; di < m_nDInputs; di = di + 1){
      forte_snprintf(&(diNames[3]), 7 - 3, "%u", di + 1);
      m_anDataInputNames[di] = CStringDictionary::getInstance().insert(diNames);
      m_anDataInputTypeIds[di] = m_ValueTypeID;
    }

    //create data output type
    m_anDataOutputTypeIds = new CStringDictionary::TStringId[3];
    m_anDataOutputTypeIds[0] = g_nStringIdARRAY;
    m_anDataOutputTypeIds[1] = m_nDInputs;
    m_anDataOutputTypeIds[2] = m_ValueTypeID;

    //create the interface Specification
    paInterfaceSpec.m_nNumEIs = 1;
    paInterfaceSpec.m_aunEINames = scm_anEventInputNames;
    paInterfaceSpec.m_nNumEOs = 1;
    paInterfaceSpec.m_aunEONames = scm_anEventOutputNames;
    paInterfaceSpec.m_nNumDIs = m_nDInputs;
    paInterfaceSpec.m_aunDINames = m_anDataInputNames;
    paInterfaceSpec.m_aunDIDataTypeNames = m_anDataInputTypeIds;
    paInterfaceSpec.m_nNumDOs = 1;
    paInterfaceSpec.m_aunDONames = scm_anDataOutputNames;
    paInterfaceSpec.m_aunDODataTypeNames = m_anDataOutputTypeIds;
    return true;
  }
  return false;
}

