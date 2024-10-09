/*******************************************************************************
 * Copyright (c) 2012, 2024 ACIN, fortiss GmbH,  Johannes Kepler University Linz,
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Monika Wenger, Martin Jobst - copy and modified from
 *                                              GEN_CSV_READER
 *******************************************************************************/
#include "GEN_CSV_READER_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_CSV_READER_fbt_gen.cpp"
#endif
#include <errno.h>
#include "devlog.h"
#include "resource.h"
#include "criticalregion.h"

DEFINE_GENERIC_FIRMWARE_FB(GEN_CSV_READER, g_nStringIdGEN_CSV_READER);

const CStringDictionary::TStringId GEN_CSV_READER::scmDataInputNames[] = { g_nStringIdQI, g_nStringIdFILE_NAME };

const CStringDictionary::TStringId GEN_CSV_READER::scmDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdSTRING };

const CStringDictionary::TStringId GEN_CSV_READER::scmEventInputNames[] = { g_nStringIdINIT, g_nStringIdREQ };

const CStringDictionary::TStringId GEN_CSV_READER::scmEventOutputNames[] = { g_nStringIdINITO, g_nStringIdCNF };

const CIEC_STRING GEN_CSV_READER::scmOK = "OK"_STRING;
const CIEC_STRING GEN_CSV_READER::scmFileAlreadyOpened = "File already opened"_STRING;
const CIEC_STRING GEN_CSV_READER::scmFileNotOpened = "File not opened"_STRING;

void GEN_CSV_READER::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  if(scmEventINITID == paEIID) {
    if(QI()) {
      openCSVFile();
    } else {
      closeCSVFile();
    }
    sendOutputEvent(scmEventINITOID, paECET);
  } else if(scmEventREQID == paEIID) {
    QO() = QI();
    if(QI()) {
      readCSVFileLine();
    }
    sendOutputEvent(scmEventCNFID, paECET);
  }
}

GEN_CSV_READER::GEN_CSV_READER(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId), mDataOutputNames(nullptr), mDataOutputTypeIds(nullptr) {
}

GEN_CSV_READER::~GEN_CSV_READER(){
  delete[] mDataOutputNames;
  delete[] mDataOutputTypeIds;
  closeCSVFile();
}

void GEN_CSV_READER::readInputData(TEventID paEI) {
  switch(paEI) {
    case scmEventINITID: {
      readData(0, *mDIs[0], mDIConns[0]);
      readData(1, *mDIs[1], mDIConns[1]);
      break;
    }
    case scmEventREQID: {
      readData(0, *mDIs[0], mDIConns[0]);
      break;
    }
    default:
      break;
  }
}

void GEN_CSV_READER::writeOutputData(TEventID paEO) {
  switch(paEO) {
    case scmEventINITOID: {
      writeData(0, *mDOs[0], mDOConns[0]);
      writeData(1, *mDOs[1], mDOConns[1]);
      break;
    }
    case scmEventCNFID: {
      writeData(0, *mDOs[0], mDOConns[0]);
      writeData(1, *mDOs[1], mDOConns[1]);
      for(TPortId i = 2; i < getFBInterfaceSpec().mNumDOs; i++){
        writeData(i, *mDOs[i], mDOConns[i]);
      }
      break;
    }
    default:
      break;
  }
}

bool GEN_CSV_READER::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  const char *acPos = strrchr(paConfigString, '_');
  if(nullptr != acPos){
    acPos++;
    paInterfaceSpec.mNumDOs = static_cast<TPortId>(forte::core::util::strtoul(acPos, nullptr, 10) + 2); // we have in addition to the SDs a QI and FILE_NAME data inputs

    mDataOutputNames = new CStringDictionary::TStringId[paInterfaceSpec.mNumDOs];
    mDataOutputTypeIds = new CStringDictionary::TStringId[paInterfaceSpec.mNumDOs];

    mDataOutputNames[0] = g_nStringIdQO;
    mDataOutputTypeIds[0] = g_nStringIdBOOL;
    mDataOutputNames[1] = g_nStringIdSTATUS;
    mDataOutputTypeIds[1] = g_nStringIdSTRING;

    generateGenericDataPointArrays("RD_", &(mDataOutputTypeIds[2]), &(mDataOutputNames[2]), paInterfaceSpec.mNumDOs - 2);

    //create the interface Specification
    paInterfaceSpec.mNumEIs = 2;
    paInterfaceSpec.mEINames = scmEventInputNames;
    paInterfaceSpec.mNumEOs = 2;
    paInterfaceSpec.mEONames = scmEventOutputNames;
    paInterfaceSpec.mDINames = scmDataInputNames;
    paInterfaceSpec.mDIDataTypeNames = scmDataInputTypeIds;
    paInterfaceSpec.mNumDOs = 2;
    paInterfaceSpec.mDONames = mDataOutputNames;
    paInterfaceSpec.mDODataTypeNames = mDataOutputTypeIds;
    return true;
  }
  return false;
}

void GEN_CSV_READER::openCSVFile() {
  QO() = false_BOOL;
  if(!mCSVFile.is_open()) {
    mCSVFile.open(FILE_NAME().getStorage().c_str());
    if(mCSVFile.is_open()) {
      QO() = true_BOOL;
      STATUS() = scmOK;
      DEVLOG_INFO("[GEN_CSV_READER]: File %s successfully opened\n", FILE_NAME().getStorage().c_str());
    } else {
      const char* errorCode = strerror(errno); 
      STATUS() = CIEC_STRING(errorCode, strlen(errorCode));
      DEVLOG_ERROR("[GEN_CSV_READER]: Couldn't open file %s. Error: %s\n", FILE_NAME().getStorage().c_str(), STATUS().getStorage().c_str());
    }
  } else {
    STATUS() = scmFileAlreadyOpened;
    DEVLOG_ERROR("[GEN_CSV_READER]: Can't open file %s since it is already opened\n", FILE_NAME().getStorage().c_str());
  }
}

void GEN_CSV_READER::closeCSVFile() {
  QO() = CIEC_BOOL(false);
  mCSVFile.close();
  STATUS() = scmOK;
  DEVLOG_INFO("[GEN_CSV_READER]: File %s successfully closed\n", FILE_NAME().getStorage().c_str());
}

void GEN_CSV_READER::readCSVFileLine() {
  if(mCSVFile.is_open()) {
    std::string line;
    if(getline(mCSVFile, line)){
      std::size_t oldPos = 0;

      for(std::size_t i = 0;  i < getNumRD(); i++) {
        std::size_t newPos = line.find(';', oldPos);
        if (newPos != std::string::npos){
          std::string value = line.substr(oldPos, newPos - oldPos);
          getDI(i+2)->unwrap().fromString(value.c_str());
        } else if (oldPos < line.length()){
          std::string value = line.substr(oldPos);
          getDI(i+2)->unwrap().fromString(value.c_str());
        } else {
          CIEC_ANY *value = CTypeLib::createDataTypeInstance(getDI(i+2)->getTypeNameID(), nullptr);
          if (value) { getDI(i+2)->setValue(*value); }
          delete value;
        }
        oldPos = newPos + 1;
      }
      QO() = true_BOOL;
    } else {
      QO() = false_BOOL;
      STATUS() = "Could not read line!"_STRING;
    }
  } else {
    QO() = false_BOOL;
    STATUS() = scmFileNotOpened;
    DEVLOG_ERROR("[GEN_CSV_READER]: Can't write to file %s since it is not opened\n", FILE_NAME().getStorage().c_str());
  }
}
