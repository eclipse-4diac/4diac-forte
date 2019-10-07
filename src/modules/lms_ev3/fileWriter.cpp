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
 
#include "fileWriter.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "fileWriter_gen.cpp"
#endif

#include <sstream>
#include <ostream>
#include <iostream>

DEFINE_FIRMWARE_FB(fileWriter, g_nStringIdfileWriter)

const CStringDictionary::TStringId fileWriter::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdFILE_NAME, g_nStringIdS1, g_nStringIdAPPEND, g_nStringIdAPPCHAR};

const CStringDictionary::TStringId fileWriter::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdBOOL, g_nStringIdSTRING};

const CStringDictionary::TStringId fileWriter::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId fileWriter::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 fileWriter::scm_anEIWithIndexes[] = {0, 3};
const TDataIOID fileWriter::scm_anEIWith[] = {0, 1, 255, 0, 2, 3, 4, 255};
const CStringDictionary::TStringId fileWriter::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};

const TDataIOID fileWriter::scm_anEOWith[] = {0, 1, 255, 0, 1, 255};
const TForteInt16 fileWriter::scm_anEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId fileWriter::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF};

const SFBInterfaceSpec fileWriter::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,
  5,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

const char * const fileWriter::scmOK = "OK";
const char * const fileWriter::scmNotInitialised = "Not initialized";
const char * const fileWriter::scmCouldNotWrite = "Could not write";

void fileWriter::executeEvent(int pa_nEIID){ //TODO: manage output and status
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
        QO() = writeFile();
      }
      sendOutputEvent(scm_nEventCNFID);
      if (false == QO()){
        std::cout << "----------ERROR\n";
      }

      break;
  }
}

fileWriter::fileWriter(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CFunctionBlock(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData){
  mFile.rdbuf()->pubsetbuf(NULL, 0); //disable buffer to avoid latency
}

bool fileWriter::openFile(){
  int retVal = false;
  std::string sysFileName(FILE_NAME().getValue());
  if(sysFileName.size()){
    mFile.open(sysFileName.c_str(), std::fstream::out); //TODO change this when fully switching to C++11 for LMS EV3
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

void fileWriter::closeFile(){
  if(mFile.is_open()){
    mFile.close();
  }
}

bool fileWriter::writeFile(){
  bool retVal = false;
  if(mFile.is_open()){
    std::string toWrite = "";
    if (APPEND()){
      toWrite = APPCHAR().getValue();
    } else {
      mFile.seekp(0, std::ios::beg);
    }
    mFile.clear();
    toWrite += S1().getValue();
    mFile << toWrite;
    if (mFile.fail()){
      STATUS() = scmCouldNotWrite;
    }else{
      STATUS() = scmOK;
      retVal = true;
    }
  }
  return retVal;
}

