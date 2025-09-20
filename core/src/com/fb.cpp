/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#include "forte/com/fb.h"

#include "forte/device.h"
#include "forte/util/string_utils.h"

#include <charconv>

using namespace forte::literals;

namespace forte::com {
  namespace {
    TPortId parseConfigStringNum(const std::string_view paConfigString) {
      TPortId result{};
      if (auto [ptr, ec] =
              std::from_chars(paConfigString.data(), paConfigString.data() + paConfigString.size(), result);
          ptr != paConfigString.data() + paConfigString.size() || ec != std::errc()) {
        return cgInvalidPortId;
      }
      return result;
    }
  } // namespace

  CommunicationFB::CommunicationFB(CFBContainer &paContainer,
                                   const SFBInterfaceSpec &paInterfaceSpec,
                                   StringId paInstanceNameId) :
      CGenFunctionBlock(paContainer, paInterfaceSpec, paInstanceNameId),
      conn_INITO(*this, 0),
      conn_QI(nullptr),
      conn_ID(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS) {
  }

  void CommunicationFB::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (paEIID == scmEventINITID) {
      setEventChainExecutor(paECET);
      if (var_QI) {
        open(var_ID.getValue());
      } else {
        close();
      }
      sendOutputEvent(scmEventINITOID, paECET);
    }
  }

  ComResult CommunicationFB::open(const std::string_view paID) {
    if (const ComResult result = BaseCommunicationFB::open(paID); result != ComResult::Ok) {
      setStatus(result);
      return result;
    }

    if (const ComResult result = getChannel()->setMaxReceiveSize(getGenDONums()); result != ComResult::Ok) {
      close();
      setStatus(result);
      return result;
    }

    if (const ComResult result = getChannel()->setMinReceiveSize(getGenDONums()); result != ComResult::Ok) {
      close();
      setStatus(result);
      return result;
    }

    setStatus(ComResult::Ok);
    return ComResult::Ok;
  }

  ComResult CommunicationFB::send(std::span<CIEC_ANY_VARIANT> paData) {
    const ComResult result = var_QI ? CGenFunctionBlock::send(paData) : ComResult::Inhibited;
    if (result != ComResult::Async) {
      setStatus(result);
    }
    return result;
  }

  ComResult CommunicationFB::poll() {
    const ComResult result = var_QI ? CGenFunctionBlock::poll() : ComResult::Inhibited;
    if (result != ComResult::Async) {
      setStatus(result);
    }
    return result;
  }

  void CommunicationFB::close() {
    CGenFunctionBlock::close();
    var_QO = false_BOOL;
    var_STATUS = CIEC_WSTRING(getComResultString(ComResult::Terminated).data());
  }

  ComResult CommunicationFB::receive(const std::span<CIEC_ANY_VARIANT> paData,
                                     ComChannel<std::span<CIEC_ANY_VARIANT>> &) {
    auto result = ComResult::Ok;
    if (paData.size() == getGenDONums()) {
      // lock variables to prevent race conditions between receiving and writing outputs
      util::CCriticalRegion lock(mFBLock);
      for (TPortId i = 0; i < getGenDONums(); ++i) {
        mGenDOs[i].setValue(paData[i].unwrap());
      }
    } else {
      result = ComResult::DataTypeError;
    }
    setStatus(ComResult::Ok);
    getEventChainExecutor()->startEventChain({*this, scmEventReceiveID});
    return result;
  }

  void CommunicationFB::sent(const ComResult paResult, ComChannel<std::span<CIEC_ANY_VARIANT>> &) {
    setStatus(paResult);
    getEventChainExecutor()->startEventChain({*this, scmEventSentID});
  }

  std::optional<std::span<CIEC_ANY_VARIANT>> CommunicationFB::getReceiveBuffer() {
    return mGenDOs;
  }

  std::optional<std::span<CIEC_ANY_VARIANT>> CommunicationFB::getSendBuffer() {
    return mGenDIs;
  }

  void CommunicationFB::setStatus(const ComResult result) {
    var_QO = CIEC_BOOL(result == ComResult::Ok);
    var_STATUS = CIEC_WSTRING(getComResultString(result).data());
  }

  std::array<TPortId, 2> CommunicationFB::parseConfigString(const std::string_view paConfigString) {
    const std::string_view configString(paConfigString);
    const std::size_t lastUnderscore = configString.rfind('_');
    if (lastUnderscore == std::string_view::npos) {
      return {cgInvalidPortId, cgInvalidPortId}; // no '_' found at all
    }
    const TPortId second = parseConfigStringNum(configString.substr(lastUnderscore + 1));

    const std::size_t firstUnderscore = configString.rfind('_', lastUnderscore - 1);
    if (firstUnderscore == std::string_view::npos) {
      return {second, cgInvalidPortId};
    }
    const TPortId first =
        parseConfigStringNum(configString.substr(firstUnderscore + 1, lastUnderscore - firstUnderscore - 1));
    return {first, second};
  }

  void CommunicationFB::configureDIs(const TPortId paGenDINum, SFBInterfaceSpec &paInterfaceSpec) {
    mDINames.emplace_back("QI"_STRID);
    mDINames.emplace_back("ID"_STRID);
    generateGenericInterfacePointNameArray("SD_", mDINames, paGenDINum);
    paInterfaceSpec.mDINames = mDINames;
  }

  void CommunicationFB::configureDOs(const TPortId paGenDONum, SFBInterfaceSpec &paInterfaceSpec) {
    mDONames.emplace_back("QO"_STRID);
    mDONames.emplace_back("STATUS"_STRID);
    generateGenericInterfacePointNameArray("RD_", mDONames, paGenDONum);
    paInterfaceSpec.mDONames = mDONames;
  }

  void CommunicationFB::createGenInputData() {
    mGenDIs.resize(getGenDINums());
  }

  void CommunicationFB::createGenOutputData() {
    mGenDOs.resize(getGenDONums());
  }

  void CommunicationFB::readInputData(const TEventID paEI) {
    if (paEI == scmEventINITID) {
      readData(0, var_QI, conn_QI);
      readData(1, var_ID, conn_ID);
    } else {
      readData(0, var_QI, conn_QI);
      readData(1, var_ID, conn_ID);
      for (size_t i = 0; i < getGenDINums(); ++i) {
        readData(i + 2, mGenDIs[i], mGenDIConns[i]);
      }
    }
  }

  void CommunicationFB::writeOutputData(const TEventID paEO) {
    const size_t numDIs = getFBInterfaceSpec().getNumDIs();
    if (paEO == scmEventINITOID) {
      writeData(numDIs + 0, var_QO, conn_QO);
      writeData(numDIs + 1, var_STATUS, conn_STATUS);
    } else {
      // lock variables to prevent race conditions between receiving and writing outputs
      util::CCriticalRegion lock(mFBLock);
      writeData(numDIs + 0, var_QO, conn_QO);
      writeData(numDIs + 1, var_STATUS, conn_STATUS);
      for (size_t i = 0; i < getGenDONums(); ++i) {
        writeData(numDIs + 2 + i, mGenDOs[i], mGenDOConns[i]);
      }
    }
  }

  CIEC_ANY *CommunicationFB::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_ID;
      default: return &mGenDIs[paIndex - getGenDIOffset()];
    }
  }

  CIEC_ANY *CommunicationFB::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
      default: return &mGenDOs[paIndex - getGenDOOffset()];
    }
  }

  CEventConnection *CommunicationFB::getEOConUnchecked(const TPortId paIndex) {
    if (paIndex == scmEventINITOID) {
      return &conn_INITO;
    }
    return CGenFunctionBlock::getEOConUnchecked(paIndex);
  }

  CDataConnection **CommunicationFB::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_ID;
      default: return CGenFunctionBlock::getDIConUnchecked(paIndex);
    }
  }

  CDataConnection *CommunicationFB::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
      default: return CGenFunctionBlock::getDOConUnchecked(paIndex);
    }
  }
} // namespace forte::com
