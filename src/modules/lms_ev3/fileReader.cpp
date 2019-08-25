/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/
 
#include "fileReader.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "fileReader_gen.cpp"
#endif

#include <sstream>
#include <ostream>
#include <iostream>

DEFINE_FIRMWARE_FB(fileReader, g_nStringIdfileReader)

const CStringDictionary::TStringId fileReader::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdFILE_NAME};

const CStringDictionary::TStringId fileReader::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING};

const CStringDictionary::TStringId fileReader::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS, g_nStringIdS1};

const CStringDictionary::TStringId fileReader::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdSTRING};

const TForteInt16 fileReader::scm_anEIWithIndexes[] = {0, 3};
const TDataIOID fileReader::scm_anEIWith[] = {0, 1, 255, 0, 255};
const CStringDictionary::TStringId fileReader::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};

const TDataIOID fileReader::scm_anEOWith[] = {0, 1, 2, 255, 0, 1, 2, 255};
const TForteInt16 fileReader::scm_anEOWithIndexes[] = {0, 4, -1};
const CStringDictionary::TStringId fileReader::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF};

const SFBInterfaceSpec fileReader::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,
  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

const char * const fileReader::scmOK = "OK";
const char * const fileReader::scmNotInitialised = "Not initialized";
const char * const fileReader::scmCouldNotRead = "Could not read";

void fileReader::executeEvent(int pa_nEIID){ //TODO: manage output and status
  switch (pa_nEIID){
    case scm_nEventINITID:
      if(true == QI()){
        QO() = openFile();
      }
      else{
        closeFile();
      }
      sendOutputEvent(scm_nEventINITOID);
      break;
    case scm_nEventREQID:
      QO() = QI();
      if(true == QI()){
        QO() = readFile();
      }
      sendOutputEvent(scm_nEventCNFID);
      if (false == QO()){
        std::cout << "----------ERROR\n";
      }
      break;
  }
}

fileReader::fileReader(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CFunctionBlock(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData){
  mFile.rdbuf()->pubsetbuf(NULL, 0); //disable buffer to avoid latency
}

bool fileReader::openFile(){
  int retVal = false;
  std::string sysFileName(FILE_NAME().getValue());
  if(sysFileName.size()){
    mFile.open(sysFileName.c_str(), std::fstream::in); //TODO change this when fully switching to C++11 for LMS EV3
    if(mFile.is_open()){
      retVal = true;
    }else{
      STATUS() = scmNotInitialised;
    }
  }else{
    STATUS() = scmNotInitialised;
  }
  return retVal;
}

void fileReader::closeFile(){
  if(mFile.is_open()){
    mFile.close();
  }
}

bool fileReader::readFile(){
  bool retVal = false;
  if(mFile.is_open()){
    std::string read = "";
    mFile.clear();
    mFile.seekg(0, std::ios::beg);
    mFile >> read;
    if (mFile.fail()){
      STATUS() = scmCouldNotRead;
    }else{
      S1() = read.c_str();
      STATUS() = scmOK;
      retVal = true;
    }
  }else{
    STATUS() = scmNotInitialised;
  }
  return retVal;
}

