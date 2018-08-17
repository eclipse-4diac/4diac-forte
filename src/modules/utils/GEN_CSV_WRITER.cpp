/*******************************************************************************
 * Copyright (c) 2012 - 2015 ACIN, fortiss GmbH
 *                      2018 Johannes Kepler University
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *******************************************************************************/
#include "GEN_CSV_WRITER.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_CSV_WRITER_gen.cpp"
#endif
#include <string.h>
#include <errno.h>

DEFINE_GENERIC_FIRMWARE_FB(GEN_CSV_WRITER, g_nStringIdGEN_CSV_WRITER);

const CStringDictionary::TStringId GEN_CSV_WRITER::scm_anDataOutputNames[] = { g_nStringIdQO, g_nStringIdSTATUS };

const CStringDictionary::TStringId GEN_CSV_WRITER::scm_anDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdSTRING };

const CStringDictionary::TStringId GEN_CSV_WRITER::scm_anEventInputNames[] = { g_nStringIdINIT, g_nStringIdREQ };

const TForteInt16 GEN_CSV_WRITER::scm_anEIWithIndexes[] = { 0, 3 };
const TDataIOID GEN_CSV_WRITER::scm_anEOWith[] = { 0, 1, 255, 0, 1, 255 };
const TForteInt16 GEN_CSV_WRITER::scm_anEOWithIndexes[] = { 0, 3, -1 };
const CStringDictionary::TStringId GEN_CSV_WRITER::scm_anEventOutputNames[] = { g_nStringIdINITO, g_nStringIdCNF };

void GEN_CSV_WRITER::executeEvent(int pa_nEIID){
  switch (pa_nEIID){
    case scm_nEventINITID:
      if(true == QI()){
        openCSVFile();
      }
      else{
        closeCSVFile();
      }
      sendOutputEvent(scm_nEventINITOID);
      break;
    case scm_nEventREQID:
      QO() = QI();
      if(true == QI()){
        writeCSVFileLine();
      }
      sendOutputEvent(scm_nEventREQID);
      break;
  }
}

GEN_CSV_WRITER::GEN_CSV_WRITER(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(pa_poSrcRes, pa_nInstanceNameId), m_pstCSVFile(0), m_anDataInputNames(0), m_anDataInputTypeIds(0), m_anEIWith(0){
}

GEN_CSV_WRITER::~GEN_CSV_WRITER(){
  delete[] m_anDataInputNames;
  delete[] m_anDataInputTypeIds;
  delete[] m_anEIWith;
}

SFBInterfaceSpecforGenerics *GEN_CSV_WRITER::createInterfaceSpec(const char *paConfigString) {
  SFBInterfaceSpecforGenerics *interfaceSpec = 0;


  const char *acPos = strrchr(paConfigString, '_');
  if(0 != acPos){
    acPos++;
    int nNumDIs = static_cast<int>(forte::core::util::strtoul(acPos,0,10));
    nNumDIs += 2; // we have in addition to the SDs a QI and FILE_NAME data inputs

    m_anDataInputNames = new CStringDictionary::TStringId[nNumDIs];
    m_anDataInputTypeIds = new CStringDictionary::TStringId[nNumDIs];

    m_anDataInputNames[0] = g_nStringIdQI;
    m_anDataInputTypeIds[0] = g_nStringIdBOOL;
    m_anDataInputNames[1] = g_nStringIdFILE_NAME;
    m_anDataInputTypeIds[1] = g_nStringIdSTRING;

    generateGenericDataPointArrays("SD_", &(m_anDataInputTypeIds[2]), &(m_anDataInputNames[2]), nNumDIs - 2);

    m_anEIWith = new TDataIOID[3 + nNumDIs];

    m_anEIWith[0] = 0;
    m_anEIWith[1] = 1;
    m_anEIWith[2] = 255;
    m_anEIWith[3] = 0;

    for(TDataIOID i = 2; i < (nNumDIs); i++){
      m_anEIWith[i + 2] = i;
    }

    m_anEIWith[2 + nNumDIs] = 255;

    //create the interface Specification
    interfaceSpec = new SFBInterfaceSpecforGenerics((TForteUInt8)2, scm_anEventInputNames, m_anEIWith, scm_anEIWithIndexes, (TForteUInt8)2, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, (TForteUInt8)nNumDIs, m_anDataInputNames, m_anDataInputTypeIds, (TForteUInt8)2, scm_anDataOutputNames, scm_anDataOutputTypeIds);
  }
  return interfaceSpec;
}

void GEN_CSV_WRITER::openCSVFile(){
  QO() = false;
  if(0 == m_pstCSVFile){
    m_pstCSVFile = fopen(FILE_NAME().getValue(), "w+");
    if(0 != m_pstCSVFile){
      STATUS() = "OK";
      QO() = true;
    }
    else{
      STATUS() = strerror(errno);
    }
  }
  else{
    STATUS() = "File already open";
  }
}

void GEN_CSV_WRITER::closeCSVFile(){
  QO() = false;
  if(0 != m_pstCSVFile){
    if(0 == fclose(m_pstCSVFile)){
      STATUS() = "OK";
    }
    else{
      STATUS() = strerror(errno);
    }
    m_pstCSVFile = 0;
  }
}

void GEN_CSV_WRITER::writeCSVFileLine(){
  if(0 != m_pstCSVFile){
    char acBuffer[100];
    for(int i = 2; i < m_pstInterfaceSpec->m_nNumDIs; i++){
      int nLen = getDI(i)->toString(acBuffer, 100);
      fwrite(acBuffer, 1, nLen, m_pstCSVFile);
      fwrite("; ", 1, 2, m_pstCSVFile);
    }
    fwrite("\n", 1, 1, m_pstCSVFile);
  }
  else{
    QO() = false;
    STATUS() = "File not opened";
  }
}

