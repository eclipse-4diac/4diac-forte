/*******************************************************************************
 * Copyright (c) 2026 HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Franz Höpfinger - initial tests
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

#include "forte/mgmcmdstruct.h"
#include "forte/eventconn.h"
#include "forte/funcbloc.h"
#include "forte/datatypes/forte_dint.h"

#include "forte_boost_output_support.h"
#include "fbtests/fbtesterglobalfixture.h"
#include "../../stdfblib/system/src/DEV_MGR.h"

using namespace std::string_literals;
using namespace forte::literals;

namespace forte::test {
  namespace {
    EMGMResponse executeMGMCommand(CResource &paResource, const SManagementCMD &paCommand) {
      SManagementCMD command = paCommand;
      return paResource.executeMGMCommand(command);
    }

    const auto cDataInputNames = std::array{"IN"_STRID};

    const SFBInterfaceSpec cOneDIInterfaceSpec = {
        .mEINames = {},
        .mEITypeNames = {},
        .mEONames = {},
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };

    //! FB with one DINT input; the test variants below change how initialize() and configureGenericDI() behave
    class COneDITestFB : public CFunctionBlock {
      public:
        COneDITestFB(const StringId paInstanceNameId, CFBContainer &paContainer) :
            CFunctionBlock(paContainer, cOneDIInterfaceSpec, paInstanceNameId),
            mInConnection(nullptr) {
        }

        CIEC_ANY *getDI(size_t paIndex) override {
          return paIndex == 0 ? &mIn : nullptr;
        }

        CIEC_ANY *getDO(size_t) override {
          return nullptr;
        }

      protected:
        CEventConnection *getEOConUnchecked(TPortId) override {
          return nullptr;
        }

        CDataConnection **getDIConUnchecked(TPortId paIndex) override {
          return paIndex == 0 ? &mInConnection : nullptr;
        }

        CDataConnection *getDOConUnchecked(TPortId) override {
          return nullptr;
        }

        void setInitialValues() override {
          mIn = CIEC_DINT(0);
        }

      private:
        void executeEvent(TEventID, CEventChainExecutionThread *const) override {
        }

        void readInputData(TEventID) override {
        }

        void writeOutputData(TEventID) override {
        }

        CIEC_DINT mIn;
        CDataConnection *mInConnection;
    };

    //! rejects every connection to its input in configureGenericDI()
    class CRejectingDITestFB final : public COneDITestFB {
        DECLARE_FIRMWARE_FB(CRejectingDITestFB)

      public:
        using COneDITestFB::COneDITestFB;

      protected:
        bool configureGenericDI(TPortId, const CIEC_ANY &) override {
          return false;
        }
    };

    //! initialize() fails; counts the calls of deinitialize()
    class CFailingInitTestFB final : public COneDITestFB {
        DECLARE_FIRMWARE_FB(CFailingInitTestFB)

      public:
        using COneDITestFB::COneDITestFB;

        bool initialize() override {
          return false;
        }

        void deinitialize() override {
          ++smDeinitializeCount;
          COneDITestFB::deinitialize();
        }

        static inline int smDeinitializeCount = 0;
    };

    DEFINE_FIRMWARE_FB(CRejectingDITestFB, "test::DeinitializeTest_RejectingDI"_STRID)
    DEFINE_FIRMWARE_FB(CFailingInitTestFB, "test::DeinitializeTest_FailingInit"_STRID)

    CFunctionBlock *findFB(CResource &paResource, StringId paName) {
      for (CFBContainer *child : paResource.getChildren()) {
        if (child->isFB() && child->getInstanceNameId() == paName) {
          return static_cast<CFunctionBlock *>(child);
        }
      }
      return nullptr;
    }
  } // namespace

  BOOST_AUTO_TEST_SUITE(DeinitializeTest)

  // An FB whose initialize() never ran (e.g. a later child of a container whose initialization failed) must survive
  // deinitialize(). DEV_MGR creates its generic connections only in initialize().
  BOOST_AUTO_TEST_CASE(deinitializeWithoutInitialize) {
    CInternalFB<iec61499::system::DEV_MGR> mgr("DeinitializeTest_DEV_MGR"_STRID, CFBTestDataGlobalFixture::getDevice());
    mgr->deinitialize();
  }

  // deinitialize() removes every destination of an event output, also when there is more than one
  BOOST_AUTO_TEST_CASE(deinitializeDisconnectsAllEventDestinations) {
    CInternalFB<iec61499::system::EMB_RES> resource("DeinitializeTest_Destinations"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    for (const auto name : {"SRC"_STRID, "DST1"_STRID, "DST2"_STRID, "DST3"_STRID}) {
      BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                     .mCMD = EMGMCommandType::CreateFBInstance,
                                                     .mFirstParam = {name},
                                                     .mSecondParam = {"iec61131::selection::F_MOVE_1STRING"_STRID},
                                                 }) == EMGMResponse::Ready);
    }
    for (const auto name : {"DST1"_STRID, "DST2"_STRID, "DST3"_STRID}) {
      BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                     .mCMD = EMGMCommandType::CreateConnection,
                                                     .mFirstParam = {"SRC"_STRID, "CNF"_STRID},
                                                     .mSecondParam = {name, "REQ"_STRID},
                                                 }) == EMGMResponse::Ready);
    }

    CFunctionBlock *src = nullptr;
    for (CFBContainer *child : resource->getChildren()) {
      if (child->isFB() && child->getInstanceNameId() == "SRC"_STRID) {
        src = static_cast<CFunctionBlock *>(child);
      }
    }
    BOOST_REQUIRE(src != nullptr);
    CEventConnection *cnf = src->getEOConnection("CNF"_STRID);
    BOOST_REQUIRE(cnf != nullptr);
    BOOST_REQUIRE_EQUAL(cnf->getDestinationList().size(), 3);

    src->deinitialize();
    BOOST_TEST(cnf->getDestinationList().empty());

    resource->deinitialize();
  }

  // createFB() cleans up an FB whose initialize() failed with deinitialize() before deleting it
  BOOST_AUTO_TEST_CASE(createFBDeinitializesWhenInitializeFails) {
    CInternalFB<iec61499::system::EMB_RES> resource("DeinitializeTest_FailingInit"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    CFailingInitTestFB::smDeinitializeCount = 0;
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateFBInstance,
                                                .mFirstParam = {"FAILING"_STRID},
                                                .mSecondParam = {"test::DeinitializeTest_FailingInit"_STRID},
                                            }) != EMGMResponse::Ready);
    BOOST_TEST(CFailingInitTestFB::smDeinitializeCount == 1);
    BOOST_TEST(findFB(*resource, "FAILING"_STRID) == nullptr);

    resource->deinitialize();
  }

  // When configureGenericDI() rejects a connection, connectDI() must not keep it: the caller deletes the connection on
  // failure, and deinitialize() would use the dangling pointer
  BOOST_AUTO_TEST_CASE(connectDIRestoresConnectionWhenConfigureFails) {
    CInternalFB<iec61499::system::EMB_RES> resource("DeinitializeTest_RejectingDI"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"SRC"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1DINT"_STRID},
                                               }) == EMGMResponse::Ready);
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"DST"_STRID},
                                                   .mSecondParam = {"test::DeinitializeTest_RejectingDI"_STRID},
                                               }) == EMGMResponse::Ready);
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateConnection,
                                                .mFirstParam = {"SRC"_STRID, "OUT"_STRID},
                                                .mSecondParam = {"DST"_STRID, "IN"_STRID},
                                            }) != EMGMResponse::Ready);

    CFunctionBlock *dst = findFB(*resource, "DST"_STRID);
    BOOST_REQUIRE(dst != nullptr);
    BOOST_TEST(dst->getDIConnection("IN"_STRID) == nullptr);

    resource->deinitialize();
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
