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

#include <boost/test/unit_test.hpp>

#include "FakeMCCDaqHatApi.h"
#include "../../core/fbtests/fbtesterglobalfixture.h"
#include "../../../modules/mccdaqhat/MCCDaqHatController.h"
#include "../../../modules/mccdaqhat/boards/MCC128Board.h"
#include "../../../modules/mccdaqhat/boards/MCC152Board.h"
#include "../../../modules/mccdaqhat/handles/MCCHandles.h"

#include <chrono>
#include <memory>
#include <string>
#include <thread>

namespace forte::eclipse4diac::io::mccdaqhat {

  //! Befriended by MCCDaqHatController: reaches the framework hooks that only the IO config FBs call in production.
  struct MCCDaqHatControllerTestAccess {
      static const char *init(MCCDaqHatController &paController) {
        return paController.init();
      }

      static bool isSlaveAvailable(MCCDaqHatController &paController, size_t paIndex) {
        return paController.isSlaveAvailable(paIndex);
      }

      static bool checkSlaveType(MCCDaqHatController &paController, size_t paIndex, HatType paType) {
        return paController.checkSlaveType(paIndex, static_cast<int>(paType));
      }

      static std::unique_ptr<forte::io::IOHandle> createHandle(MCCDaqHatController &paController,
                                                               size_t paDeviceIndex,
                                                               MCCDaqHatController::HandleKind paKind,
                                                               uint8_t paChannel) {
        const std::string id = "test";
        MCCDaqHatController::HandleDescriptor descriptor(id, forte::io::IOMapper::In, paDeviceIndex, paKind, paChannel);
        return std::unique_ptr<forte::io::IOHandle>(paController.createIOHandle(descriptor));
      }

      static size_t handleCount(MCCDaqHatController &paController) {
        return paController.mInputHandles.size() + paController.mOutputHandles.size();
      }

      static bool allHandlesBelongTo(MCCDaqHatController &paController, size_t paDeviceIndex) {
        for (const auto *list : {&paController.mInputHandles, &paController.mOutputHandles}) {
          for (const auto &handle : *list) {
            if (static_cast<MCCHandle &>(*handle).getDeviceIndex() != paDeviceIndex) {
              return false;
            }
          }
        }
        return true;
      }

      //! Runs init() and runLoop() on the controller thread and waits until the loop gave up with an error.
      static std::string runUntilError(MCCDaqHatController &paController) {
        paController.start();
        const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(10);
        while (!paController.hasError() && std::chrono::steady_clock::now() < deadline) {
          std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        paController.end();
        return paController.mErrorText;
      }
  };

  namespace {
    using test::FakeMCCDaqHatApi;
    using HandleKind = MCCDaqHatController::HandleKind;

    constexpr uint16_t scMCC128Id = static_cast<uint16_t>(HatType::MCC128);
    constexpr uint16_t scMCC152Id = static_cast<uint16_t>(HatType::MCC152);

    //! Controller on the shared test device with a fake vendor library; the controller owns the fake.
    struct ControllerFixture {
        FakeMCCDaqHatApi *mApi;
        std::unique_ptr<MCCDaqHatController> mController;

        ControllerFixture() {
          auto api = std::make_unique<FakeMCCDaqHatApi>();
          mApi = api.get();
          mController = std::make_unique<MCCDaqHatController>(
              forte::test::CFBTestDataGlobalFixture::getDevice().getDeviceExecution(), std::move(api));
        }

        MCCDaqHatController &operator*() {
          return *mController;
        }
    };
  } // namespace

  BOOST_AUTO_TEST_SUITE(MCCDaqHatController_Tests)

  BOOST_AUTO_TEST_CASE(initFailsWithoutAnyHat) {
    ControllerFixture fixture;
    BOOST_TEST(std::string(MCCDaqHatControllerTestAccess::init(*fixture)) == "No MCC DAQ HAT found");
  }

