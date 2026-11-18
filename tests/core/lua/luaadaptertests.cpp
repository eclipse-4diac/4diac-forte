/*******************************************************************************
 * Copyright (c) 2026 Aimirim STI
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * AI Disclosure: This file was largely AI-generated. The AI-generated
 * portions are made available under CC0-1.0 and not subject to the
 * project's licence. The human contributor has reviewed and verified
 * that the code is correct.
 *
 * SPDX-License-Identifier: EPL-2.0 AND CC0-1.0
 * Assisted-by: Anthropic Claude Opus 5.5 (claude-opus-5-5)
 *
 * Contributors:
 *   Pedro Ricardo C. Souza - initial implementation
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

#include <chrono>
#include <string>
#include <thread>
#include <vector>

#include "../../../core/src/anyadapter.h"
#include "../../../core/src/lua/luaadapter.h"
#include "../../../core/src/lua/luaengine.h"

#include "forte/ecet.h"
#include "forte/resource.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_int.h"
#include "forte/util/criticalregion.h"

#include "forte_boost_output_support.h"
#include "../fbtests/fbtesterglobalfixture.h"

using namespace forte::literals;
using forte::test::CFBTestDataGlobalFixture;

namespace forte::lua::test {

  namespace {
    //! One state reads every module, as the device's definition state does
    CLuaEngine &definitionEngine() {
      static CLuaEngine engine(CLuaEngine::scmDefinitionInstructionLimit);
      return engine;
    }

    // ACounter from the IDE examples: REQ samples PV, CNF carries Q and CV
    constexpr char cCounterAdapter[] = R"lua(
local interfaceSpec = {
  numEIs = 2,
  EINames = {"REQ", "R"},
  EIWith = {0, 65535},
  EIWithIndexes = {0, -1},
  numEOs = 1,
  EONames = {"CNF"},
  EOWith = {0, 1, 65535},
  EOWithIndexes = {0},
  numDIs = 1,
  DINames = {"PV"},
  DIDataTypeNames = {"INT"},
  numDOs = 2,
  DONames = {"Q", "CV"},
  DODataTypeNames = {"BOOL", "INT"}
}

return {interfaceSpec = interfaceSpec}
)lua";

    //! An adapter module whose interface is given as Lua table fields
    std::string adapterModule(const std::string &paFields) {
      return "return {interfaceSpec = {" + paFields + "}}";
    }

    const std::string cEmptyEvents = "numEIs = 0, EINames = {}, EIWith = {}, EIWithIndexes = {}, "
                                     "numEOs = 0, EONames = {}, EOWith = {}, EOWithIndexes = {}, ";

    std::string dataModule(const std::string &paTypes, std::size_t paCount) {
      std::string names;
      for (std::size_t i = 0; i < paCount; ++i) {
        names += (i == 0 ? "\"D" : ", \"D") + std::to_string(i) + "\"";
      }
      return adapterModule(cEmptyEvents + "numDIs = " + std::to_string(paCount) + ", DINames = {" + names +
                           "}, DIDataTypeNames = {" + paTypes + "}, numDOs = 0, DONames = {}, DODataTypeNames = {}");
    }

    EMGMResponse create(StringId paName, const std::string &paSource, std::shared_ptr<CLuaAdapterTypeEntry> &paEntry) {
      std::string error;
      const EMGMResponse response = CLuaAdapterTypeEntry::create(definitionEngine(), paName, paSource, paEntry, error);
      BOOST_TEST_MESSAGE("create " << paName.data() << ": " << error);
      return response;
    }

    EMGMResponse createResponse(const std::string &paSource) {
      std::shared_ptr<CLuaAdapterTypeEntry> entry;
      return create("luaadaptertest::Invalid"_STRID, paSource, entry);
    }

    std::shared_ptr<CLuaAdapterTypeEntry> registerCounterAdapter(StringId paName) {
      std::shared_ptr<CLuaAdapterTypeEntry> entry;
      BOOST_REQUIRE(create(paName, cCounterAdapter, entry) == EMGMResponse::Ready);
      BOOST_REQUIRE(registerRuntimeAdapterType(entry) == EMGMResponse::Ready);
      return entry;
    }

    const auto cHostEventInputNames = std::array{"FWD"_STRID};
    const auto cPlugNames = std::array{"P"_STRID};
    const auto cSocketNames = std::array{"S"_STRID};

    const SFBInterfaceSpec cPlugHostSpec = {
        .mEINames = cHostEventInputNames,
        .mEITypeNames = {},
        .mEONames = {},
        .mEOTypeNames = {},
        .mDINames = {},
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = cPlugNames,
    };

    const SFBInterfaceSpec cSocketHostSpec = {
        .mEINames = cHostEventInputNames,
        .mEITypeNames = {},
        .mEONames = {},
        .mEOTypeNames = {},
        .mDINames = {},
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = cSocketNames,
        .mPlugNames = {},
    };

    /*!\brief A function block with one adapter pin of a type known by name
     *
     * Its event input FWD sends the configured event through the adapter, adapter events received are recorded.
     */
    class CAdapterHost final : public CFunctionBlock {
      public:
        CAdapterHost(StringId paInstanceNameId, CFBContainer &paContainer, StringId paAdapterTypeId, bool paIsPlug) :
            CFunctionBlock(paContainer, paIsPlug ? cPlugHostSpec : cSocketHostSpec, paInstanceNameId) {
          if (paIsPlug) {
            mPlug = std::make_unique<CTypedPlugPin>("P"_STRID, *this, paAdapterTypeId, 0);
          } else {
            mSocket = std::make_unique<CTypedSocketPin>("S"_STRID, *this, paAdapterTypeId, 0);
          }
        }

        StringId getFBTypeId() const override {
          return "luaadaptertest::Host"_STRID;
        }

        CAdapter *getAdapterBlock() {
          return mPlug ? mPlug->getAdapterBlock() : mSocket->getAdapterBlock();
        }

        CTypedPlugPin &getPlug() {
          return *mPlug;
        }

        void setForwardedEvent(TEventID paEvent) {
          mForwardedEvent = paEvent;
        }

        std::vector<TEventID> takeReceivedEvents() {
          util::CCriticalRegion region(mLock);
          return std::exchange(mReceivedEvents, {});
        }

        //! Sends FWD through the resource's event chain thread and waits for the chain to finish
        void forward() {
          CEventChainExecutionThread *const thread = getResource()->getResourceEventExecution();
          thread->startEventChain(TEventEntry(*this, 0));
          do {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
          } while (thread->isProcessingEvents());
        }

        CIEC_ANY *getDI(TPortId) override {
          return nullptr;
        }

        CIEC_ANY *getDO(TPortId) override {
          return nullptr;
        }

      protected:
        void setInitialValues() override {
        }

        CEventConnection *getEOConUnchecked(TPortId) override {
          return nullptr;
        }

        CDataConnection *getDOConUnchecked(TPortId) override {
          return nullptr;
        }

        CDataConnection **getDIConUnchecked(TPortId) override {
          return nullptr;
        }

        IPlugPin *getPlugPinUnchecked(TPortId) override {
          return mPlug.get();
        }

        ISocketPin *getSocketPinUnchecked(TPortId) override {
          return mSocket.get();
        }

      private:
        void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override {
          if (paEIID == 0) {
            sendAdapterEvent(*getAdapterBlock(), mForwardedEvent, paECET);
          } else {
            util::CCriticalRegion region(mLock);
            mReceivedEvents.push_back(paEIID);
          }
        }

        void readInputData(TEventID) override {
        }

        void writeOutputData(TEventID) override {
        }

        std::unique_ptr<CTypedPlugPin> mPlug;
        std::unique_ptr<CTypedSocketPin> mSocket;
        TEventID mForwardedEvent = 0;
        arch::CSyncObject mLock;
        std::vector<TEventID> mReceivedEvents;
    };

    //! A plug host and a socket host of one adapter type, started and connected
    struct SConnectedHosts {
        SConnectedHosts(StringId paAdapterTypeId) :
            mPlugHost("PlugHost"_STRID, CFBTestDataGlobalFixture::getResource(), paAdapterTypeId, true),
            mSocketHost("SocketHost"_STRID, CFBTestDataGlobalFixture::getResource(), paAdapterTypeId, false) {
          BOOST_REQUIRE(mPlugHost.initialize());
          BOOST_REQUIRE(mSocketHost.initialize());
          BOOST_REQUIRE(mPlugHost.changeExecutionState(EMGMCommandType::Start) == EMGMResponse::Ready);
          BOOST_REQUIRE(mSocketHost.changeExecutionState(EMGMCommandType::Start) == EMGMResponse::Ready);
          BOOST_REQUIRE(mPlugHost.getPlug().getAdapterCon().connect(mSocketHost, std::array{"S"_STRID}) ==
                        EMGMResponse::Ready);
        }

        ~SConnectedHosts() {
          mPlugHost.getPlug().getAdapterCon().disconnect(mSocketHost, std::array{"S"_STRID});
        }

        CAdapterHost mPlugHost;
        CAdapterHost mSocketHost;
    };

    TForteInt16 intValue(const CIEC_ANY &paValue) {
      return static_cast<TForteInt16>(static_cast<const CIEC_INT &>(paValue));
    }

    std::vector<StringId> names(std::span<const StringId> paNames) {
      return {paNames.begin(), paNames.end()};
    }
  } // namespace

  BOOST_AUTO_TEST_SUITE(LuaAdapterTests)

  BOOST_AUTO_TEST_CASE(interfaceIsRead) {
    std::shared_ptr<CLuaAdapterTypeEntry> entry;
    BOOST_REQUIRE(create("luaadaptertest::Read"_STRID, cCounterAdapter, entry) == EMGMResponse::Ready);
    BOOST_TEST(entry->getDefinition() == std::string_view(cCounterAdapter));

    const CLuaInterface &interface = entry->getInterface();
    const SFBInterfaceSpec &spec = interface.getSpec();
    BOOST_TEST(names(spec.mEINames) == (std::vector{"REQ"_STRID, "R"_STRID}));
    BOOST_TEST(names(spec.mEONames) == (std::vector{"CNF"_STRID}));
    BOOST_TEST(names(spec.mDINames) == (std::vector{"PV"_STRID}));
    BOOST_TEST(names(spec.mDONames) == (std::vector{"Q"_STRID, "CV"_STRID}));
    BOOST_TEST(interface.getEIWith(0).size() == 1U);
    BOOST_TEST(interface.getEIWith(1).empty());
    BOOST_TEST(interface.getEOWith(0).size() == 2U);
    BOOST_TEST(interface.getEOWith(0)[1] == 1U);
    BOOST_TEST(interface.getAdapters().empty());

    const SFBInterfaceSpec &mirrored = interface.getMirroredSpec();
    BOOST_TEST(names(mirrored.mEINames) == (std::vector{"CNF"_STRID}));
    BOOST_TEST(names(mirrored.mEONames) == (std::vector{"REQ"_STRID, "R"_STRID}));
    BOOST_TEST(names(mirrored.mDINames) == (std::vector{"Q"_STRID, "CV"_STRID}));
    BOOST_TEST(names(mirrored.mDONames) == (std::vector{"PV"_STRID}));
  }

  BOOST_AUTO_TEST_CASE(arraysAndStructs) {
    std::shared_ptr<CLuaAdapterTypeEntry> entry;
    BOOST_REQUIRE(create("luaadaptertest::Arrays"_STRID,
                         dataModule("\"ARRAY [1..4] OF INT\", \"ARRAY\", 3, \"REAL\", \"ARRAY[-2..-1] OF BOOL\", "
                                    "\"lua::test::LuaEngineTestStruct\"",
                                    4),
                         entry) == EMGMResponse::Ready);
    const auto types = entry->getInterface().getDITypes();
    BOOST_REQUIRE(types.size() == 4U);

    auto values = CLuaInterface::createValues(types);
    const std::vector<std::pair<intmax_t, intmax_t>> bounds = {{1, 4}, {0, 2}, {-2, -1}};
    for (std::size_t i = 0; i < bounds.size(); ++i) {
      BOOST_REQUIRE(values[i]->getDataTypeID() == CIEC_ANY::e_ARRAY);
      const auto &array = static_cast<const CIEC_ARRAY &>(*values[i]);
      BOOST_TEST(array.getLowerBound() == bounds[i].first);
      BOOST_TEST(array.getUpperBound() == bounds[i].second);
    }
    BOOST_TEST(static_cast<const CIEC_ARRAY &>(*values[0])[1].getDataTypeID() == CIEC_ANY::e_INT);
    BOOST_TEST(values[3]->getDataTypeID() == CIEC_ANY::e_STRUCT);
    BOOST_TEST(values[3]->getTypeNameID() == "lua::test::LuaEngineTestStruct"_STRID);
  }

  BOOST_AUTO_TEST_CASE(moduleErrors) {
    BOOST_TEST(createResponse("return {interfaceSpec = ") == EMGMResponse::InvalidObject);
    BOOST_TEST(createResponse("error('boom')") == EMGMResponse::InvalidObject);
    BOOST_TEST(createResponse("return {}") == EMGMResponse::BadParams);
    BOOST_TEST(createResponse("return {interfaceSpec = 5}") == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(interfaceErrors) {
    const std::string events = "EIWith = {}, EIWithIndexes = {-1}, numEOs = 0, EONames = {}, EOWith = {}, "
                               "EOWithIndexes = {}, ";
    const std::string data = "numDIs = 1, DINames = {\"D\"}, DIDataTypeNames = {\"INT\"}, numDOs = 0, DONames = {}, "
                             "DODataTypeNames = {}";
    // the valid base case
    BOOST_TEST(createResponse(adapterModule("numEIs = 1, EINames = {\"E\"}, " + events + data)) == EMGMResponse::Ready);
    // counts that do not match the lists
    BOOST_TEST(createResponse(adapterModule("numEIs = 2, EINames = {\"E\"}, " + events + data)) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(adapterModule("numEIs = 1.5, EINames = {\"E\"}, " + events + data)) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(adapterModule("numEIs = 256, EINames = {\"E\"}, " + events + data)) ==
               EMGMResponse::BadParams);
    // invalid and duplicate names
    BOOST_TEST(createResponse(adapterModule("numEIs = 1, EINames = {\"A.B\"}, " + events + data)) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(adapterModule("numEIs = 1, EINames = {1}, " + events + data)) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(adapterModule(cEmptyEvents + "numDIs = 2, DINames = {\"D\", \"D\"}, "
                                                           "DIDataTypeNames = {\"INT\", \"INT\"}, numDOs = 0, "
                                                           "DONames = {}, DODataTypeNames = {}")) ==
               EMGMResponse::BadParams);
    // missing list
    BOOST_TEST(createResponse(adapterModule("numEIs = 1, EINames = {\"E\"}, EIWith = {}, numEOs = 0, EONames = {}, "
                                            "EOWith = {}, EOWithIndexes = {}, " +
                                            data)) == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(withErrors) {
    const std::string tail = "numEOs = 0, EONames = {}, EOWith = {}, EOWithIndexes = {}, numDIs = 1, "
                             "DINames = {\"D\"}, DIDataTypeNames = {\"INT\"}, numDOs = 0, DONames = {}, "
                             "DODataTypeNames = {}";
    const auto withModule = [&tail](const std::string &paWith, const std::string &paIndexes) {
      return adapterModule("numEIs = 1, EINames = {\"E\"}, EIWith = {" + paWith + "}, EIWithIndexes = {" + paIndexes +
                           "}, " + tail);
    };
    BOOST_TEST(createResponse(withModule("0, 65535", "0")) == EMGMResponse::Ready);
    // a data input that does not exist
    BOOST_TEST(createResponse(withModule("1, 65535", "0")) == EMGMResponse::BadParams);
    // a run without delimiter
    BOOST_TEST(createResponse(withModule("0", "0")) == EMGMResponse::BadParams);
    // an offset outside the list
    BOOST_TEST(createResponse(withModule("0, 65535", "2")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withModule("0, 65535", "-2")) == EMGMResponse::BadParams);
    // one offset per event
    BOOST_TEST(createResponse(withModule("0, 65535", "0, 0")) == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(dataTypeErrors) {
    BOOST_TEST(createResponse(dataModule("\"NOT_A_TYPE\"", 1)) == EMGMResponse::UnsupportedType);
    BOOST_TEST(createResponse(dataModule("\"ARRAY [0..3] OF NOT_A_TYPE\"", 1)) == EMGMResponse::UnsupportedType);
    BOOST_TEST(createResponse(dataModule("\"ARRAY\", 2, \"NOT_A_TYPE\"", 1)) == EMGMResponse::UnsupportedType);
    BOOST_TEST(createResponse(dataModule("\"ARRAY [4..1] OF INT\"", 1)) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(dataModule("\"ARRAY [0..x] OF INT\"", 1)) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(dataModule("\"ARRAY\", 0, \"INT\"", 1)) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(dataModule("\"ARRAY\", 2", 1)) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(dataModule("\"INT\", \"INT\"", 1)) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(dataModule("\"In valid\"", 1)) == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(plugAndSocketViews) {
    const StringId typeName = "luaadaptertest::Views"_STRID;
    auto entry = registerCounterAdapter(typeName);
    {
      CAdapterHost plugHost("PlugHost"_STRID, CFBTestDataGlobalFixture::getResource(), typeName, true);
      CAdapterHost socketHost("SocketHost"_STRID, CFBTestDataGlobalFixture::getResource(), typeName, false);
      BOOST_TEST(entry->getInstanceCount() == 2U);

      CAdapter *const plug = plugHost.getAdapterBlock();
      CAdapter *const socket = socketHost.getAdapterBlock();
      BOOST_REQUIRE(plug != nullptr);
      BOOST_REQUIRE(socket != nullptr);
      BOOST_TEST(plug->getFBTypeId() == typeName);
      BOOST_TEST(plug->getInstanceNameId() == "P"_STRID);
      BOOST_TEST(names(plug->getFBInterfaceSpec().mEINames) == (std::vector{"CNF"_STRID}));
      BOOST_TEST(names(socket->getFBInterfaceSpec().mEINames) == (std::vector{"REQ"_STRID, "R"_STRID}));
      BOOST_TEST(plug->getDI(1)->getDataTypeID() == CIEC_ANY::e_INT);
      BOOST_TEST(plug->getDO(0)->getDataTypeID() == CIEC_ANY::e_INT);
      BOOST_TEST(socket->getDO(0)->getDataTypeID() == CIEC_ANY::e_BOOL);
      BOOST_TEST(plug->getDI(2) == nullptr);
      BOOST_TEST(socket->getDI(1) == nullptr);

      // the adapter block is reachable as a child of its host
      BOOST_TEST(plugHost.getChildren().size() == 1U);
    }
    BOOST_TEST(entry->getInstanceCount() == 0U);
  }

  BOOST_AUTO_TEST_CASE(eventsAndDataCrossTheConnection) {
    const StringId typeName = "luaadaptertest::Connected"_STRID;
    registerCounterAdapter(typeName);
    SConnectedHosts hosts(typeName);
    CAdapter &plug = *hosts.mPlugHost.getAdapterBlock();
    CAdapter &socket = *hosts.mSocketHost.getAdapterBlock();
    BOOST_TEST(plug.getPeer() == &socket);
    BOOST_TEST(socket.getPeer() == &plug);

    // socket side sends REQ with PV, the plug side receives adapter event REQ of list ID 0
    static_cast<CIEC_INT &>(*socket.getDI(0)) = CIEC_INT(17);
    hosts.mSocketHost.setForwardedEvent(0);
    hosts.mSocketHost.forward();
    BOOST_TEST(hosts.mPlugHost.takeReceivedEvents() == (std::vector<TEventID>{256}));
    BOOST_TEST(intValue(*plug.getDO(0)) == 17);

    // R carries no data
    static_cast<CIEC_INT &>(*socket.getDI(0)) = CIEC_INT(99);
    hosts.mSocketHost.setForwardedEvent(1);
    hosts.mSocketHost.forward();
    BOOST_TEST(hosts.mPlugHost.takeReceivedEvents() == (std::vector<TEventID>{257}));
    BOOST_TEST(intValue(*plug.getDO(0)) == 17);

    // plug side answers CNF with Q and CV
    static_cast<CIEC_BOOL &>(*plug.getDI(0)) = CIEC_BOOL(true);
    static_cast<CIEC_INT &>(*plug.getDI(1)) = CIEC_INT(-5);
    hosts.mPlugHost.setForwardedEvent(0);
    hosts.mPlugHost.forward();
    BOOST_TEST(hosts.mSocketHost.takeReceivedEvents() == (std::vector<TEventID>{256}));
    BOOST_TEST(static_cast<bool>(static_cast<CIEC_BOOL &>(*socket.getDO(0))));
    BOOST_TEST(intValue(*socket.getDO(1)) == -5);
  }

  BOOST_AUTO_TEST_CASE(resetRestoresDefaults) {
    const StringId typeName = "luaadaptertest::Reset"_STRID;
    registerCounterAdapter(typeName);
    CAdapterHost host("Host"_STRID, CFBTestDataGlobalFixture::getResource(), typeName, false);
    BOOST_REQUIRE(host.initialize());
    BOOST_REQUIRE(host.changeExecutionState(EMGMCommandType::Start) == EMGMResponse::Ready);
    CAdapter &socket = *host.getAdapterBlock();
    static_cast<CIEC_INT &>(*socket.getDI(0)) = CIEC_INT(3);
    static_cast<CIEC_INT &>(*socket.getDO(1)) = CIEC_INT(4);
    BOOST_REQUIRE(host.changeExecutionState(EMGMCommandType::Stop) == EMGMResponse::Ready);
    BOOST_REQUIRE(host.changeExecutionState(EMGMCommandType::Reset) == EMGMResponse::Ready);
    BOOST_TEST(intValue(*socket.getDI(0)) == 0);
    BOOST_TEST(intValue(*socket.getDO(1)) == 0);
  }

  BOOST_AUTO_TEST_CASE(initialValuesFollowTheView) {
    const StringId typeName = "luaadaptertest::Initial"_STRID;
    std::shared_ptr<CLuaAdapterTypeEntry> entry;
    BOOST_REQUIRE(
        create(typeName,
               adapterModule(cEmptyEvents + "numDIs = 1, DINames = {\"PV\"}, DIDataTypeNames = {\"INT\"}, "
                                            "DIInitialValues = {\"7\"}, numDOs = 2, DONames = {\"Q\", \"CV\"}, "
                                            "DODataTypeNames = {\"BOOL\", \"INT\"}, "
                                            "DOInitialValues = {\"\", \"-1\"}"),
               entry) == EMGMResponse::Ready);
    BOOST_REQUIRE(registerRuntimeAdapterType(entry) == EMGMResponse::Ready);
    CAdapterHost plugHost("PlugHost"_STRID, CFBTestDataGlobalFixture::getResource(), typeName, true);
    CAdapterHost socketHost("SocketHost"_STRID, CFBTestDataGlobalFixture::getResource(), typeName, false);
    BOOST_REQUIRE(socketHost.initialize());
    BOOST_REQUIRE(socketHost.changeExecutionState(EMGMCommandType::Start) == EMGMResponse::Ready);
    CAdapter &plug = *plugHost.getAdapterBlock();
    CAdapter &socket = *socketHost.getAdapterBlock();
    BOOST_TEST(intValue(*socket.getDI(0)) == 7);
    BOOST_TEST(intValue(*socket.getDO(1)) == -1);
    BOOST_TEST(intValue(*plug.getDO(0)) == 7);
    BOOST_TEST(intValue(*plug.getDI(1)) == -1);

    static_cast<CIEC_INT &>(*socket.getDI(0)) = CIEC_INT(3);
    static_cast<CIEC_INT &>(*socket.getDO(1)) = CIEC_INT(4);
    BOOST_REQUIRE(socketHost.changeExecutionState(EMGMCommandType::Stop) == EMGMResponse::Ready);
    BOOST_REQUIRE(socketHost.changeExecutionState(EMGMCommandType::Reset) == EMGMResponse::Ready);
    BOOST_TEST(intValue(*socket.getDI(0)) == 7);
    BOOST_TEST(intValue(*socket.getDO(1)) == -1);

    BOOST_TEST(createResponse(adapterModule(cEmptyEvents + "numDIs = 0, DINames = {}, DIDataTypeNames = {}, "
                                                           "numDOs = 0, DONames = {}, DODataTypeNames = {}, "
                                                           "numDIOs = 1, DIONames = {\"V\"}, "
                                                           "DIODataTypeNames = {\"INT\"}")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(adapterModule(cEmptyEvents + "numDIs = 1, DINames = {\"PV\"}, "
                                                           "DIDataTypeNames = {\"INT\"}, DIInitialValues = {\"TRUE\"}, "
                                                           "numDOs = 0, DONames = {}, DODataTypeNames = {}")) ==
               EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(pinsCheckTheirType) {
    const StringId first = "luaadaptertest::First"_STRID;
    const StringId second = "luaadaptertest::Second"_STRID;
    registerCounterAdapter(first);
    registerCounterAdapter(second);
    CAdapterHost plugHost("PlugHost"_STRID, CFBTestDataGlobalFixture::getResource(), first, true);
    CAdapterHost socketHost("SocketHost"_STRID, CFBTestDataGlobalFixture::getResource(), second, false);
    BOOST_TEST(plugHost.getPlug().getAdapterCon().connect(socketHost, std::array{"S"_STRID}) ==
               EMGMResponse::InvalidObject);

    CAdapterHost unknown("Unknown"_STRID, CFBTestDataGlobalFixture::getResource(), "luaadaptertest::Missing"_STRID,
                         true);
    BOOST_TEST(unknown.getAdapterBlock() == nullptr);
    BOOST_TEST(unknown.getPlug().getAdapterTypeId() == "luaadaptertest::Missing"_STRID);
    BOOST_TEST(unknown.getChildren().empty());
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::lua::test
