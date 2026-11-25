/*******************************************************************************
 * Copyright (c) 2026 Thomas Oellinger
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Oellinger - initial implementation
 *******************************************************************************/

#include "MCCDaqHatController.h"

#include "forte/util/devlog.h"
#include "boards/MCC128Board.h"
#include "boards/MCC152Board.h"
#include "handles/MCCHandles.h"

#include <utility>

namespace forte::eclipse4diac::io::mccdaqhat {

  MCCDaqHatController::MCCDaqHatController(CDeviceExecution &paDeviceExecution) :
      MCCDaqHatController(paDeviceExecution, createDaqHatsLibApi()) {
  }

  MCCDaqHatController::MCCDaqHatController(CDeviceExecution &paDeviceExecution,
                                           std::unique_ptr<IMCCDaqHatApi> paApi) :
      IODeviceMultiController(paDeviceExecution),
      mApi(std::move(paApi)) {
  }

  void MCCDaqHatController::setConfig(forte::io::IODeviceController::Config *paConfig) {
    mConfig = *static_cast<Config *>(paConfig);
  }

  const char *MCCDaqHatController::init() {
    util::CCriticalRegion criticalRegion(mBoardsMutex);
    mDetectedHats = mApi->listHats();
    if (mDetectedHats.empty()) {
      return "No MCC DAQ HAT found";
    }
    for (const auto &hat : mDetectedHats) {
      DEVLOG_INFO("[MCCDaqHat] Found HAT id 0x%04x at address %u\n", hat.mId, hat.mAddress);
    }
    return nullptr;
  }

  void MCCDaqHatController::runLoop() {
    const unsigned int rate = (mConfig.mUpdateInterval > 0) ? mConfig.mUpdateInterval : 1;
    const TForteUInt64 timeout = 1000000000ULL / rate;
    while (isAlive()) {
      mWakeSemaphore.timedWait(timeout);
      if (!isAlive()) {
        break;
      }
      {
        util::CCriticalRegion criticalRegion(mBoardsMutex);
        for (auto &[index, board] : mBoards) {
          if (!board->poll()) {
            mErrorText = board->getLastError();
            mError = mErrorText.c_str();
          }
        }
      }
      if (hasError()) {
        return;
      }
      checkForInputChanges();
    }
  }

  void MCCDaqHatController::deInit() {
    util::CCriticalRegion criticalRegion(mBoardsMutex);
    for (auto &[index, board] : mBoards) {
      board->close();
    }
    mBoards.clear();
    mDeviceAddresses.clear();
    mDetectedHats.clear();
  }

  bool MCCDaqHatController::registerDeviceAddress(size_t paIndex, uint8_t paAddress) {
    util::CCriticalRegion criticalRegion(mBoardsMutex);
    for (const auto &[index, address] : mDeviceAddresses) {
      if (index != paIndex && address == paAddress) {
        DEVLOG_ERROR("[MCCDaqHat] Address %u of device %u is already used by device %u\n", paAddress,
                     static_cast<unsigned int>(paIndex), static_cast<unsigned int>(index));
        // A stale entry from an earlier INIT of this device must not keep claiming its old address.
        mDeviceAddresses.erase(paIndex);
        return false;
      }
    }
    mDeviceAddresses[paIndex] = paAddress;
    return true;
  }

  uint16_t MCCDaqHatController::detectedIdAt(uint8_t paAddress) const {
    for (const auto &hat : mDetectedHats) {
      if (hat.mAddress == paAddress) {
        return hat.mId;
      }
    }
    return static_cast<uint16_t>(HatType::None);
  }

  bool MCCDaqHatController::isSlaveAvailable(size_t paIndex) {
    util::CCriticalRegion criticalRegion(mBoardsMutex);
    const auto it = mDeviceAddresses.find(paIndex);
    if (it == mDeviceAddresses.end()) {
      return false;
    }
    if (detectedIdAt(it->second) == static_cast<uint16_t>(HatType::None)) {
      // The framework only puts "Module not found" into STATUS; the address is logged here.
      DEVLOG_ERROR("[MCCDaqHat] Address %u: no HAT found\n", it->second);
      return false;
    }
    return true;
  }

  bool MCCDaqHatController::checkSlaveType(size_t paIndex, int paType) {
    util::CCriticalRegion criticalRegion(mBoardsMutex);
    const uint8_t address = mDeviceAddresses.at(paIndex);
    const uint16_t found = detectedIdAt(address);
    if (static_cast<int>(found) != paType) {
      DEVLOG_ERROR("[MCCDaqHat] Address %u: expected HAT type 0x%04x, found 0x%04x\n", address,
                   static_cast<unsigned int>(paType), found);
      return false;
    }
    return true;
  }

  forte::io::IOHandle *MCCDaqHatController::createIOHandle(
      forte::io::IODeviceController::HandleDescriptor &paHandleDescriptor) {
    auto &desc = static_cast<HandleDescriptor &>(paHandleDescriptor);
    util::CCriticalRegion criticalRegion(mBoardsMutex);
    const auto it = mBoards.find(desc.mSlaveIndex);
    if (it == mBoards.end()) {
      return nullptr;
    }
    MCCBoard &board = *it->second;
    const bool isMCC128 = board.getType() == HatType::MCC128;
    switch (desc.mKind) {
      case HandleKind::AnalogIn:
        if (!isMCC128) {
          return nullptr;
        }
        return new MCCAnalogInHandle(this, desc.mSlaveIndex, static_cast<MCC128Board &>(board), desc.mChannel);
      case HandleKind::DigitalIn:
        if (isMCC128) {
          return nullptr;
        }
        return new MCCDigitalInHandle(this, desc.mSlaveIndex, static_cast<MCC152Board &>(board), desc.mChannel);
      case HandleKind::DigitalOut:
        if (isMCC128) {
          return nullptr;
        }
        return new MCCDigitalOutHandle(this, desc.mSlaveIndex, static_cast<MCC152Board &>(board), desc.mChannel);
      case HandleKind::AnalogOut:
        if (isMCC128) {
          return nullptr;
        }
        return new MCCAnalogOutHandle(this, desc.mSlaveIndex, static_cast<MCC152Board &>(board), desc.mChannel);
    }
    return nullptr;
  }

  void MCCDaqHatController::addSlaveHandle(size_t, std::unique_ptr<forte::io::IOHandle> paHandle) {
    // Kept in the controller lists (as revolutionPi does) so that checkForInputChanges() sees the inputs.
    util::CCriticalRegion criticalRegion(mHandleMutex);
    paHandle->isInput() ? mInputHandles.push_back(std::move(paHandle)) : mOutputHandles.push_back(std::move(paHandle));
  }

  void MCCDaqHatController::dropSlaveHandles(size_t paIndex) {
    util::CCriticalRegion criticalRegion(mHandleMutex);
    auto dropFrom = [paIndex](THandleList &paList) {
      std::erase_if(paList, [paIndex](const std::unique_ptr<forte::io::IOHandle> &paHandle) {
        if (static_cast<MCCHandle &>(*paHandle).getDeviceIndex() != paIndex) {
          return false;
        }
        forte::io::IOMapper::getInstance().deregisterHandle(*paHandle);
        return true;
      });
    };
    dropFrom(mInputHandles);
    dropFrom(mOutputHandles);
  }

  bool MCCDaqHatController::isHandleValueEqual(forte::io::IOHandle &paHandle) {
    return !static_cast<MCCHandle &>(paHandle).checkChanged();
  }

} // namespace forte::eclipse4diac::io::mccdaqhat