  BOOST_AUTO_TEST_CASE(deviceWithMatchingHatTypeAtItsAddressIsAccepted) {
    ControllerFixture fixture;
    fixture.mApi->mHats = {{2, scMCC128Id}, {5, scMCC152Id}};
    BOOST_TEST(MCCDaqHatControllerTestAccess::init(*fixture) == nullptr);
    BOOST_TEST((*fixture).registerDeviceAddress(0, 2));
    BOOST_TEST((*fixture).registerDeviceAddress(1, 5));
    BOOST_TEST(MCCDaqHatControllerTestAccess::isSlaveAvailable(*fixture, 0));
    BOOST_TEST(MCCDaqHatControllerTestAccess::checkSlaveType(*fixture, 0, HatType::MCC128));
    BOOST_TEST(MCCDaqHatControllerTestAccess::isSlaveAvailable(*fixture, 1));
    BOOST_TEST(MCCDaqHatControllerTestAccess::checkSlaveType(*fixture, 1, HatType::MCC152));
  }

  BOOST_AUTO_TEST_CASE(deviceWithWrongHatTypeIsRejected) {
    ControllerFixture fixture;
    fixture.mApi->mHats = {{2, scMCC128Id}};
    BOOST_TEST(MCCDaqHatControllerTestAccess::init(*fixture) == nullptr);
    BOOST_TEST((*fixture).registerDeviceAddress(0, 2));
    BOOST_TEST(MCCDaqHatControllerTestAccess::isSlaveAvailable(*fixture, 0));
    BOOST_TEST(!MCCDaqHatControllerTestAccess::checkSlaveType(*fixture, 0, HatType::MCC152));
  }

  BOOST_AUTO_TEST_CASE(deviceWithoutHatAtItsAddressIsNotAvailable) {
    ControllerFixture fixture;
    fixture.mApi->mHats = {{2, scMCC128Id}};
    BOOST_TEST(MCCDaqHatControllerTestAccess::init(*fixture) == nullptr);
    BOOST_TEST((*fixture).registerDeviceAddress(0, 3));
    BOOST_TEST(!MCCDaqHatControllerTestAccess::isSlaveAvailable(*fixture, 0));
    // A device that never registered an address (e.g. its address check failed) is not available either.
    BOOST_TEST(!MCCDaqHatControllerTestAccess::isSlaveAvailable(*fixture, 7));
  }

  BOOST_AUTO_TEST_CASE(deviceAddressBeyondJumperRangeFindsNoHat) {
    ControllerFixture fixture;
    fixture.mApi->mHats = {{7, scMCC152Id}};
    BOOST_TEST(MCCDaqHatControllerTestAccess::init(*fixture) == nullptr);
    BOOST_TEST((*fixture).registerDeviceAddress(0, 8));
    BOOST_TEST(!MCCDaqHatControllerTestAccess::isSlaveAvailable(*fixture, 0));
    BOOST_TEST(validateAddress(8) == "Address 8 is out of range 0..7");
  }

  BOOST_AUTO_TEST_CASE(duplicateDeviceAddressIsRejected) {
    ControllerFixture fixture;
    fixture.mApi->mHats = {{4, scMCC128Id}};
    BOOST_TEST(MCCDaqHatControllerTestAccess::init(*fixture) == nullptr);
    BOOST_TEST((*fixture).registerDeviceAddress(1, 4));
    BOOST_TEST(!(*fixture).registerDeviceAddress(2, 4));
    BOOST_TEST(MCCDaqHatControllerTestAccess::isSlaveAvailable(*fixture, 1));
    BOOST_TEST(!MCCDaqHatControllerTestAccess::isSlaveAvailable(*fixture, 2));
    // Registering the same device again (re-INIT) is not a conflict, and a freed address can be taken over.
    BOOST_TEST((*fixture).registerDeviceAddress(1, 4));
    BOOST_TEST((*fixture).registerDeviceAddress(1, 6));
    BOOST_TEST((*fixture).registerDeviceAddress(2, 4));
  }

