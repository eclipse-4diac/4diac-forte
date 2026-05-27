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

#include "forte/mgmcmdstruct.h"

#include "forte_boost_output_support.h"
#include "fbtests/fbtesterglobalfixture.h"

using namespace forte::literals;

namespace forte::test {

  namespace {
    EMGMResponse executeMGMCommand(CResource &paResource, const SManagementCMD &paCommand) {
      SManagementCMD command = paCommand;
      return paResource.executeMGMCommand(command);
    }
  } // namespace

  BOOST_AUTO_TEST_SUITE(MGMCommandTest)

  BOOST_AUTO_TEST_CASE(createFB) {
    CInternalFB<iec61499::system::EMB_RES> resource("MGMCommandTest_createFB"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    // create FB
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateFBInstance,
                                                .mFirstParam = {"F_MOVE"_STRID},
                                                .mSecondParam = {"iec61131::selection::F_MOVE_1DINT"_STRID},
                                            }) == EMGMResponse::Ready);

    // create FB (exists)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateFBInstance,
                                                .mFirstParam = {"F_MOVE"_STRID},
                                                .mSecondParam = {"iec61131::selection::F_MOVE_1DINT"_STRID},
                                            }) == EMGMResponse::InvalidState);

    resource->deinitialize();
  }

  BOOST_AUTO_TEST_CASE(deleteFB) {
    CInternalFB<iec61499::system::EMB_RES> resource("MGMCommandTest_deleteFB"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    // delete FB (does not exist)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteFBInstance,
                                                .mFirstParam = {"F_MOVE"_STRID},
                                            }) == EMGMResponse::NoSuchObject);

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1DINT"_STRID},
                                               }) == EMGMResponse::Ready);

    // delete FB
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteFBInstance,
                                                .mFirstParam = {"F_MOVE"_STRID},
                                            }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1DINT"_STRID},
                                               }) == EMGMResponse::Ready);

    resource->deinitialize();
  }

  BOOST_AUTO_TEST_CASE(createConnection) {
    CInternalFB<iec61499::system::EMB_RES> resource("MGMCommandTest_createConnection"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_1"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1DINT"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_2"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1DINT"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_3"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1DINT"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_4"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1STRING"_STRID},
                                               }) == EMGMResponse::Ready);

    // create connection (F_MOVE_1.IN is not an output)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "IN"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                            }) == EMGMResponse::NoSuchObject);

    // create connection (F_MOVE_2.OUT is not an input)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "OUT"_STRID},
                                            }) == EMGMResponse::NoSuchObject);

    // create connection (incompatible types from F_MOVE_1.OUT (DINT) to F_MOVE_4.IN (STRING))
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                .mSecondParam = {"F_MOVE_4"_STRID, "IN"_STRID},
                                            }) == EMGMResponse::InvalidOperation);

    // create connection
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                            }) == EMGMResponse::Ready);

    // create connection (same endpoints are allowed)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                            }) == EMGMResponse::Ready);

    // create connection (input already connected)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateConnection,
                                                .mFirstParam = {"F_MOVE_3"_STRID, "OUT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                            }) == EMGMResponse::InvalidState);

    resource->deinitialize();
  }

  BOOST_AUTO_TEST_CASE(deleteConnection) {
    CInternalFB<iec61499::system::EMB_RES> resource("MGMCommandTest_deleteConnection"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_1"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1DINT"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_2"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1DINT"_STRID},
                                               }) == EMGMResponse::Ready);

    // delete connection (does not exist)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                            }) == EMGMResponse::NoSuchObject);

    // create connection
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateConnection,
                                                   .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                   .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                               }) == EMGMResponse::Ready);

    // delete connection
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                            }) == EMGMResponse::Ready);

    resource->deinitialize();
  }

  BOOST_AUTO_TEST_CASE(queryFB) {
    CInternalFB<iec61499::system::EMB_RES> resource("MGMCommandTest_queryFB"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    // query FB (empty)
    SManagementCMD queryCommand = {
        .mCMD = EMGMCommandType::QueryFB,
    };
    BOOST_TEST(resource->executeMGMCommand(queryCommand) == EMGMResponse::Ready);
    BOOST_TEST(queryCommand.mAdditionalParams ==
               "<FB Name=\"START\" Type=\"iec61499::events::E_RESTART\" Status=\"IDLE\"/>\n");

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_1"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1DINT"_STRID},
                                               }) == EMGMResponse::Ready);

    // query FB
    queryCommand = {
        .mCMD = EMGMCommandType::QueryFB,
    };
    BOOST_TEST(resource->executeMGMCommand(queryCommand) == EMGMResponse::Ready);
    BOOST_TEST(queryCommand.mAdditionalParams ==
               "<FB Name=\"F_MOVE_1\" Type=\"iec61131::selection::F_MOVE_1DINT\" Status=\"IDLE\"/>\n"
               "<FB Name=\"START\" Type=\"iec61499::events::E_RESTART\" Status=\"IDLE\"/>\n");

    resource->deinitialize();
  }

  BOOST_AUTO_TEST_CASE(queryConnection) {
    CInternalFB<iec61499::system::EMB_RES> resource("MGMCommandTest_queryConnection"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_1"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1DINT"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_2"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1DINT"_STRID},
                                               }) == EMGMResponse::Ready);

    // query connection (empty)
    SManagementCMD queryCommand = {
        .mCMD = EMGMCommandType::QueryConnection,
    };
    BOOST_TEST(resource->executeMGMCommand(queryCommand) == EMGMResponse::Ready);
    BOOST_TEST(queryCommand.mAdditionalParams == "");

    // create connection
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateConnection,
                                                   .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                   .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                               }) == EMGMResponse::Ready);

    // query connection
    queryCommand = {
        .mCMD = EMGMCommandType::QueryConnection,
    };
    BOOST_TEST(resource->executeMGMCommand(queryCommand) == EMGMResponse::Ready);
    BOOST_TEST(queryCommand.mAdditionalParams == "<Connection Source=\"F_MOVE_1.OUT\" Destination=\"F_MOVE_2.IN\"/>\n");

    resource->deinitialize();
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::test
