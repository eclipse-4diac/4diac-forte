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

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
