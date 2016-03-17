/*******************************************************************************
 * Copyright (c) 2012 - 2013 Profactor GmbH, ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Matthias Plasch, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "GEN_F_MUX.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_F_MUX_gen.cpp"
#endif
#include <ctype.h>
#include <stdio.h>

DEFINE_GENERIC_FIRMWARE_FB(GEN_F_MUX, g_nStringIdGEN_F_MUX);

const CStringDictionary::TStringId GEN_F_MUX::scm_anEventOutputNames[] = { g_nStringIdEO };

GEN_F_MUX::GEN_F_MUX(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CFunctionBlock(pa_poSrcRes, 0, pa_nInstanceNameId, 0, 0){
}

GEN_F_MUX::~GEN_F_MUX(){
  delete[] m_anEventInputNames;
  delete[] m_anDataInputNames;
  delete[] m_anDataInputTypeIds;
  delete[] m_anDataOutputNames;
  delete[] m_anDataOutputTypeIds;
  delete[] m_anEIWith;
  delete[] m_anEIWithIndexes;
  delete[] m_anEOWith;
  delete[] m_anEOWithIndexes;
}

void GEN_F_MUX::executeEvent(int pa_nEIID){

  if(pa_nEIID < m_nEInputs && pa_nEIID > -1){

    int startIndex = pa_nEIID * m_nDOutputs;
    bool status = true;

    for(int input_index = startIndex, output_index = 2; input_index < startIndex + m_nDOutputs; input_index++, output_index++){

      CIEC_ANY *p_dataInput = getDI(input_index);
      CIEC_ANY *p_dataOutput = getDO(output_index);

      // check whether datatypes match
      if(p_dataInput != 0 && p_dataOutput != 0 && p_dataInput->getDataTypeID() == p_dataOutput->getDataTypeID()){

        p_dataOutput->setValue(*p_dataInput);
        status = true && status;
      }
      else{
        status = false;
      }
    }

    if(status == true){
      *static_cast<CIEC_BOOL*>(getDO(0)) = true;
      *static_cast<CIEC_WSTRING *>(getDO(1)) = "OK";
    }
    else{
      *static_cast<CIEC_BOOL*>(getDO(0)) = false;
      *static_cast<CIEC_WSTRING *>(getDO(1)) = "Datatype ERROR";
    }

    sendOutputEvent(0);
  }
}

bool GEN_F_MUX::configureFB(const char *pa_acConfigString){
  bool bRetVal = false;

  int index = 0;
  char *paramEI = 0;
  char *paramDO = 0;
  char baseName[cg_nIdentifierLength];

  TIdentifier typeIdString;
  size_t inlength;

  m_nConfiguredFBTypeNameId = CStringDictionary::getInstance().insert(pa_acConfigString);

  memcpy(typeIdString, pa_acConfigString, cg_nIdentifierLength);

  typeIdString[cg_nIdentifierLength] = '\0'; //make a string

  inlength = strlen(typeIdString);

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
  if(paramDO != 0){

    for(index = index + 1; index < (int) inlength - 1; index++){

      if(typeIdString[index] == '_'){

        typeIdString[index] = '\0';
        paramDO = &(typeIdString[index + 1]);
        break;
      }
    }
  }

  if(paramDO == 0){

    return false;
  }
  else{
    //set the data and event port numbers
    m_nEInputs = forte::core::util::strtoul(paramEI,0,10);
    m_nDOutputs = forte::core::util::strtoul(paramDO,0,10);
    m_nEOutputs = 1;
    m_nDInputs = m_nEInputs * m_nDOutputs;

    DEVLOG_DEBUG("EIs: %d; DIs: %d; EOs: %d; DOs: %d \n", m_nEInputs, m_nDInputs, m_nEOutputs, m_nDOutputs);

    //return with error if there are not enough event inputs (use common merge FB instead!!)
    if(m_nEInputs < 2){
	  DEVLOG_ERROR("At least 2 Event Inputs need to be set\n");
      return false;
    }
  }

  //now the number of needed eventInputs and dataOutputs are available in the integer array
  //create the eventInputs
  if(m_nEInputs < CFunctionBlock::scm_nMaxInterfaceEvents && m_nDInputs < CFunctionBlock::scm_nMaxInterfaceEvents){
    //create the eventInputs
    m_anEventInputNames = new CStringDictionary::TStringId[m_nEInputs];

    generateGenericInterfacePointNameArray("EI", m_anEventInputNames,  m_nEInputs);

    //create the data inputs
    m_anDataInputNames = new CStringDictionary::TStringId[m_nDInputs];
    m_anDataInputTypeIds = new CStringDictionary::TStringId[m_nDInputs];
    char diNames[cg_nIdentifierLength] = { "IN_" };
    int di_posIndex = 0;
    for(int ei = 0; ei < m_nEInputs; ei = ei + 1){

      for(int di = 0; di < m_nDOutputs; di = di + 1){

#ifdef WIN32
        _snprintf(&(diNames[3]), 11 - 3, "%i_%i", ei+1, di+1);
#else
        snprintf(&(diNames[3]), 11 - 3, "%i_%i", ei + 1, di + 1);
#endif
        di_posIndex = ei * m_nDOutputs + di;
        m_anDataInputNames[di_posIndex] = CStringDictionary::getInstance().insert(diNames);
        m_anDataInputTypeIds[di_posIndex] = g_nStringIdANY;
      }
    }

    //create the data outputs
    m_anDataOutputNames = new CStringDictionary::TStringId[m_nDOutputs + 2];
    m_anDataOutputTypeIds = new CStringDictionary::TStringId[m_nDOutputs + 2];

    //data outputs for status and QO
    m_anDataOutputNames[0] = CStringDictionary::getInstance().insert("QO");
    m_anDataOutputTypeIds[0] = g_nStringIdBOOL;
    m_anDataOutputNames[1] = CStringDictionary::getInstance().insert("STATUS");
    m_anDataOutputTypeIds[1] = g_nStringIdWSTRING;

    generateGenericDataPointArrays("OUT_", &(m_anDataOutputTypeIds[2]), &(m_anDataOutputNames[2]), m_nDOutputs);

    //now create the WITH constructs...
    //first the With-Indexes Events
    m_anEIWithIndexes = new TForteInt16[m_nEInputs];
    m_anEOWithIndexes = new TForteInt16[2]; //contains terminating -1 value

    for(int ei_index = 0; ei_index < m_nEInputs; ei_index = ei_index + 1){
      if(ei_index == 0){
        m_anEIWithIndexes[ei_index] = 0;
      }
      else{
        m_anEIWithIndexes[ei_index] = ei_index * (m_nDOutputs + 1);
      }
    }

    m_anEOWithIndexes[0] = 0;
    m_anEOWithIndexes[1] = -1;

    //second the With-Indexes for the data variables
    m_anEIWith = new TDataIOID[m_nDInputs + m_nEInputs]; //for inputs per event + '255' separators between withs
    m_anEOWith = new TDataIOID[m_nDOutputs + 2 + 1]; //for outputs only one '255' separator since one output event is needed + 2 for QO and STATUS

    //in-withs
    int withListIndex = 0;
    int dataIndex = 0;
    for(int in_block = 0; in_block < m_nEInputs; in_block = in_block + 1){

      for(int in_with = 0; in_with < m_nDOutputs; in_with = in_with + 1){
        m_anEIWith[withListIndex] = static_cast<TDataIOID>(dataIndex);
        withListIndex++;
        dataIndex++;
      }

      //set '255' separator
      m_anEIWith[withListIndex] = 255;
      withListIndex++;
    }

    //out-withs
    m_anEOWith[0] = 0; //for QO and STATUS
    m_anEOWith[1] = 1;

    for(int out_with = 2; out_with < m_nDOutputs + 2; out_with = out_with + 1){
      m_anEOWith[out_with] = static_cast<TForteUInt8>(out_with);
    }
    //set '255' separator
    m_anEOWith[m_nDOutputs + 2] = 255;

    //create the interface Specification
    SFBInterfaceSpecforGenerics *pstInterfaceSpec = new SFBInterfaceSpecforGenerics(m_nEInputs, m_anEventInputNames, m_anEIWith, m_anEIWithIndexes, m_nEOutputs, scm_anEventOutputNames, m_anEOWith, m_anEOWithIndexes, m_nDInputs, m_anDataInputNames, m_anDataInputTypeIds, m_nDOutputs + 2, m_anDataOutputNames, m_anDataOutputTypeIds);

    TForteByte *acFBConnData = new TForteByte[genFBConnDataSize(pstInterfaceSpec->m_nNumEOs, pstInterfaceSpec->m_nNumDIs, pstInterfaceSpec->m_nNumDOs)];
    TForteByte *acFBVarsData = new TForteByte[genFBVarsDataSize(pstInterfaceSpec->m_nNumDIs, pstInterfaceSpec->m_nNumDOs)];

    setupFBInterface(pstInterfaceSpec, acFBConnData, acFBVarsData, true);
    bRetVal = true;
  }
  return bRetVal;
}
