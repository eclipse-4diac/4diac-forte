/*******************************************************************************
 * Copyright (c) 2026 Malte Grave
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Malte Grave - initial API and implementation
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

#include <atomic>
#include <string>

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/io/configFB/io_configFB_controller.h"
#include "forte/io/device/io_controller.h"

#include "../fbtests/fbtesterglobalfixture.h"
#include "../../../core/src/typelib_internal.h"

using namespace forte::literals;

/* Tests for the shutdown of IOConfigFBController.
 *
 * Stopping the device controller writes STATUS(), which goes through a pure virtual method getDO().
 * We have a new design requirement that all cleanup happens in deinitialize(), while the object is still complete, and
 * the destructor is defaulted. These tests check that deinitialize() fully stops the controller, so that the following
 * delete has nothing left to do and runs cleanly.
 */
namespace forte::io::test {

  namespace {
    struct SControllerProbe {
        std::atomic<bool> mRunLoopEntered{false};
        std::atomic<int> mDeInitCalls{0};
        std::atomic<int> mDestroyed{0};

        void reset() {
          mRunLoopEntered = false;
          mDeInitCalls = 0;
          mDestroyed = 0;
        }
    };

    SControllerProbe gProbe;

    class CMockDeviceController : public IODeviceController {
      public:
        explicit CMockDeviceController(CDeviceExecution &paDeviceExecution) : IODeviceController(paDeviceExecution) {
        }

        ~CMockDeviceController() override {
          ++gProbe.mDestroyed;
        }

        void setConfig(Config *) override {
        }

      protected:
        const char *init() override {
          return nullptr;
        }

        void runLoop() override {
          gProbe.mRunLoopEntered = true;
          while (isAlive()) {
            sleepThread(1);
          }
        }

        void deInit() override {
          ++gProbe.mDeInitCalls;
        }

        IOHandle *createIOHandle(HandleDescriptor &) override {
          return nullptr;
        }
    };

    const auto cEventInputNames = std::array{"INIT"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "IND"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cDataInputNames = std::array{"QI"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
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

    class CMockIOConfigFB : public IOConfigFBController {
      public:
        explicit CMockIOConfigFB(CFBContainer &paContainer) :
            IOConfigFBController(paContainer, cFBInterfaceSpec, "MockIOConfig"_STRID),
            var_QI(false_BOOL),
            var_QO(false_BOOL),
            var_STATUS(u""_WSTRING),
            conn_INITO(*this, 0),
            conn_IND(*this, 1),
            conn_QI(nullptr),
            conn_QO(*this, 0, var_QO),
            conn_STATUS(*this, 1, var_STATUS) {
        }

        StringId getFBTypeId() const override {
          return "MockIOConfig"_STRID;
        }

        /*! Start the device controller without an event chain executor.
         *
         * The startup notification of the controller is then dropped by the device execution, so the test thread
         * is the only one touching the FB and the test stays deterministic.
         */
        bool startController() {
          return init(nullptr);
        }

        CIEC_BOOL var_QI;
        CIEC_BOOL var_QO;
        CIEC_WSTRING var_STATUS;

        CEventConnection conn_INITO;
        CEventConnection conn_IND;
        CDataConnection *conn_QI;
        COutDataConnection<CIEC_BOOL> conn_QO;
        COutDataConnection<CIEC_WSTRING> conn_STATUS;

        CIEC_ANY *getDI(size_t paIndex) override {
          return (0 == paIndex) ? &var_QI : nullptr;
        }

        CIEC_ANY *getDO(size_t paIndex) override {
          switch (paIndex) {
            case 0: return &var_QO;
            case 1: return &var_STATUS;
          }
          return nullptr;
        }

        CEventConnection *getEOConUnchecked(TPortId paIndex) override {
          switch (paIndex) {
            case 0: return &conn_INITO;
            case 1: return &conn_IND;
          }
          return nullptr;
        }

        CDataConnection **getDIConUnchecked(TPortId paIndex) override {
          return (0 == paIndex) ? &conn_QI : nullptr;
        }

        CDataConnection *getDOConUnchecked(TPortId paIndex) override {
          switch (paIndex) {
            case 0: return &conn_QO;
            case 1: return &conn_STATUS;
          }
          return nullptr;
        }

      protected:
        IODeviceController *createDeviceController(CDeviceExecution &paDeviceExecution) override {
          return new CMockDeviceController(paDeviceExecution);
        }

        void setConfig() override {
        }

      private:
        void readInputData(TEventID) override {
        }

        void writeOutputData(TEventID) override {
        }

        void setInitialValues() override {
        }
    };

    bool waitForRunLoop() {
      for (int i = 0; i < 2000 && !gProbe.mRunLoopEntered; ++i) {
        arch::CThread::sleepThread(1);
      }
      return gProbe.mRunLoopEntered;
    }

    /*! Creates an initialized mock FB with a running device controller */
    CMockIOConfigFB *createRunningFB() {
      auto *fb = new CMockIOConfigFB(forte::test::CFBTestDataGlobalFixture::getResource());
      BOOST_REQUIRE(fb->initialize());
      BOOST_REQUIRE(fb->startController());
      BOOST_REQUIRE(nullptr != fb->getDeviceController());
      BOOST_REQUIRE(waitForRunLoop());
      return fb;
    }

    struct SProbeFixture {
        SProbeFixture() {
          gProbe.reset();
        }
    };
  } // namespace

  BOOST_FIXTURE_TEST_SUITE(IOConfigFBController_Shutdown, SProbeFixture)

  BOOST_AUTO_TEST_CASE(deinitializeStopsRunningController) {
    CMockIOConfigFB *fb = createRunningFB();

    fb->deinitialize();

    BOOST_TEST(fb->getDeviceController() == nullptr);
    BOOST_TEST(gProbe.mDeInitCalls == 1);
    BOOST_TEST(gProbe.mDestroyed == 1);
    BOOST_TEST(std::string(fb->var_STATUS.getValue()) == "Stopped");

    delete fb;

    // the defaulted destructor must not touch the (in our case, the already released) controller
    BOOST_TEST(gProbe.mDeInitCalls == 1);
    BOOST_TEST(gProbe.mDestroyed == 1);
  }

  BOOST_AUTO_TEST_CASE(deleteFBStopsRunningController) {
    CMockIOConfigFB *fb = createRunningFB();

    BOOST_TEST(forte::deleteFB(fb));

    BOOST_TEST(gProbe.mDeInitCalls == 1);
    BOOST_TEST(gProbe.mDestroyed == 1);
  }

  BOOST_AUTO_TEST_CASE(deinitializeWithoutControllerIsNoop) {
    auto *fb = new CMockIOConfigFB(forte::test::CFBTestDataGlobalFixture::getResource());
    BOOST_REQUIRE(fb->initialize());

    fb->deinitialize();
    fb->deinitialize();

    BOOST_TEST(fb->getDeviceController() == nullptr);
    BOOST_TEST(std::string(fb->var_STATUS.getValue()).empty());
    BOOST_TEST(gProbe.mDestroyed == 0);

    delete fb;
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::io::test
