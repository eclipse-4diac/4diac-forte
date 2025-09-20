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

#include "fileReader_fbt.h"

using namespace forte::literals;

#include <sstream>
#include <ostream>
#include <iostream>

DEFINE_FIRMWARE_FB(fileReader, "fileReader"_STRID)

namespace {
  const auto cDataInputNames = std::array{"QI"_STRID, "FILE_NAME"_STRID};

  const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID, "S1"_STRID};

  const auto cEventInputNames = std::array{"INIT"_STRID, "REQ"_STRID};
  const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};

  const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID};
  const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };
} // namespace

const char *const fileReader::scmOK = "OK";
const char *const fileReader::scmNotInitialised = "Not initialized";
const char *const fileReader::scmCouldNotRead = "Could not read";

void fileReader::executeEvent(TEventID paEIID,
                              CEventChainExecutionThread *const paECET) { // TODO: manage output and status
  switch (paEIID) {
    case scmEventINITID:
      if (true == QI()) {
        QO() = openFile();
      } else {
        closeFile();
      }
      sendOutputEvent(scmEventINITOID, paECET);
      break;
    case scmEventREQID:
      QO() = QI();
      if (true == QI()) {
        QO() = readFile();
      }
      sendOutputEvent(scmEventCNFID, paECET);
      if (false == QO()) {
        std::cout << "----------ERROR\n";
      }
      break;
  }
}

fileReader::fileReader(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId) {
  mFile.rdbuf()->pubsetbuf(nullptr, 0); // disable buffer to avoid latency
}

bool fileReader::openFile() {
  int retVal = false;
  std::string sysFileName(FILE_NAME().getValue());
  if (sysFileName.size()) {
    mFile.open(sysFileName.c_str(), std::fstream::in); // TODO change this when fully switching to C++11 for LMS EV3
    if (mFile.is_open()) {
      retVal = true;
    } else {
      STATUS() = scmNotInitialised;
    }
  } else {
    STATUS() = scmNotInitialised;
  }
  return retVal;
}

void fileReader::closeFile() {
  if (mFile.is_open()) {
    mFile.close();
  }
}

bool fileReader::readFile() {
  bool retVal = false;
  if (mFile.is_open()) {
    std::string read = "";
    mFile.clear();
    mFile.seekg(0, std::ios::beg);
    mFile >> read;
    if (mFile.fail()) {
      STATUS() = scmCouldNotRead;
    } else {
      S1() = read.c_str();
      STATUS() = scmOK;
      retVal = true;
    }
  } else {
    STATUS() = scmNotInitialised;
  }
  return retVal;
}
