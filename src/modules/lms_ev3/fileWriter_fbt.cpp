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
 
#include "fileWriter_fbt.h"

USE_STRING_ID(APPCHAR);
USE_STRING_ID(APPEND);
USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(FILE_NAME);
USE_STRING_ID(fileWriter);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(REQ);
USE_STRING_ID(S1);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);
USE_STRING_ID(WSTRING);


#include <sstream>
#include <ostream>
#include <iostream>

DEFINE_FIRMWARE_FB(fileWriter, STRID(fileWriter))

const CStringDictionary::TStringId fileWriter::scmDataInputNames[] = {STRID(QI), STRID(FILE_NAME), STRID(S1), STRID(APPEND), STRID(APPCHAR)};

const CStringDictionary::TStringId fileWriter::scmDataInputTypeIds[] = {STRID(BOOL), STRID(STRING), STRID(STRING), STRID(BOOL), STRID(STRING)};

const CStringDictionary::TStringId fileWriter::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};

const CStringDictionary::TStringId fileWriter::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};

const TForteInt16 fileWriter::scmEIWithIndexes[] = {0, 3};
const TDataIOID fileWriter::scmEIWith[] = {0, 1, scmWithListDelimiter, 0, 2, 3, 4, scmWithListDelimiter};
const CStringDictionary::TStringId fileWriter::scmEventInputNames[] = {STRID(INIT), STRID(REQ)};
const CStringDictionary::TStringId fileWriter::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};

const TDataIOID fileWriter::scmEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 fileWriter::scmEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId fileWriter::scmEventOutputNames[] = {STRID(INITO), STRID(CNF)};
const CStringDictionary::TStringId fileWriter::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};

const SFBInterfaceSpec fileWriter::scmFBInterfaceSpec = {
  2,  scmEventInputNames, scmEventInputTypeIds,  scmEIWith,  scmEIWithIndexes,
  2,  scmEventOutputNames, scmEventOutputTypeIds,  scmEOWith, scmEOWithIndexes,
  5,  scmDataInputNames, scmDataInputTypeIds,
  2,  scmDataOutputNames, scmDataOutputTypeIds,
  0, 0
};

const char * const fileWriter::scmOK = "OK";
const char * const fileWriter::scmNotInitialised = "Not initialized";
const char * const fileWriter::scmCouldNotWrite = "Could not write";

void fileWriter::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) { //TODO: manage output and status
  switch (paEIID){
    case scmEventINITID:
      if(true == QI()){
        QO() = openFile();
      }
      else{
        closeFile();
      }
      sendOutputEvent(scmEventINITOID, paECET);
      break;
    case scmEventREQID:
      QO() = QI();
      if(true == QI()){
        QO() = writeFile();
      }
      sendOutputEvent(scmEventCNFID, paECET);
      if (false == QO()){
        std::cout << "----------ERROR\n";
      }

      break;
  }
}

fileWriter::fileWriter(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId){
  mFile.rdbuf()->pubsetbuf(nullptr, 0); //disable buffer to avoid latency
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