  BOOST_AUTO_TEST_CASE(createIOHandleMatchesHandleKindToBoardType) {
    ControllerFixture fixture;
    BOOST_TEST((*fixture).registerDeviceAddress(0, 1));
    BOOST_TEST((*fixture).registerDeviceAddress(1, 2));
    (*fixture).createBoard<MCC128Board>(0);
    (*fixture).createBoard<MCC152Board>(1);

    BOOST_TEST(MCCDaqHatControllerTestAccess::createHandle(*fixture, 0, HandleKind::AnalogIn, 3) != nullptr);
    BOOST_TEST(MCCDaqHatControllerTestAccess::createHandle(*fixture, 0, HandleKind::DigitalIn, 3) == nullptr);
    BOOST_TEST(MCCDaqHatControllerTestAccess::createHandle(*fixture, 0, HandleKind::DigitalOut, 3) == nullptr);
    BOOST_TEST(MCCDaqHatControllerTestAccess::createHandle(*fixture, 0, HandleKind::AnalogOut, 0) == nullptr);

    BOOST_TEST(MCCDaqHatControllerTestAccess::createHandle(*fixture, 1, HandleKind::AnalogIn, 3) == nullptr);
    BOOST_TEST(MCCDaqHatControllerTestAccess::createHandle(*fixture, 1, HandleKind::DigitalIn, 3) != nullptr);
    BOOST_TEST(MCCDaqHatControllerTestAccess::createHandle(*fixture, 1, HandleKind::DigitalOut, 3) != nullptr);
    BOOST_TEST(MCCDaqHatControllerTestAccess::createHandle(*fixture, 1, HandleKind::AnalogOut, 1) != nullptr);

    // No board was created for device 2, so no handle can be created either.
    BOOST_TEST(MCCDaqHatControllerTestAccess::createHandle(*fixture, 2, HandleKind::AnalogIn, 0) == nullptr);
  }

  BOOST_AUTO_TEST_CASE(dropSlaveHandlesRemovesOnlyThatDevicesHandles) {
    ControllerFixture fixture;
    BOOST_TEST((*fixture).registerDeviceAddress(0, 1));
    BOOST_TEST((*fixture).registerDeviceAddress(1, 2));
    (*fixture).createBoard<MCC128Board>(0);
    (*fixture).createBoard<MCC152Board>(1);
    (*fixture).addSlaveHandle(0, MCCDaqHatControllerTestAccess::createHandle(*fixture, 0, HandleKind::AnalogIn, 0));
    (*fixture).addSlaveHandle(0, MCCDaqHatControllerTestAccess::createHandle(*fixture, 0, HandleKind::AnalogIn, 1));
    (*fixture).addSlaveHandle(1, MCCDaqHatControllerTestAccess::createHandle(*fixture, 1, HandleKind::DigitalIn, 0));
    (*fixture).addSlaveHandle(1, MCCDaqHatControllerTestAccess::createHandle(*fixture, 1, HandleKind::DigitalOut, 1));
    BOOST_TEST(MCCDaqHatControllerTestAccess::handleCount(*fixture) == 4U);

    (*fixture).dropSlaveHandles(0);
    BOOST_TEST(MCCDaqHatControllerTestAccess::handleCount(*fixture) == 2U);
    BOOST_TEST(MCCDaqHatControllerTestAccess::allHandlesBelongTo(*fixture, 1));

    (*fixture).dropSlaveHandles(1);
    BOOST_TEST(MCCDaqHatControllerTestAccess::handleCount(*fixture) == 0U);
  }

  BOOST_AUTO_TEST_CASE(persistentBoardErrorsStopTheController) {
    ControllerFixture fixture;
    fixture.mApi->mHats = {{1, scMCC152Id}};
    MCCDaqHatController::Config config;
    config.mUpdateInterval = 1000;
    (*fixture).setConfig(&config);
    BOOST_TEST((*fixture).registerDeviceAddress(0, 1));
    BOOST_TEST((*fixture).createBoard<MCC152Board>(0).open(0xFF, 0xFF, 0xFF).empty());
    fixture.mApi->mNextResult = -7; // every following poll fails

    BOOST_TEST(MCCDaqHatControllerTestAccess::runUntilError(*fixture) == "Address 1: fake error");
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::eclipse4diac::io::mccdaqhat
