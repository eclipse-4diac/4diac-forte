/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH, fortiss GmbH
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
#include "GEN_ARRAY2VALUES.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_ARRAY2VALUES_gen.cpp"
#endif

#include <stdio.h>
#include "forte_printer.h"
#include "resource.h"
#include "criticalregion.h"

DEFINE_GENERIC_FIRMWARE_FB(GEN_ARRAY2VALUES, g_nStringIdGEN_ARRAY2VALUES)

const CStringDictionary::TStringId GEN_ARRAY2VALUES::scm_anDataInputNames[] = { g_nStringIdIN };

const CStringDictionary::TStringId GEN_ARRAY2VALUES::scm_anEventInputNames[] = { g_nStringIdREQ };

const CStringDictionary::TStringId GEN_ARRAY2VALUES::scm_anEventOutputNames[] = { g_nStringIdCNF };

GEN_ARRAY2VALUES::GEN_ARRAY2VALUES(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(paSrcRes, paInstanceNameId), m_anDataOutputNames(nullptr), m_anDataOutputTypeIds(nullptr), m_anDataInputTypeIds(nullptr), mDOutputs(0), mValueTypeID(CStringDictionary::scm_nInvalidStringId){
}

GEN_ARRAY2VALUES::~GEN_ARRAY2VALUES(){
  delete[] m_anDataOutputNames;
  delete[] m_anDataInputTypeIds;
  delete[] m_anDataOutputTypeIds;
}

void GEN_ARRAY2VALUES::executeEvent(TEventID paEIID){
  switch (paEIID){
    case scm_nEventREQID:

      for(size_t output_index = 0; output_index < mDOutputs; output_index++) {
        //copy input values to array
        getDO(static_cast<unsigned int>(output_index))->setValue((IN_Array()[output_index]));
      }

      sendOutputEvent(scm_nEventCNFID);

      break;
  }
}

void GEN_ARRAY2VALUES::readInputData(TEventID) {
  RES_DATA_CON_CRITICAL_REGION();
  readData(0, *mDIs[0], mDIConns[0]);
}

void GEN_ARRAY2VALUES::writeOutputData(TEventID) {
  RES_DATA_CON_CRITICAL_REGION();
  for(TPortId i = 0; i < mInterfaceSpec->m_nNumDOs; ++i) {
    writeData(i, *mDOs[i], mDOConns[i]);
  }
}

bool GEN_ARRAY2VALUES::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec){
  const char *dNumberPos = strchr(paConfigString, '_');

  if(nullptr != dNumberPos){
    ++dNumberPos;
    //get position of a second underscore
    const char *dTypePos = strchr(dNumberPos, '_');

    if(nullptr != dTypePos){
      //there is a number and a data type of inputs within the typename
      mDOutputs = static_cast<size_t>(forte::core::util::strtoul(dNumberPos, nullptr, 10));
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

    //create the interface Specification
    paInterfaceSpec.m_nNumEIs = 1;
    paInterfaceSpec.m_aunEINames = scm_anEventInputNames;
    paInterfaceSpec.m_nNumEOs = 1;
    paInterfaceSpec.m_aunEONames = scm_anEventOutputNames;
    paInterfaceSpec.m_nNumDIs = 1;
    paInterfaceSpec.m_aunDINames = scm_anDataInputNames;
    paInterfaceSpec.m_aunDIDataTypeNames = m_anDataInputTypeIds;
    paInterfaceSpec.m_nNumDOs = mDOutputs;
    paInterfaceSpec.m_aunDONames = m_anDataOutputNames;
    paInterfaceSpec.m_aunDODataTypeNames = m_anDataOutputTypeIds;
    return true;
  }

  return false;
}

