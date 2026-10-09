/*******************************************************************************
 * Copyright (c) 2026 Martin Erich Jobst
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

#include <boost/test/unit_test.hpp>

#include "forte/datatypes/forte_any_variant.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/funcbloc.h"
#include "forte/mgmcmdstruct.h"

#include "forte_boost_output_support.h"
#include "fbtests/fbtesterglobalfixture.h"

using namespace forte::literals;

namespace forte::test {

  namespace {
    const auto cDataInOutNames = std::array{"INOUT"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = {},
        .mEITypeNames = {},
        .mEONames = {},
        .mEOTypeNames = {},
        .mDINames = {},
        .mDONames = {},
        .mDIONames = cDataInOutNames,
        .mSocketNames = {},
        .mPlugNames = {},
    };

    template<typename T>
    class CInOutConnectionTestFB : public CFunctionBlock {
      public:
        CInOutConnectionTestFB(const StringId paInstanceNameId, CFBContainer &paContainer) :
            CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
            mInOutConnection(nullptr),
            mOutInOutConnection(*this, 0, mInOut) {
        }

        CIEC_ANY *getDI(size_t) override {
          return nullptr;
        }

        CIEC_ANY *getDO(size_t) override {
          return nullptr;
        }

        CIEC_ANY *getDIO(size_t paIndex) override {
          return paIndex == 0 ? &mInOut : nullptr;
        }

      protected:
        CEventConnection *getEOConUnchecked(TPortId) override {
          return nullptr;
        }

        CDataConnection **getDIConUnchecked(TPortId) override {
          return nullptr;
        }

        CDataConnection *getDOConUnchecked(TPortId) override {
          return nullptr;
        }

        CInOutDataConnection **getDIOInConUnchecked(TPortId paIndex) override {
          return paIndex == 0 ? &mInOutConnection : nullptr;
        }

        CInOutDataConnection *getDIOOutConUnchecked(TPortId paIndex) override {
          return paIndex == 0 ? &mOutInOutConnection : nullptr;
        }

        void setInitialValues() override {
          mInOut.reset();
          mOutInOutConnection.getDefiningValue().reset();
        }

      private:
        void executeEvent(TEventID, CEventChainExecutionThread *const) override {
        }

        void readInputData(TEventID) override {
        }

        void writeOutputData(TEventID) override {
        }

        T mInOut;
        CInOutDataConnection *mInOutConnection;
        COutInOutDataConnection<T> mOutInOutConnection;
    };

    class CGenericInOutConnectionTestFB final : public CInOutConnectionTestFB<CIEC_ANY_VARIANT> {
        DECLARE_FIRMWARE_FB(CGenericInOutConnectionTestFB)

      public:
        using CInOutConnectionTestFB::CInOutConnectionTestFB;
    };

    class CBoolInOutConnectionTestFB final : public CInOutConnectionTestFB<CIEC_BOOL> {
        DECLARE_FIRMWARE_FB(CBoolInOutConnectionTestFB)

      public:
        using CInOutConnectionTestFB::CInOutConnectionTestFB;
    };

    DEFINE_FIRMWARE_FB(CGenericInOutConnectionTestFB, "test::GenericInOutConnectionTestFB"_STRID)
    DEFINE_FIRMWARE_FB(CBoolInOutConnectionTestFB, "test::BoolInOutConnectionTestFB"_STRID)

    EMGMResponse executeMGMCommand(CResource &paResource, const SManagementCMD &paCommand) {
      SManagementCMD command = paCommand;
      return paResource.executeMGMCommand(command);
    }
  } // namespace

  BOOST_AUTO_TEST_SUITE(InOutConnectionTest)

  BOOST_AUTO_TEST_CASE(createConnection) {
    CInternalFB<iec61499::system::EMB_RES> resource("InOutConnectionTest_createConnection"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"SOURCE"_STRID},
                                                   .mSecondParam = {"test::BoolInOutConnectionTestFB"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"DESTINATION"_STRID},
                                                   .mSecondParam = {"test::BoolInOutConnectionTestFB"_STRID},
                                               }) == EMGMResponse::Ready);

    // create connection
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateConnection,
                                                .mFirstParam = {"SOURCE"_STRID, "INOUT"_STRID},
                                                .mSecondParam = {"DESTINATION"_STRID, "INOUT"_STRID},
                                            }) == EMGMResponse::Ready);

    // create connection (exists)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateConnection,
                                                .mFirstParam = {"SOURCE"_STRID, "INOUT"_STRID},
                                                .mSecondParam = {"DESTINATION"_STRID, "INOUT"_STRID},
                                            }) == EMGMResponse::InvalidState);

    resource->deinitialize();
  }

  BOOST_AUTO_TEST_CASE(deleteConnection) {
    CInternalFB<iec61499::system::EMB_RES> resource("InOutConnectionTest_deleteConnection"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"SOURCE"_STRID},
                                                   .mSecondParam = {"test::BoolInOutConnectionTestFB"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"DESTINATION"_STRID},
                                                   .mSecondParam = {"test::BoolInOutConnectionTestFB"_STRID},
                                               }) == EMGMResponse::Ready);

    // delete connection (does not exist)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteConnection,
                                                .mFirstParam = {"SOURCE"_STRID, "INOUT"_STRID},
                                                .mSecondParam = {"DESTINATION"_STRID, "INOUT"_STRID},
                                            }) == EMGMResponse::NoSuchObject);

    // create connection
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateConnection,
                                                   .mFirstParam = {"SOURCE"_STRID, "INOUT"_STRID},
                                                   .mSecondParam = {"DESTINATION"_STRID, "INOUT"_STRID},
                                               }) == EMGMResponse::Ready);

    // delete connection (wrong order does not exist)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteConnection,
                                                .mFirstParam = {"DESTINATION"_STRID, "INOUT"_STRID},
                                                .mSecondParam = {"SOURCE"_STRID, "INOUT"_STRID},
                                            }) == EMGMResponse::NoSuchObject);

    // delete connection
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteConnection,
                                                .mFirstParam = {"SOURCE"_STRID, "INOUT"_STRID},
                                                .mSecondParam = {"DESTINATION"_STRID, "INOUT"_STRID},
                                            }) == EMGMResponse::Ready);

    // delete connection (does not exist)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteConnection,
                                                .mFirstParam = {"SOURCE"_STRID, "INOUT"_STRID},
                                                .mSecondParam = {"DESTINATION"_STRID, "INOUT"_STRID},
                                            }) == EMGMResponse::NoSuchObject);

    resource->deinitialize();
  }

  BOOST_AUTO_TEST_CASE(queryConnection) {
    CInternalFB<iec61499::system::EMB_RES> resource("InOutConnectionTest_queryConnection"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"SOURCE"_STRID},
                                                   .mSecondParam = {"test::BoolInOutConnectionTestFB"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"DESTINATION"_STRID},
                                                   .mSecondParam = {"test::BoolInOutConnectionTestFB"_STRID},
                                               }) == EMGMResponse::Ready);

    // create connection
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateConnection,
                                                   .mFirstParam = {"SOURCE"_STRID, "INOUT"_STRID},
                                                   .mSecondParam = {"DESTINATION"_STRID, "INOUT"_STRID},
                                               }) == EMGMResponse::Ready);

    // query connection
    SManagementCMD queryCommand = {
        .mCMD = EMGMCommandType::QueryConnection,
    };
    BOOST_TEST(resource->executeMGMCommand(queryCommand) == EMGMResponse::Ready);
    BOOST_TEST(queryCommand.mAdditionalParams ==
               "<Connection Source=\"SOURCE.INOUT\" Destination=\"DESTINATION.INOUT\"/>\n");

    resource->deinitialize();
  }

  BOOST_AUTO_TEST_CASE(configureGenericSourceToInOut) {
    CInternalFB<iec61499::system::EMB_RES> resource("InOutConnectionTest_configureGenericSource"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"SOURCE"_STRID},
                                                   .mSecondParam = {"test::GenericInOutConnectionTestFB"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"DESTINATION"_STRID},
                                                   .mSecondParam = {"test::BoolInOutConnectionTestFB"_STRID},
                                               }) == EMGMResponse::Ready);

    // create connection
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateConnection,
                                                   .mFirstParam = {"SOURCE"_STRID, "INOUT"_STRID},
                                                   .mSecondParam = {"DESTINATION"_STRID, "INOUT"_STRID},
                                               }) == EMGMResponse::Ready);

    CIEC_ANY *source = resource->getVar(std::array{"SOURCE"_STRID, "INOUT"_STRID});
    BOOST_REQUIRE(source != nullptr);
    BOOST_TEST(source->unwrap().getDataTypeID() == CIEC_ANY::e_BOOL);

    resource->deinitialize();
  }

  BOOST_AUTO_TEST_CASE(configureGenericSourceToInput) {
    CInternalFB<iec61499::system::EMB_RES> resource("InOutConnectionTest_configureGenericSource"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"SOURCE"_STRID},
                                                   .mSecondParam = {"test::GenericInOutConnectionTestFB"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"DESTINATION"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1BOOL"_STRID},
                                               }) == EMGMResponse::Ready);

    // create connection
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateConnection,
                                                   .mFirstParam = {"SOURCE"_STRID, "INOUT"_STRID},
                                                   .mSecondParam = {"DESTINATION"_STRID, "IN"_STRID},
                                               }) == EMGMResponse::Ready);

    CIEC_ANY *source = resource->getVar(std::array{"SOURCE"_STRID, "INOUT"_STRID});
    BOOST_REQUIRE(source != nullptr);
    BOOST_TEST(source->unwrap().getDataTypeID() == CIEC_ANY::e_BOOL);

    resource->deinitialize();
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::test
