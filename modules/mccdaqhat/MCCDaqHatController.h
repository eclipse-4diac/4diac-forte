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

#pragma once

#include "forte/io/device/io_controller_multi.h"
#include "forte/arch/forte_sem.h"
#include "forte/arch/forte_sync.h"
#include "forte/util/criticalregion.h"
#include "mccdaqhat_api.h"
#include "boards/MCCBoard.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace forte::eclipse4diac::io::mccdaqhat {

  struct MCCDaqHatControllerTestAccess;

  class MCCDaqHatController final : public forte::io::IODeviceMultiController {
      friend struct MCCDaqHatControllerTestAccess; //!< Lets the unit tests reach the framework-internal hooks

    public:
      struct Config : forte::io::IODeviceController::Config {
          unsigned int mUpdateInterval = 25; //!< Poll rate in Hz
      };

      enum class HandleKind { AnalogIn, DigitalIn, DigitalOut, AnalogOut };

      class HandleDescriptor : public forte::io::IODeviceMultiController::HandleDescriptor {
        public:
          HandleKind mKind;
          uint8_t mChannel;

          HandleDescriptor(std::string const &paId,
                           forte::io::IOMapper::Direction paDirection,
                           size_t paDeviceIndex,
                           HandleKind paKind,
                           uint8_t paChannel) :
              IODeviceMultiController::HandleDescriptor(paId, paDirection, paDeviceIndex),
              mKind(paKind),
              mChannel(paChannel) {
          }
      };

      explicit MCCDaqHatController(CDeviceExecution &paDeviceExecution);

      //! Injection point for the vendor library seam; unit tests pass a fake here
      MCCDaqHatController(CDeviceExecution &paDeviceExecution, std::unique_ptr<IMCCDaqHatApi> paApi);

      void setConfig(forte::io::IODeviceController::Config *paConfig) override;
      void addSlaveHandle(size_t paIndex, std::unique_ptr<forte::io::IOHandle> paHandle) override;
      void dropSlaveHandles(size_t paIndex) override;

      /*! @brief Remembers the jumper address of a device; called by the device FB before the framework type check
       *
       * @return false (after logging the other device's index) if another device already uses this address; two
       *         devices on one HAT would open and reconfigure the same board twice.
       */
      bool registerDeviceAddress(size_t paIndex, uint8_t paAddress);

      //! Creates the board object of a device, replacing a previous one. The device must drop its handles first.
      template<typename TBoard>
      TBoard &createBoard(size_t paIndex) {
        util::CCriticalRegion criticalRegion(mBoardsMutex);
        auto board = std::make_unique<TBoard>(*mApi, mDeviceAddresses.at(paIndex));
        TBoard &result = *board;
        mBoards[paIndex] = std::move(board);
        return result;
      }

    protected:
      const char *init() override;
      void runLoop() override;
      void deInit() override;
      forte::io::IOHandle *createIOHandle(forte::io::IODeviceController::HandleDescriptor &paHandleDescriptor) override;
      bool isHandleValueEqual(forte::io::IOHandle &paHandle) override;

    private:
      bool isSlaveAvailable(size_t paIndex) override;
      bool checkSlaveType(size_t paIndex, int paType) override;

      //! HAT id at a jumper address, HatType::None if no HAT answers there (mBoardsMutex held)
      uint16_t detectedIdAt(uint8_t paAddress) const;

      std::unique_ptr<IMCCDaqHatApi> mApi;
      Config mConfig;
      std::vector<HatEntry> mDetectedHats;
      std::map<size_t, uint8_t> mDeviceAddresses;
      std::map<size_t, std::unique_ptr<MCCBoard>> mBoards;
      arch::CSyncObject mBoardsMutex;
      arch::CSemaphore mWakeSemaphore; //!< Timed wait between polls; inc() is the hook for a future interrupt path
      std::string mErrorText;          //!< Storage for mError, which is a plain const char *
  };

} // namespace forte::eclipse4diac::io::mccdaqhat
