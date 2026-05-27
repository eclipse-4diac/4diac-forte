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
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_struct.h"

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

  class CIEC_GatheringConnectionTestStruct : public CIEC_STRUCT {
      DECLARE_FIRMWARE_DATATYPE(GatheringConnectionTestStruct)
    public:
      CIEC_STRING Var1;
      CIEC_BOOL Var2;
      CIEC_INT Var3;

      CIEC_GatheringConnectionTestStruct() = default;

      CIEC_GatheringConnectionTestStruct(CIEC_STRING paVar1, CIEC_BOOL paVar2, CIEC_INT paVar3) :
          Var1(paVar1),
          Var2(paVar2),
          Var3(paVar3) {};

      size_t getStructSize() const override {
        return 3;
      }

      const StringId *elementNames() const override {
        return scmElementNames;
      }

      StringId getStructTypeNameID() const override {
        return "AnyDerivedTestStruct"_STRID;
      }

      CIEC_ANY *getMember(size_t paGatheringIndex) override {
        switch (paGatheringIndex) {
          case 0: return &Var1;
          case 1: return &Var2;
          case 2: return &Var3;
        }
        return nullptr;
      }

      const CIEC_ANY *getMember(size_t paGatheringIndex) const override {
        switch (paGatheringIndex) {
          case 0: return &Var1;
          case 1: return &Var2;
          case 2: return &Var3;
        }
        return nullptr;
      }

    private:
      static const StringId scmElementNames[];
  };

  const StringId CIEC_GatheringConnectionTestStruct::scmElementNames[] = {"Var1"_STRID, "Var2"_STRID, "Var3"_STRID};

  DEFINE_FIRMWARE_DATATYPE(GatheringConnectionTestStruct, "GatheringConnectionTestStruct"_STRID)

  BOOST_AUTO_TEST_SUITE(GatheringConnectionTest)

  BOOST_AUTO_TEST_CASE(createConnection) {
    CInternalFB<iec61499::system::EMB_RES> resource("GatheringConnectionTest_createConnection"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_1"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1STRING"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(
        executeMGMCommand(*resource,
                          {
                              .mCMD = EMGMCommandType::CreateFBInstance,
                              .mFirstParam = {"F_MOVE_2"_STRID},
                              .mSecondParam = {"iec61131::selection::F_MOVE_1GatheringConnectionTestStruct"_STRID},
                          }) == EMGMResponse::Ready);

    // create connection
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID, "Var1"_STRID},
                                            }) == EMGMResponse::Ready);

    // create connection (exists)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID, "Var1"_STRID},
                                            }) == EMGMResponse::InvalidState);

    resource->deinitialize();
  }

  BOOST_AUTO_TEST_CASE(deleteConnection) {
    CInternalFB<iec61499::system::EMB_RES> resource("GatheringConnectionTest_deleteConnection"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_1"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1STRING"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(
        executeMGMCommand(*resource,
                          {
                              .mCMD = EMGMCommandType::CreateFBInstance,
                              .mFirstParam = {"F_MOVE_2"_STRID},
                              .mSecondParam = {"iec61131::selection::F_MOVE_1GatheringConnectionTestStruct"_STRID},
                          }) == EMGMResponse::Ready);

    // delete connection (does not exist)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID, "Var1"_STRID},
                                            }) == EMGMResponse::NoSuchObject);

    // create connection
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateConnection,
                                                   .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                   .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID, "Var1"_STRID},
                                               }) == EMGMResponse::Ready);

    // delete connection (w/o "Var1" does not exist)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID},
                                            }) == EMGMResponse::NoSuchObject);

    // delete connection
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID, "Var1"_STRID},
                                            }) == EMGMResponse::Ready);

    // delete connection (does not exist)
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteConnection,
                                                .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID, "Var1"_STRID},
                                            }) == EMGMResponse::NoSuchObject);

    resource->deinitialize();
  }

  BOOST_AUTO_TEST_CASE(queryConnection) {
    CInternalFB<iec61499::system::EMB_RES> resource("GatheringConnectionTest_queryConnection"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());

    // create FB
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateFBInstance,
                                                   .mFirstParam = {"F_MOVE_1"_STRID},
                                                   .mSecondParam = {"iec61131::selection::F_MOVE_1STRING"_STRID},
                                               }) == EMGMResponse::Ready);

    // create FB
    BOOST_REQUIRE(
        executeMGMCommand(*resource,
                          {
                              .mCMD = EMGMCommandType::CreateFBInstance,
                              .mFirstParam = {"F_MOVE_2"_STRID},
                              .mSecondParam = {"iec61131::selection::F_MOVE_1GatheringConnectionTestStruct"_STRID},
                          }) == EMGMResponse::Ready);

    // create connection
    BOOST_REQUIRE(executeMGMCommand(*resource, {
                                                   .mCMD = EMGMCommandType::CreateConnection,
                                                   .mFirstParam = {"F_MOVE_1"_STRID, "OUT"_STRID},
                                                   .mSecondParam = {"F_MOVE_2"_STRID, "IN"_STRID, "Var1"_STRID},
                                               }) == EMGMResponse::Ready);

    // query connection
    SManagementCMD queryCommand = {
        .mCMD = EMGMCommandType::QueryConnection,
    };
    BOOST_TEST(resource->executeMGMCommand(queryCommand) == EMGMResponse::Ready);
    BOOST_TEST(queryCommand.mAdditionalParams ==
               "<Connection Source=\"F_MOVE_1.OUT\" Destination=\"F_MOVE_2.IN.Var1\"/>\n");

    resource->deinitialize();
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::test
