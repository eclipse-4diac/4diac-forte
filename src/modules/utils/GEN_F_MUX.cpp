/*******************************************************************************
 * Copyright (c) 2012, 2023 Profactor GmbH, ACIN
 *                          Johannes Kepler University
 *                          Martin Erich Jobst
 *
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
 *    Martin Jobst - add generic readInputData and writeOutputData
 *******************************************************************************/
#include "GEN_F_MUX.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_F_MUX_gen.cpp"
#endif

#include <ctype.h>
#include <stdio.h>

#include "forte_printer.h"
#include "resource.h"
#include "criticalregion.h"

DEFINE_GENERIC_FIRMWARE_FB(GEN_F_MUX, g_nStringIdGEN_F_MUX);

const CStringDictionary::TStringId GEN_F_MUX::scm_anEventOutputNames[] = { g_nStringIdEO };

GEN_F_MUX::GEN_F_MUX(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(paSrcRes, paInstanceNameId),
  m_anEventInputNames(nullptr),
  m_anDataOutputNames(nullptr),
  m_anDataInputNames(nullptr),
  m_anDataOutputTypeIds(nullptr),
  m_anDataInputTypeIds(nullptr),
  mEInputs(0),
  mEOutputs(0),
  mDInputs(0),
  mDOutputs(0){
}

GEN_F_MUX::~GEN_F_MUX(){
  delete[] m_anEventInputNames;
  delete[] m_anDataInputNames;
  delete[] m_anDataInputTypeIds;
  delete[] m_anDataOutputNames;
  delete[] m_anDataOutputTypeIds;
}

void GEN_F_MUX::executeEvent(TEventID paEIID){

  if(static_cast<size_t>(paEIID) < mEInputs) {

    size_t startIndex = paEIID * mDOutputs;
    bool status = true;

    for(size_t input_index = startIndex, output_index = 2; input_index < startIndex + mDOutputs; input_index++, output_index++) {

      CIEC_ANY *p_dataInput = getDI(static_cast<unsigned int>(input_index));
      CIEC_ANY *p_dataOutput = getDO(static_cast<unsigned int>(output_index));

      // check whether datatypes match
      if(p_dataInput != nullptr && p_dataOutput != nullptr && p_dataInput->getDataTypeID() == p_dataOutput->getDataTypeID()){

        p_dataOutput->setValue(*p_dataInput);
        status = true && status;
      }
      else{
        status = false;
      }
    }

    if(status == true){
      *static_cast<CIEC_BOOL*>(getDO(0)) = CIEC_BOOL(true);
      *static_cast<CIEC_WSTRING *>(getDO(1)) = CIEC_WSTRING("OK");
    }
    else{
      *static_cast<CIEC_BOOL*>(getDO(0)) = CIEC_BOOL(false);
      *static_cast<CIEC_WSTRING *>(getDO(1)) = CIEC_WSTRING("Datatype ERROR");
    }

    sendOutputEvent(0);
  }
}

void GEN_F_MUX::readInputData(TEventID paEI) {
  RES_DATA_CON_CRITICAL_REGION();
  for(TPortId i = 0; i < mDOutputs; ++i) {
    TPortId index = paEI * mDOutputs + i;
    readData(index, *mDIs[index], mDIConns[index]);
  }
}

void GEN_F_MUX::writeOutputData(TEventID) {
  RES_DATA_CON_CRITICAL_REGION();
  for(TPortId i = 0; i < mInterfaceSpec->m_nNumDOs; ++i) {
    writeData(i, *mDOs[i], mDOConns[i]);
  }
}

bool GEN_F_MUX::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  int index = 0;
  char *paramEI = nullptr;
  char *paramDO = nullptr;
  char baseName[cg_nIdentifierLength];

  TIdentifier typeIdString;

  memcpy(typeIdString, paConfigString, (strlen(paConfigString) > cg_nIdentifierLength) ? cg_nIdentifierLength : strlen(paConfigString) + 1); //plus 1 for the null character
  typeIdString[cg_nIdentifierLength] = '\0';

  size_t inlength = strlen(typeIdString);

  for(index = 0; index < (int) inlength - 1; index++){

    if(typeIdString[index] == '_'){

      //make sure that there is no invalid index
      if(index - 1 >= 0){
        memcpy(&baseName[0], &typeIdString[index - 1], 5); // F_MUX ... prefix has 5 characters
        baseName[5] = '\0';

        //check if the prefix F_MUX exists; in this case, the next "_" separates the parameters
        if(strcmp(baseName, "F_MUX") == 0){
          continue;
        }
      }
      else{
        //error on creating the FB; this would mean that the Typename starts with "_"
        return false;
      }

      paramEI = paramDO = &(typeIdString[index + 1]);
      break;
    }
  }
  if(paramDO != nullptr){

    for(index = index + 1; index < (int) inlength - 1; index++){

      if(typeIdString[index] == '_'){

        typeIdString[index] = '\0';
        paramDO = &(typeIdString[index + 1]);
        break;
      }
    }
  }

  if(paramDO == nullptr){
    return false;
  }
  else{
    //set the data and event port numbers
    mEInputs = static_cast<size_t>(forte::core::util::strtoul(paramEI, nullptr, 10));
    mDOutputs = static_cast<size_t>(forte::core::util::strtoul(paramDO, nullptr, 10));
    mEOutputs = 1;
    mDInputs = mEInputs * mDOutputs;

    DEVLOG_DEBUG("EIs: %d; DIs: %d; EOs: %d; DOs: %d \n", mEInputs, mDInputs, mEOutputs, mDOutputs);

    //return with error if there are not enough event inputs (use common merge FB instead!!)
    if(mEInputs < 2){
      DEVLOG_ERROR("At least 2 Event Inputs need to be set\n");
      return false;
    }
  }

  //now the number of needed eventInputs and dataOutputs are available in the integer array
  //create the eventInputs
  if(mEInputs < CFunctionBlock::scm_nMaxInterfaceEvents && mDInputs < CFunctionBlock::scm_nMaxInterfaceEvents){
    //create the eventInputs
    m_anEventInputNames = new CStringDictionary::TStringId[mEInputs];

    generateGenericInterfacePointNameArray("EI", m_anEventInputNames,  mEInputs);

    //create the data inputs
    m_anDataInputNames = new CStringDictionary::TStringId[mDInputs];
    m_anDataInputTypeIds = new CStringDictionary::TStringId[mDInputs];
    char diNames[cg_nIdentifierLength] = { "IN_" };
    size_t di_posIndex = 0;
    for(size_t ei = 0; ei < mEInputs; ei++) {

      for(size_t di = 0; di < mDOutputs; di++) {
        forte_snprintf(&(diNames[3]), 11 - 3, "%u_%u", ei + 1, di + 1);
        di_posIndex = ei * mDOutputs + di;
        m_anDataInputNames[di_posIndex] = CStringDictionary::getInstance().insert(diNames);
        m_anDataInputTypeIds[di_posIndex] = g_nStringIdANY;
      }
    }

    //create the data outputs
    m_anDataOutputNames = new CStringDictionary::TStringId[mDOutputs + 2];
    m_anDataOutputTypeIds = new CStringDictionary::TStringId[mDOutputs + 2];

    //data outputs for status and QO
    m_anDataOutputNames[0] = CStringDictionary::getInstance().insert("QO");
    m_anDataOutputTypeIds[0] = g_nStringIdBOOL;
    m_anDataOutputNames[1] = CStringDictionary::getInstance().insert("STATUS");
    m_anDataOutputTypeIds[1] = g_nStringIdWSTRING;

    generateGenericDataPointArrays("OUT_", &(m_anDataOutputTypeIds[2]), &(m_anDataOutputNames[2]), mDOutputs);

    //create the interface Specification
    paInterfaceSpec.m_nNumEIs = mEInputs;
    paInterfaceSpec.m_aunEINames = m_anEventInputNames;
    paInterfaceSpec.m_nNumEOs = mEOutputs;
    paInterfaceSpec.m_aunEONames = scm_anEventOutputNames;
    paInterfaceSpec.m_nNumDIs = mDInputs;
    paInterfaceSpec.m_aunDINames = m_anDataInputNames;
    paInterfaceSpec.m_aunDIDataTypeNames = m_anDataInputTypeIds;
    paInterfaceSpec.m_nNumDOs = mDOutputs + 2;
    paInterfaceSpec.m_aunDONames = m_anDataOutputNames;
    paInterfaceSpec.m_aunDODataTypeNames = m_anDataOutputTypeIds;
    return true;
  }
  return false;
}
