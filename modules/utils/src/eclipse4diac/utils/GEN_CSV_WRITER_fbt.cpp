/*******************************************************************************
 * Copyright (c) 2012, 2025 ACIN, fortiss GmbH, Johannes Kepler University,
 *                          Martin Erich Jobst, Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *   Alois Zoitl  - introduced new CGenFB class for better handling generic FBs
 *   Martin Jobst - refactor for ANY variant
 *                - add generic readInputData and writeOutputData
 *   Michael Hansal - add support for array parameters
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include "forte/eclipse4diac/utils/GEN_CSV_WRITER_fbt.h"
#include <memory>
#include "forte/util/string_utils.h"

using namespace forte::literals;

#include <errno.h>
#include "forte/util/devlog.h"

namespace forte::eclipse4diac::utils {
  namespace {
    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
    const auto cEventInputNames = std::array{"INIT"_STRID, "REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
  } // namespace

  DEFINE_GENERIC_FIRMWARE_FB(GEN_CSV_WRITER, "eclipse4diac::utils::GEN_CSV_WRITER"_STRID)

  const CIEC_STRING GEN_CSV_WRITER::scmOK = "OK"_STRING;
  const CIEC_STRING GEN_CSV_WRITER::scmFileAlreadyOpened = "File already opened"_STRING;
  const CIEC_STRING GEN_CSV_WRITER::scmFileNotOpened = "File not opened"_STRING;

  void GEN_CSV_WRITER::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (scmEventINITID == paEIID) {
      if (var_QI) {
        openCSVFile();
      } else {
        closeCSVFile();
      }
      sendOutputEvent(scmEventINITOID, paECET);
    } else if (scmEventREQID == paEIID) {
      var_QO = var_QI;
      if (var_QI) {
        writeCSVFileLine();
      }
      sendOutputEvent(scmEventCNFID, paECET);
    }
  }

  GEN_CSV_WRITER::GEN_CSV_WRITER(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      conn_INITO(*this, 0),
      conn_CNF(*this, 1),
      conn_QI(nullptr),
      conn_FILE_NAME(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS),
      mCSVFile(nullptr) {
  }

  GEN_CSV_WRITER::~GEN_CSV_WRITER() {
    closeCSVFile();
  }

  void GEN_CSV_WRITER::readInputData(TEventID paEI) {
    switch (paEI) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(1, var_FILE_NAME, conn_FILE_NAME);
        break;
      }
      case scmEventREQID: {
        readData(0, var_QI, conn_QI);
        for (TPortId i = 0; i < getGenDINums(); i++) {
          readData(i, mGenDIs[i], mGenDIConns[i]);
        }
        break;
      }
      default: break;
    }
  }

  void GEN_CSV_WRITER::writeOutputData(TEventID) {
    size_t numDIs = getFBInterfaceSpec().getNumDIs();
    writeData(numDIs + 0, var_QO, conn_QO);
    writeData(numDIs + 1, var_STATUS, conn_STATUS);
  }

  bool GEN_CSV_WRITER::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    const char *acPos = strrchr(paConfigString, '_');
    if (nullptr != acPos) {
      acPos++;
      size_t numGenDIs = static_cast<TPortId>(util::strtoul(acPos, nullptr, 10));
      mDataInputNames.reserve(numGenDIs + 2);
      mDataInputNames.emplace_back("QI"_STRID);
      mDataInputNames.emplace_back("FILE_NAME"_STRID);
      generateGenericInterfacePointNameArray("SD_", mDataInputNames, numGenDIs);

      // create the interface Specification
      paInterfaceSpec.mEINames = cEventInputNames;
      paInterfaceSpec.mEONames = cEventOutputNames;
      paInterfaceSpec.mDINames = mDataInputNames;
      paInterfaceSpec.mDONames = cDataOutputNames;
      return true;
    }
    return false;
  }

  void GEN_CSV_WRITER::openCSVFile() {
    var_QO = false_BOOL;
    if (nullptr == mCSVFile) {
      mCSVFile = forte_fopen(var_FILE_NAME.getStorage().c_str(), "w+");
      if (nullptr != mCSVFile) {
        var_QO = true_BOOL;
        var_STATUS = scmOK;
        DEVLOG_INFO("[GEN_CSV_WRITER]: File %s successfully opened\n", var_FILE_NAME.getStorage().c_str());
      } else {
        const char *errorCode = strerror(errno);
        var_STATUS = CIEC_STRING(errorCode, strlen(errorCode));
        DEVLOG_ERROR("[GEN_CSV_WRITER]: Couldn't open file %s. Error: %s\n", var_FILE_NAME.getStorage().c_str(),
                     var_STATUS.getStorage().c_str());
      }
    } else {
      var_STATUS = scmFileAlreadyOpened;
      DEVLOG_ERROR("[GEN_CSV_WRITER]: Can't open file %s since it is already opened\n",
                   var_FILE_NAME.getStorage().c_str());
    }
  }

  void GEN_CSV_WRITER::closeCSVFile() {
    var_QO = false_BOOL;
    if (nullptr != mCSVFile) {
      if (0 == forte_fclose(mCSVFile)) {
        var_STATUS = scmOK;
        DEVLOG_INFO("[GEN_CSV_WRITER]: File %s successfully closed\n", var_FILE_NAME.getStorage().c_str());
      } else {
        const char *errorCode = strerror(errno);
        var_STATUS = CIEC_STRING(errorCode, strlen(errorCode));
        DEVLOG_ERROR("[GEN_CSV_WRITER]: Couldn't close file %s. Error: %s\n", var_FILE_NAME.getStorage().c_str(),
                     var_STATUS.getStorage().c_str());
      }
      mCSVFile = nullptr;
    }
  }

  bool GEN_CSV_WRITER::areDIsSameArrayLength(size_t &commonArraySize) {
    constexpr TPortId firstDataDI = 2;
    if (getFBInterfaceSpec().getNumDIs() <= firstDataDI) {
      return false;
    }

    for (TPortId i = firstDataDI; i < getFBInterfaceSpec().getNumDIs(); i++) {
      auto &value = getDI(i)->unwrap();
      if (value.getDataTypeID() != CIEC_ANY::e_ARRAY) {
        return false;
      }

      const size_t size = static_cast<const CIEC_ARRAY &>(value).size();
      if (i == firstDataDI) {
        commonArraySize = size;
      } else if (size != commonArraySize) {
        return false;
      }
    }

    return true;
  }

  void GEN_CSV_WRITER::writeCSVFileLine() {
    if (nullptr != mCSVFile) {
      size_t numLines;
      bool multiline = areDIsSameArrayLength(numLines);
      if (!multiline) {
        numLines = 1;
      }

      for (size_t line = 0; line < numLines; line++) {
        for (TPortId i = 2; i < getFBInterfaceSpec().getNumDIs(); i++) {
          auto &value = getDI(i)->unwrap();
          if (multiline) {
            auto &array = static_cast<const CIEC_ARRAY &>(value);
            array.at(line).unwrap().toString(mDataOutPutBuffer);
          } else {
            value.toString(mDataOutPutBuffer);
          }
          forte_fwrite(mDataOutPutBuffer.c_str(), 1, mDataOutPutBuffer.size(), mCSVFile);
          forte_fwrite("; ", 1, 2, mCSVFile);
          mDataOutPutBuffer.clear();
        }
        forte_fwrite("\n", 1, 1, mCSVFile);
      }
    } else {
      var_QO = false_BOOL;
      var_STATUS = scmFileNotOpened;
      DEVLOG_ERROR("[GEN_CSV_WRITER]: Can't write to file %s since it is not opened\n",
                   var_FILE_NAME.getStorage().c_str());
    }
  }

  void GEN_CSV_WRITER::createGenInputData() {
    mGenDIs = std::make_unique<CIEC_ANY_VARIANT[]>(getGenDINums());
  }

  CIEC_ANY *GEN_CSV_WRITER::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_FILE_NAME;
      default: return &mGenDIs[paIndex - 2];
    }
    return nullptr;

    return nullptr;
  }

  CIEC_ANY *GEN_CSV_WRITER::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *GEN_CSV_WRITER::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **GEN_CSV_WRITER::getDIConUnchecked(TPortId paDINum) {
    switch (paDINum) {
      case 0: return &conn_QI;
      case 1: return &conn_FILE_NAME;
      default: return CGenFunctionBlock<CFunctionBlock>::getDIConUnchecked(paDINum);
    }
    return nullptr;
  }

  CDataConnection *GEN_CSV_WRITER::getDOConUnchecked(TPortId paDONum) {
    switch (paDONum) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }
} // namespace forte::eclipse4diac::utils
