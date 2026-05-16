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

  BOOST_AUTO_TEST_SUITE(NegatingConnectionTest)

  BOOST_AUTO_TEST_CASE(createConnection) {
    CInternalFB<iec61499::system::EMB_RES> resource("NegatingConnectionTest_createConnection"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_1"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1BOOL"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_2"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1BOOL"_STRID},
                                               }) == EMGMResponse::Ready);

    // create connection
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID, "NOT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                            }) == EMGMResponse::Ready);

    // create connection (exists)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID, "NOT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                            }) == EMGMResponse::InvalidState);
  }

  BOOST_AUTO_TEST_CASE(deleteConnection) {
    CInternalFB<iec61499::system::EMB_RES> resource("NegatingConnectionTest_deleteConnection"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_1"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1BOOL"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_2"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1BOOL"_STRID},
                                               }) == EMGMResponse::Ready);

    // delete connection (does not exist)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID, "NOT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                            }) == EMGMResponse::NoSuchObject);

    // create connection
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateConnection,
                                                   .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID, "NOT"_STRID},
                                                   .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                               }) == EMGMResponse::Ready);

    // delete connection (w/o "NOT" does not exist)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                            }) == EMGMResponse::NoSuchObject);

    // delete connection
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID, "NOT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                            }) == EMGMResponse::Ready);

    // delete connection (does not exist)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID, "NOT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                            }) == EMGMResponse::NoSuchObject);
  }

  BOOST_AUTO_TEST_CASE(queryConnection) {
    CInternalFB<iec61499::system::EMB_RES> resource("NegatingConnectionTest_queryConnection"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_1"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1BOOL"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_2"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1BOOL"_STRID},
                                               }) == EMGMResponse::Ready);

    // create connection
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateConnection,
                                                   .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID, "NOT"_STRID},
                                                   .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                               }) == EMGMResponse::Ready);

    // query connection
    SManagementCMD queryCommand = {
        .mCMD = EMGMCommandType::QueryConnection,
    };
    BOOST_TEST(resource->executeMGMCommand(queryCommand) == EMGMResponse::Ready);
    BOOST_TEST(queryCommand.mAdditionalParams ==
               "<Connection Source=\"F_MOVE_1.OUT.NOT\" Destination=\"F_MOVE_2.IN\"/>\n");
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::test
