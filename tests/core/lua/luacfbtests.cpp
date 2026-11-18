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

#include "../../../core/src/lua/luaadapter.h"
#include "../../../core/src/lua/luabfb.h"
#include "../../../core/src/lua/luacfb.h"
#include "../../../core/src/lua/luaengine.h"

#include "forte/ecet.h"
#include "forte/resource.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/inoutdataconn.h"

#include "forte_boost_output_support.h"
#include "../fbtests/fbtestfixture.h"
#include "../fbtests/fbtesterglobalfixture.h"

using namespace forte::literals;
using forte::test::CFBTestDataGlobalFixture;
using forte::test::CFBTestFixtureBase;

namespace forte::lua::test {

  namespace {
    //! One state reads every module, as the device's definition state does
    CLuaEngine &definitionEngine() {
      static CLuaEngine engine(CLuaEngine::scmDefinitionInstructionLimit);
      return engine;
    }

    // INIT sets STEP = 1 and clears CV, REQ adds STEP to CV while CV < PV
    constexpr char cCounter[] = R"lua(
local DI_PV = 33554432
local DO_Q = 67108864
local DO_CV = 67108865
local IN_STEP = 268435456
return {
  ECC = function(fb, id)
    if id == 0 then
      fb[IN_STEP] = 1
      fb[DO_CV] = 0
      fb[DO_Q] = false
      fb(0)
    elseif id == 1 and fb[DO_CV] < fb[DI_PV] then
      fb[DO_CV] = fb[DO_CV] + fb[IN_STEP]
      fb[DO_Q] = fb[DO_CV] >= fb[DI_PV]
      fb(1)
    end
  end,
  interfaceSpec = {
    numEIs = 2, EINames = {"INIT", "REQ"}, EIWith = {0, 65535, 0, 65535}, EIWithIndexes = {0, 2},
    numEOs = 2, EONames = {"INITO", "CNF"}, EOWith = {1, 65535, 0, 1, 65535}, EOWithIndexes = {0, 2},
    numDIs = 1, DINames = {"PV"}, DIDataTypeNames = {"INT"},
    numDOs = 2, DONames = {"Q", "CV"}, DODataTypeNames = {"BOOL", "INT"},
    numAdapters = 0, adapterInstanceDefinition = {}
  },
  internalVarsInformation = {numIntVars = 1, intVarsNames = {"STEP"}, intVarsDataTypeNames = {"INT"}}
}
)lua";

    // CounterPair from the IDE examples (docs/ide/forte-lua-type-deployment/examples/03-composite)
    constexpr char cCounterPair[] = R"lua(
local EI_INIT = 0
local EI_REQ = 1
local EO_CNF = 0
local DI_PV = 33554432
local DO_CV = 67108864

local interfaceSpec = {
  numEIs = 2,
  EINames = {"INIT", "REQ"},
  EIWith = {0, 65535, 0, 65535},
  EIWithIndexes = {0, 2},
  numEOs = 1,
  EONames = {"CNF"},
  EOWith = {0, 65535},
  EOWithIndexes = {0},
  numDIs = 1,
  DINames = {"PV"},
  DIDataTypeNames = {"INT"},
  numDOs = 1,
  DONames = {"CV"},
  DODataTypeNames = {"INT"},
  numAdapters = 0,
  adapterInstanceDefinition = {
  }
}

local fbnSpec = {
  internalFBs = {
    {fbNameID = "C1", fbTypeID = "luacfbtest::Counter"},
    {fbNameID = "C2", fbTypeID = "luacfbtest::Counter"}
  },
  parameters = {
    {fbNum = 1, diNameID = "PV", paramValue = "5"}
  },
  eventConnections = {
    {srcID = "INIT", srcFBNum = -1, dstID = "INIT", dstFBNum = 0},
    {srcID = "REQ", srcFBNum = -1, dstID = "REQ", dstFBNum = 0},
    {srcID = "CNF", srcFBNum = 0, dstID = "REQ", dstFBNum = 1},
    {srcID = "CNF", srcFBNum = 1, dstID = "CNF", dstFBNum = -1}
  },
  fannedOutEventConnections = {
    {connectionNum = 0, dstID = "INIT", dstFBNum = 1}
  },
  dataConnections = {
    {srcID = "PV", srcFBNum = -1, dstID = "PV", dstFBNum = 0},
    {srcID = "CV", srcFBNum = 1, dstID = "CV", dstFBNum = -1}
  },
  fannedOutDataConnections = {
  },
  adapterConnections = {
  },
  numFBs = 2,
  numECons = 4,
  numFECons = 1,
  numDCons = 2,
  numFDCons = 0,
  numAdpCons = 0,
  numParams = 1
}

return {interfaceSpec = interfaceSpec, fbnSpec = fbnSpec}
)lua";

    // a composite in a composite next to a compiled E_CTU, which counts the CNFs of the pair
    constexpr char cNested[] = R"lua(
return {
  interfaceSpec = {
    numEIs = 2, EINames = {"INIT", "REQ"}, EIWith = {0, 65535}, EIWithIndexes = {0, 0},
    numEOs = 1, EONames = {"CNF"}, EOWith = {0, 1, 65535}, EOWithIndexes = {0},
    numDIs = 1, DINames = {"PV"}, DIDataTypeNames = {"INT"},
    numDOs = 2, DONames = {"Q", "CV"}, DODataTypeNames = {"BOOL", "UINT"},
    numAdapters = 0, adapterInstanceDefinition = {}
  },
  fbnSpec = {
    internalFBs = {
      {fbNameID = "PAIR", fbTypeID = "luacfbtest::CounterPair"},
      {fbNameID = "CTU", fbTypeID = "iec61499::events::E_CTU"}
    },
    parameters = {{fbNum = 1, diNameID = "PV", paramValue = "2"}},
    eventConnections = {
      {srcID = "INIT", srcFBNum = -1, dstID = "INIT", dstFBNum = 0},
      {srcID = "REQ", srcFBNum = -1, dstID = "REQ", dstFBNum = 0},
      {srcID = "CNF", srcFBNum = 0, dstID = "CU", dstFBNum = 1},
      {srcID = "CUO", srcFBNum = 1, dstID = "CNF", dstFBNum = -1}
    },
    dataConnections = {
      {srcID = "PV", srcFBNum = -1, dstID = "PV", dstFBNum = 0},
      {srcID = "Q", srcFBNum = 1, dstID = "Q", dstFBNum = -1},
      {srcID = "CV", srcFBNum = 1, dstID = "CV", dstFBNum = -1}
    },
    adapterConnections = {},
    numFBs = 2, numECons = 4, numDCons = 3, numAdpCons = 0, numParams = 1
  }
}
)lua";

    // no internal FBs: REQ passes IN to OUT and fires CNF
    constexpr char cPassThrough[] = R"lua(
return {
  interfaceSpec = {
    numEIs = 1, EINames = {"REQ"}, EIWith = {0, 65535}, EIWithIndexes = {0},
    numEOs = 1, EONames = {"CNF"}, EOWith = {0, 65535}, EOWithIndexes = {0},
    numDIs = 1, DINames = {"IN"}, DIDataTypeNames = {"INT"},
    numDOs = 1, DONames = {"OUT"}, DODataTypeNames = {"INT"},
    numAdapters = 0, adapterInstanceDefinition = {}
  },
  fbnSpec = {
    internalFBs = {}, parameters = {},
    eventConnections = {{srcID = "REQ", srcFBNum = -1, dstID = "CNF", dstFBNum = -1}},
    dataConnections = {{srcID = "IN", srcFBNum = -1, dstID = "OUT", dstFBNum = -1}},
    adapterConnections = {},
    numFBs = 0, numECons = 1, numDCons = 1, numAdpCons = 0, numParams = 0
  }
}
)lua";

    // PassThrough with initial values, FIX is not connected inside
    constexpr char cInitialPassThrough[] = R"lua(
return {
  interfaceSpec = {
    numEIs = 1, EINames = {"REQ"}, EIWith = {0, 65535}, EIWithIndexes = {0},
    numEOs = 1, EONames = {"CNF"}, EOWith = {0, 65535}, EOWithIndexes = {0},
    numDIs = 1, DINames = {"IN"}, DIDataTypeNames = {"INT"}, DIInitialValues = {"12"},
    numDOs = 2, DONames = {"OUT", "FIX"}, DODataTypeNames = {"INT", "INT"}, DOInitialValues = {"", "-3"},
    numAdapters = 0, adapterInstanceDefinition = {}
  },
  fbnSpec = {
    internalFBs = {}, parameters = {},
    eventConnections = {{srcID = "REQ", srcFBNum = -1, dstID = "CNF", dstFBNum = -1}},
    dataConnections = {{srcID = "IN", srcFBNum = -1, dstID = "OUT", dstFBNum = -1}},
    adapterConnections = {},
    numFBs = 0, numECons = 1, numDCons = 1, numAdpCons = 0, numParams = 0
  }
}
)lua";

    constexpr char cCounterAdapter[] = R"lua(
return {interfaceSpec = {
  numEIs = 2, EINames = {"REQ", "R"}, EIWith = {0, 65535}, EIWithIndexes = {0, -1},
  numEOs = 1, EONames = {"CNF"}, EOWith = {0, 1, 65535}, EOWithIndexes = {0},
  numDIs = 1, DINames = {"PV"}, DIDataTypeNames = {"INT"},
  numDOs = 2, DONames = {"Q", "CV"}, DODataTypeNames = {"BOOL", "INT"}
}}
)lua";

    // plug side of ACounter: answers REQ with CNF, CV = 2 * PV
    constexpr char cDoubler[] = R"lua(
local AEI_P_REQ = 134217728
local AEO_P_CNF = 134217728
local ADO_P_Q = 167772160
local ADO_P_CV = 167772161
local ADI_P_PV = 201326592
return {
  ECC = function(fb, id)
    if id == AEI_P_REQ then
      fb[ADO_P_CV] = fb[ADI_P_PV] * 2
      fb[ADO_P_Q] = true
      fb(AEO_P_CNF)
    end
  end,
  interfaceSpec = {
    numEIs = 0, EINames = {}, EIWith = {}, EIWithIndexes = {},
    numEOs = 0, EONames = {}, EOWith = {}, EOWithIndexes = {},
    numDIs = 0, DINames = {}, DIDataTypeNames = {},
    numDOs = 0, DONames = {}, DODataTypeNames = {},
    numAdapters = 1,
    adapterInstanceDefinition = {
      {adapterNameID = "P", adapterTypeNameID = "luacfbtest::ACounter", isPlug = true}
    }
  }
}
)lua";

    // a composite with its own socket, wired inside the network through the adapter FB S
    constexpr char cAdapterHost[] = R"lua(
return {
  interfaceSpec = {
    numEIs = 1, EINames = {"GO"}, EIWith = {0, 65535}, EIWithIndexes = {0},
    numEOs = 1, EONames = {"DONE"}, EOWith = {0, 65535}, EOWithIndexes = {0},
    numDIs = 1, DINames = {"X"}, DIDataTypeNames = {"INT"},
    numDOs = 1, DONames = {"RESULT"}, DODataTypeNames = {"INT"},
    numAdapters = 1,
    adapterInstanceDefinition = {
      {adapterNameID = "S", adapterTypeNameID = "luacfbtest::ACounter", isPlug = false}
    }
  },
  fbnSpec = {
    internalFBs = {}, parameters = {},
    eventConnections = {
      {srcID = "GO", srcFBNum = -1, dstID = "REQ", dstFBNum = 65536},
      {srcID = "CNF", srcFBNum = 65536, dstID = "DONE", dstFBNum = -1}
    },
    dataConnections = {
      {srcID = "X", srcFBNum = -1, dstID = "PV", dstFBNum = 65536},
      {srcID = "CV", srcFBNum = 65536, dstID = "RESULT", dstFBNum = -1}
    },
    adapterConnections = {},
    numFBs = 0, numECons = 2, numDCons = 2, numAdpCons = 0, numParams = 0
  }
}
)lua";

    // REQ adds 1 to the in-out V, which is read with REQ and written back with CNF
    constexpr char cInOutInc[] = R"lua(
local DIO_V = 100663296
return {
  ECC = function(fb, id)
    if id == 0 then
      fb[DIO_V] = fb[DIO_V] + 1
      fb(0)
    end
  end,
  interfaceSpec = {
    numEIs = 1, EINames = {"REQ"}, EIWith = {0, 65535}, EIWithIndexes = {0},
    numEOs = 1, EONames = {"CNF"}, EOWith = {0, 65535}, EOWithIndexes = {0},
    numDIs = 0, DINames = {}, DIDataTypeNames = {},
    numDOs = 0, DONames = {}, DODataTypeNames = {},
    numDIOs = 1, DIONames = {"V"}, DIODataTypeNames = {"INT"}, DIOInitialValues = {"10"},
    numAdapters = 0, adapterInstanceDefinition = {}
  }
}
)lua";

    // the in-out V is passed to the in-out of the internal InOutInc, read with REQ and written back with CNF
    constexpr char cInOutWrap[] = R"lua(
return {
  interfaceSpec = {
    numEIs = 1, EINames = {"REQ"}, EIWith = {0, 65535}, EIWithIndexes = {0},
    numEOs = 1, EONames = {"CNF"}, EOWith = {0, 65535}, EOWithIndexes = {0},
    numDIs = 0, DINames = {}, DIDataTypeNames = {},
    numDOs = 0, DONames = {}, DODataTypeNames = {},
    numDIOs = 1, DIONames = {"V"}, DIODataTypeNames = {"INT"}, DIOInitialValues = {"3"},
    numAdapters = 0, adapterInstanceDefinition = {}
  },
  fbnSpec = {
    internalFBs = {{fbNameID = "INC", fbTypeID = "luacfbtest::InOutInc"}}, parameters = {},
    eventConnections = {
      {srcID = "REQ", srcFBNum = -1, dstID = "REQ", dstFBNum = 0},
      {srcID = "CNF", srcFBNum = 0, dstID = "CNF", dstFBNum = -1}
    },
    dataConnections = {{srcID = "V", srcFBNum = -1, dstID = "V", dstFBNum = 0}},
    adapterConnections = {},
    numFBs = 1, numECons = 2, numDCons = 1, numAdpCons = 0, numParams = 0
  }
}
)lua";

    const std::string cEmptyInterface =
        "interfaceSpec = {numEIs = 1, EINames = {\"EI\"}, EIWith = {0, 65535}, EIWithIndexes = {0}, "
        "numEOs = 1, EONames = {\"EO\"}, EOWith = {}, EOWithIndexes = {-1}, numDIs = 1, DINames = {\"DI\"}, "
        "DIDataTypeNames = {\"INT\"}, numDOs = 1, DONames = {\"DO\"}, DODataTypeNames = {\"INT\"}, "
        "numAdapters = 2, adapterInstanceDefinition = {"
        "{adapterNameID = \"P\", adapterTypeNameID = \"luacfbtest::ACounter\", isPlug = true}, "
        "{adapterNameID = \"S\", adapterTypeNameID = \"luacfbtest::ACounter\", isPlug = false}}}";

    //! A module with cEmptyInterface and a network of two counters A and B with the given parts
    std::string network(const std::string &paEvents,
                        std::size_t paNumEvents,
                        const std::string &paData = "",
                        std::size_t paNumData = 0,
                        const std::string &paExtra = "") {
      return "return {" + cEmptyInterface +
             ", fbnSpec = {internalFBs = {{fbNameID = \"A\", fbTypeID = \"luacfbtest::Counter\"}, "
             "{fbNameID = \"B\", fbTypeID = \"luacfbtest::Counter\"}}, parameters = {}, eventConnections = {" +
             paEvents + "}, dataConnections = {" + paData +
             "}, adapterConnections = {}, numFBs = 2, numECons = " + std::to_string(paNumEvents) +
             ", numDCons = " + std::to_string(paNumData) + ", numAdpCons = 0, numParams = 0" + paExtra + "}}";
    }

    EMGMResponse createCFB(StringId paName, const std::string &paSource, std::shared_ptr<CLuaCFBTypeEntry> &paEntry) {
      std::string error;
      const EMGMResponse response = CLuaCFBTypeEntry::create(definitionEngine(), paName, paSource, paEntry, error);
      BOOST_TEST_MESSAGE("create " << paName.data() << ": " << error);
      return response;
    }

    EMGMResponse createResponse(const std::string &paSource) {
      std::shared_ptr<CLuaCFBTypeEntry> entry;
      return createCFB("luacfbtest::Invalid"_STRID, paSource, entry);
    }

    std::shared_ptr<const CRuntimeFBTypeEntry> registerCFB(StringId paName, const std::string &paSource) {
      std::shared_ptr<CLuaCFBTypeEntry> entry;
      BOOST_REQUIRE(createCFB(paName, paSource, entry) == EMGMResponse::Ready);
      BOOST_REQUIRE(registerRuntimeFBType(entry) == EMGMResponse::Ready);
      return getRuntimeFBTypeEntry(paName);
    }

    void registerBFB(StringId paName, const std::string &paSource) {
      std::shared_ptr<CLuaBFBTypeEntry> entry;
      std::string error;
      BOOST_REQUIRE(CLuaBFBTypeEntry::create(definitionEngine(), paName, paSource, entry, error) ==
                    EMGMResponse::Ready);
      BOOST_REQUIRE(registerRuntimeFBType(entry) == EMGMResponse::Ready);
    }

    void registerCounterAdapter() {
      std::shared_ptr<CLuaAdapterTypeEntry> entry;
      std::string error;
      BOOST_REQUIRE(CLuaAdapterTypeEntry::create(definitionEngine(), "luacfbtest::ACounter"_STRID, cCounterAdapter,
                                                 entry, error) == EMGMResponse::Ready);
      BOOST_REQUIRE(registerRuntimeAdapterType(entry) == EMGMResponse::Ready);
    }

    void registerCounterTypes() {
      registerBFB("luacfbtest::Counter"_STRID, cCounter);
      registerCFB("luacfbtest::CounterPair"_STRID, cCounterPair);
    }

    void registerInOutTypes() {
      registerBFB("luacfbtest::InOutInc"_STRID, cInOutInc);
      registerCFB("luacfbtest::InOutWrap"_STRID, cInOutWrap);
    }

    void triggerAndWait(CFunctionBlock &paFB, TEventID paEvent) {
      CEventChainExecutionThread *const thread = CFBTestDataGlobalFixture::getResource().getResourceEventExecution();
      thread->startEventChain(TEventEntry(paFB, paEvent));
      do {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
      } while (thread->isProcessingEvents());
    }

    TForteInt16 intValue(const CIEC_ANY *paValue) {
      return static_cast<TForteInt16>(static_cast<const CIEC_INT &>(*paValue));
    }

    CFunctionBlock *createInstance(StringId paName, StringId paType) {
      return forte::createFB(paName, paType, CFBTestDataGlobalFixture::getResource());
    }

    //! An instance of a run-time type in the test resource, deleted at the end of the scope
    struct SInstance {
        SInstance(StringId paName, StringId paType) : mFB(createInstance(paName, paType)) {
          BOOST_REQUIRE(mFB != nullptr);
          BOOST_REQUIRE(mFB->changeExecutionState(EMGMCommandType::Start) == EMGMResponse::Ready);
        }

        ~SInstance() {
          forte::deleteFB(mFB);
        }

        CFunctionBlock *operator->() {
          return mFB;
        }

        CFunctionBlock *mFB;
    };

    struct SCounterPairTypes {
        SCounterPairTypes() {
          registerCounterTypes();
        }
    };

    class CLuaCounterPairFixture : private SCounterPairTypes, public CFBTestFixtureBase {
      public:
        CLuaCounterPairFixture() : CFBTestFixtureBase("luacfbtest::CounterPair"_STRID) {
          setInputData({&mPV});
          setOutputData({&mCV});
          setup();
        }

      protected:
        CIEC_INT mPV;
        CIEC_INT mCV;
    };

    struct SNestedTypes {
        SNestedTypes() {
          registerCounterTypes();
          registerCFB("luacfbtest::Nested"_STRID, cNested);
        }
    };

    class CLuaNestedFixture : private SNestedTypes, public CFBTestFixtureBase {
      public:
        CLuaNestedFixture() : CFBTestFixtureBase("luacfbtest::Nested"_STRID) {
          setInputData({&mPV});
          setOutputData({&mQ, &mCV});
          setup();
        }

      protected:
        CIEC_INT mPV;
        CIEC_BOOL mQ;
        CIEC_UINT mCV;
    };

    struct SPassThroughType {
        SPassThroughType() {
          registerCFB("luacfbtest::PassThrough"_STRID, cPassThrough);
        }
    };

    class CLuaPassThroughFixture : private SPassThroughType, public CFBTestFixtureBase {
      public:
        CLuaPassThroughFixture() : CFBTestFixtureBase("luacfbtest::PassThrough"_STRID) {
          setInputData({&mIn});
          setOutputData({&mOut});
          setup();
        }

      protected:
        CIEC_INT mIn;
        CIEC_INT mOut;
    };
  } // namespace

  BOOST_FIXTURE_TEST_SUITE(LuaCFBCounterPairTests, CLuaCounterPairFixture)

  BOOST_AUTO_TEST_CASE(countsThroughBothCounters) {
    // INIT reaches both counters through the fanned out connection, INITO is not connected
    triggerEvent(0);
    BOOST_TEST(eventChainEmpty());

    mPV = CIEC_INT(2);
    triggerEvent(1);
    BOOST_TEST(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(static_cast<TForteInt16>(mCV) == 1);

    triggerEvent(1);
    BOOST_TEST(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(static_cast<TForteInt16>(mCV) == 2);

    // the first counter reached PV, the second one with the parameter PV = 5 gets no more REQ
    triggerEvent(1);
    BOOST_TEST(eventChainEmpty());
  }

  BOOST_AUTO_TEST_SUITE_END()

  BOOST_FIXTURE_TEST_SUITE(LuaCFBNestedTests, CLuaNestedFixture)

  BOOST_AUTO_TEST_CASE(compositeInCompositeWithCompiledFB) {
    triggerEvent(0);
    BOOST_TEST(eventChainEmpty());

    mPV = CIEC_INT(10);
    triggerEvent(1);
    BOOST_TEST(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(static_cast<TForteUInt16>(mCV) == 1);
    BOOST_TEST(!static_cast<bool>(mQ));

    triggerEvent(1);
    BOOST_TEST(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(static_cast<TForteUInt16>(mCV) == 2);
    BOOST_TEST(static_cast<bool>(mQ));
  }

  BOOST_AUTO_TEST_SUITE_END()

  BOOST_FIXTURE_TEST_SUITE(LuaCFBPassThroughTests, CLuaPassThroughFixture)

  BOOST_AUTO_TEST_CASE(interfaceToInterface) {
    mIn = CIEC_INT(7);
    triggerEvent(0);
    BOOST_TEST(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(static_cast<TForteInt16>(mOut) == 7);
  }

  BOOST_AUTO_TEST_SUITE_END()

  BOOST_AUTO_TEST_SUITE(LuaCFBTests)

  BOOST_AUTO_TEST_CASE(parametersAfterReset) {
    registerCounterTypes();
    SInstance pair("ResetPair"_STRID, "luacfbtest::CounterPair"_STRID);
    CIEC_ANY *const pv = pair->getVar(std::array{"C2"_STRID, "PV"_STRID});
    BOOST_REQUIRE(pv != nullptr);
    BOOST_TEST(intValue(pv) == 5);

    static_cast<CIEC_INT &>(*pv) = CIEC_INT(9);
    BOOST_TEST(pair->changeExecutionState(EMGMCommandType::Stop) == EMGMResponse::Ready);
    BOOST_TEST(pair->changeExecutionState(EMGMCommandType::Reset) == EMGMResponse::Ready);
    BOOST_TEST(intValue(pv) == 5);
    BOOST_TEST(pair->changeExecutionState(EMGMCommandType::Start) == EMGMResponse::Ready);
  }

  BOOST_AUTO_TEST_CASE(initialValues) {
    registerCFB("luacfbtest::InitialPassThrough"_STRID, cInitialPassThrough);
    SInstance fb("InitialPassThrough"_STRID, "luacfbtest::InitialPassThrough"_STRID);
    BOOST_TEST(intValue(fb->getDI(0)) == 12);
    BOOST_TEST(intValue(fb->getDO(1)) == -3);
    triggerAndWait(*fb.mFB, 0);
    BOOST_TEST(intValue(fb->getDO(0)) == 12);

    static_cast<CIEC_INT &>(*fb->getDI(0)) = CIEC_INT(1);
    static_cast<CIEC_INT &>(*fb->getDO(1)) = CIEC_INT(2);
    BOOST_TEST(fb->changeExecutionState(EMGMCommandType::Stop) == EMGMResponse::Ready);
    BOOST_TEST(fb->changeExecutionState(EMGMCommandType::Reset) == EMGMResponse::Ready);
    BOOST_TEST(intValue(fb->getDI(0)) == 12);
    BOOST_TEST(intValue(fb->getDO(1)) == -3);
    BOOST_TEST(fb->changeExecutionState(EMGMCommandType::Start) == EMGMResponse::Ready);
  }

  BOOST_AUTO_TEST_CASE(instanceCount) {
    registerCounterTypes();
    const auto entry = getRuntimeFBTypeEntry("luacfbtest::CounterPair"_STRID);
    const auto counterEntry = getRuntimeFBTypeEntry("luacfbtest::Counter"_STRID);
    {
      SInstance pair("CountedPair"_STRID, "luacfbtest::CounterPair"_STRID);
      BOOST_TEST(entry->getInstanceCount() == 1U);
      BOOST_TEST(counterEntry->getInstanceCount() == 2U);
    }
    BOOST_TEST(entry->getInstanceCount() == 0U);
    BOOST_TEST(counterEntry->getInstanceCount() == 0U);
  }

  BOOST_AUTO_TEST_CASE(ownSocketInTheNetwork) {
    registerCounterAdapter();
    registerBFB("luacfbtest::Doubler"_STRID, cDoubler);
    registerCFB("luacfbtest::AdapterHost"_STRID, cAdapterHost);
    SInstance server("HostServer"_STRID, "luacfbtest::Doubler"_STRID);
    SInstance host("Host"_STRID, "luacfbtest::AdapterHost"_STRID);
    BOOST_TEST(host->getSocketPin("S"_STRID)->getAdapterTypeId() == "luacfbtest::ACounter"_STRID);
    BOOST_REQUIRE(server->getPlugPin("P"_STRID)->getAdapterCon().connect(*host.mFB, std::array{"S"_STRID}) ==
                  EMGMResponse::Ready);

    static_cast<CIEC_INT &>(*host->getDI(0)) = CIEC_INT(21);
    triggerAndWait(*host.mFB, 0);
    BOOST_TEST(intValue(host->getDO(0)) == 42);

    BOOST_TEST(server->getPlugPin("P"_STRID)->getAdapterCon().disconnect(*host.mFB, std::array{"S"_STRID}) ==
               EMGMResponse::Ready);
  }

  BOOST_AUTO_TEST_CASE(inOutVariables) {
    registerInOutTypes();
    SInstance wrap("Wrap"_STRID, "luacfbtest::InOutWrap"_STRID);
    BOOST_TEST(wrap->getFBInterfaceSpec().getNumDIOs() == 1U);
    BOOST_TEST(intValue(wrap->getDIO(0)) == 3);
    BOOST_TEST(wrap->getDIO(1) == nullptr);
    CIEC_ANY *const innerV = wrap->getVar(std::array{"INC"_STRID, "V"_STRID});
    BOOST_REQUIRE(innerV != nullptr);
    BOOST_TEST(intValue(innerV) == 10);

    // unconnected, the internal FB works on the composite's own value
    triggerAndWait(*wrap.mFB, 0);
    BOOST_TEST(intValue(wrap->getDIO(0)) == 4);
    BOOST_TEST(intValue(wrap->getDataInOut("V"_STRID)) == 4);
    BOOST_TEST(intValue(innerV) == 4);
    triggerAndWait(*wrap.mFB, 0);
    BOOST_TEST(intValue(wrap->getDIO(0)) == 5);
  }

  BOOST_AUTO_TEST_CASE(inOutChain) {
    registerInOutTypes();
    SInstance source("Source"_STRID, "luacfbtest::InOutInc"_STRID);
    SInstance first("First"_STRID, "luacfbtest::InOutWrap"_STRID);
    SInstance second("Second"_STRID, "luacfbtest::InOutWrap"_STRID);
    CInOutDataConnection *const sourceOut = source->getDIOOutConnection("V"_STRID);
    CInOutDataConnection *const firstOut = first->getDIOOutConnection("V"_STRID);
    BOOST_REQUIRE(sourceOut != nullptr);
    BOOST_REQUIRE(firstOut != nullptr);

    // Source.V -> First.V -> Second.V, the second link made first so the first one has to pass its value on
    BOOST_REQUIRE(firstOut->connect(*second.mFB, std::array{"V"_STRID}) == EMGMResponse::Ready);
    BOOST_REQUIRE(sourceOut->connect(*first.mFB, std::array{"V"_STRID}) == EMGMResponse::Ready);
    BOOST_TEST(first->getDIOInConnection("V"_STRID) == sourceOut);
    BOOST_TEST(second->getDIOInConnection("V"_STRID) == firstOut);

    // each composite takes the source's value with REQ and writes the result back with CNF
    triggerAndWait(*second.mFB, 0);
    BOOST_TEST(intValue(second->getDIO(0)) == 11);
    BOOST_TEST(intValue(source->getDIO(0)) == 11);
    BOOST_TEST(intValue(first->getDIO(0)) == 3);
    triggerAndWait(*first.mFB, 0);
    BOOST_TEST(intValue(first->getDIO(0)) == 12);
    BOOST_TEST(intValue(source->getDIO(0)) == 12);
    triggerAndWait(*source.mFB, 0);
    BOOST_TEST(intValue(source->getDIO(0)) == 13);
    triggerAndWait(*second.mFB, 0);
    BOOST_TEST(intValue(second->getDIO(0)) == 14);
    BOOST_TEST(intValue(source->getDIO(0)) == 14);
    BOOST_TEST(intValue(first->getDIO(0)) == 12);

    BOOST_TEST(sourceOut->disconnect(*first.mFB, std::array{"V"_STRID}) == EMGMResponse::Ready);
    BOOST_TEST(firstOut->disconnect(*second.mFB, std::array{"V"_STRID}) == EMGMResponse::Ready);
    BOOST_TEST(first->getDIOInConnection("V"_STRID) == nullptr);
  }

  BOOST_AUTO_TEST_CASE(inOutReset) {
    registerInOutTypes();
    SInstance wrap("ResetWrap"_STRID, "luacfbtest::InOutWrap"_STRID);
    triggerAndWait(*wrap.mFB, 0);
    triggerAndWait(*wrap.mFB, 0);
    BOOST_TEST(intValue(wrap->getDIO(0)) == 5);

    BOOST_TEST(wrap->changeExecutionState(EMGMCommandType::Stop) == EMGMResponse::Ready);
    BOOST_TEST(wrap->changeExecutionState(EMGMCommandType::Reset) == EMGMResponse::Ready);
    BOOST_TEST(intValue(wrap->getDIO(0)) == 3);
    BOOST_TEST(intValue(wrap->getVar(std::array{"INC"_STRID, "V"_STRID})) == 10);
    BOOST_TEST(wrap->changeExecutionState(EMGMCommandType::Start) == EMGMResponse::Ready);

    triggerAndWait(*wrap.mFB, 0);
    BOOST_TEST(intValue(wrap->getDIO(0)) == 4);
  }

  BOOST_AUTO_TEST_CASE(createErrors) {
    registerCounterAdapter();
    const std::string conn = "{srcID = \"EI\", srcFBNum = -1, dstID = \"REQ\", dstFBNum = 0}";
    BOOST_TEST(createResponse(network(conn, 1)) == EMGMResponse::Ready);
    BOOST_TEST(createResponse(network(conn, 1).substr(0, 40)) == EMGMResponse::InvalidObject);
    BOOST_TEST(createResponse("return {" + cEmptyInterface + "}") == EMGMResponse::BadParams);
    BOOST_TEST(createResponse("return {" + cEmptyInterface + ", fbnSpec = 1}") == EMGMResponse::BadParams);
    // in-out variables: a data source of the network only, that never drives an output of the composite
    const auto withInOut = [](std::string paModule) {
      const std::size_t numAdapters = paModule.find("numAdapters");
      BOOST_REQUIRE(numAdapters != std::string::npos);
      paModule.insert(numAdapters, "numDIOs = 1, DIONames = {\"V\"}, DIODataTypeNames = {\"INT\"}, ");
      return paModule;
    };
    BOOST_TEST(createResponse(withInOut(network(conn, 1))) == EMGMResponse::Ready);
    BOOST_TEST(createResponse(withInOut(network(conn, 1, "{srcID = \"V\", srcFBNum = -1, dstID = \"PV\", dstFBNum = 0}",
                                                1))) == EMGMResponse::Ready);
    BOOST_TEST(createResponse(
                   withInOut(network(conn, 1, "{srcID = \"V\", srcFBNum = -1, dstID = \"DO\", dstFBNum = -1}", 1))) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withInOut(network(conn, 1, "{srcID = \"CV\", srcFBNum = 0, dstID = \"V\", dstFBNum = -1}",
                                                1))) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(network(conn, 1, "{srcID = \"V\", srcFBNum = -1, dstID = \"PV\", dstFBNum = 0}", 1)) ==
               EMGMResponse::BadParams);
    // counts
    BOOST_TEST(createResponse(network(conn, 2)) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(network(conn, 0)) == EMGMResponse::BadParams);
    // endpoints
    BOOST_TEST(createResponse(network("{srcID = \"NOPE\", srcFBNum = -1, dstID = \"REQ\", dstFBNum = 0}", 1)) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(network("{srcID = \"EO\", srcFBNum = -1, dstID = \"REQ\", dstFBNum = 0}", 1)) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(network("{srcID = \"EI\", srcFBNum = 2, dstID = \"REQ\", dstFBNum = 0}", 1)) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(network("{srcID = \"EI\", srcFBNum = -2, dstID = \"REQ\", dstFBNum = 0}", 1)) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(network("{srcID = \"EI\", srcFBNum = -1, dstID = \"REQ\", dstFBNum = 65538}", 1)) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(network("{srcID = \"EI\", srcFBNum = -1, dstID = \"A.REQ\", dstFBNum = 0}", 1)) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(network("{srcID = \"EI\", srcFBNum = -1, dstID = \"REQ\"}", 1)) ==
               EMGMResponse::BadParams);
    // own adapters: S is adapter 1 since plugs come first
    BOOST_TEST(createResponse(network("{srcID = \"CNF\", srcFBNum = 65537, dstID = \"REQ\", dstFBNum = 0}", 1)) ==
               EMGMResponse::Ready);
    // data
    BOOST_TEST(createResponse(network(conn, 1, "{srcID = \"DI\", srcFBNum = -1, dstID = \"PV\", dstFBNum = 0}", 1)) ==
               EMGMResponse::Ready);
    BOOST_TEST(createResponse(network(conn, 1, "{srcID = \"EI\", srcFBNum = -1, dstID = \"PV\", dstFBNum = 0}", 1)) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(network(conn, 1,
                                      "{srcID = \"CV\", srcFBNum = 0, dstID = \"DO\", dstFBNum = -1}, "
                                      "{srcID = \"CV\", srcFBNum = 1, dstID = \"DO\", dstFBNum = -1}",
                                      2)) == EMGMResponse::BadParams);
    // fanned out connections
    BOOST_TEST(createResponse(network(conn, 1, "", 0,
                                      ", fannedOutEventConnections = {{connectionNum = 0, dstID = \"REQ\", "
                                      "dstFBNum = 1}}, numFECons = 1")) == EMGMResponse::Ready);
    BOOST_TEST(createResponse(network(conn, 1, "", 0,
                                      ", fannedOutEventConnections = {{connectionNum = 1, dstID = \"REQ\", "
                                      "dstFBNum = 1}}, numFECons = 1")) == EMGMResponse::BadParams);
    // the form the IDE writes for an output of the composite (CFB-3)
    BOOST_TEST(createResponse(
                   network(conn, 1, "", 0, ", fannedOutEventConnections = {{dstID = \"EO\", -1}}, numFECons = 1")) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(network(conn, 1, "", 0,
                                      ", fannedOutEventConnections = {{connectionNum = 0, dstID = \"REQ\", "
                                      "dstFBNum = 1}}, numFECons = 2")) == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(networkErrors) {
    registerCounterAdapter();
    const auto withFBs = [](const std::string &paFBs, std::size_t paCount, const std::string &paParameters = "",
                            std::size_t paNumParameters = 0) {
      return "return {" + cEmptyInterface + ", fbnSpec = {internalFBs = {" + paFBs + "}, parameters = {" +
             paParameters + "}, eventConnections = {}, dataConnections = {}, adapterConnections = {}, numFBs = " +
             std::to_string(paCount) +
             ", numECons = 0, numDCons = 0, numAdpCons = 0, numParams = " + std::to_string(paNumParameters) + "}}";
    };
    BOOST_TEST(createResponse(withFBs("{fbNameID = \"A\", fbTypeID = \"pkg::Any\"}", 1)) == EMGMResponse::Ready);
    BOOST_TEST(createResponse(withFBs("{fbNameID = \"A\", fbTypeID = \"pkg::Any\"}, "
                                      "{fbNameID = \"A\", fbTypeID = \"pkg::Any\"}",
                                      2)) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withFBs("{fbNameID = \"S\", fbTypeID = \"pkg::Any\"}", 1)) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withFBs("{fbNameID = \"A\", fbTypeID = \"luacfbtest::Invalid\"}", 1)) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withFBs("{fbNameID = \"1A\", fbTypeID = \"pkg::Any\"}", 1)) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withFBs("{fbNameID = \"A\", fbTypeID = \"pkg:Any\"}", 1)) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withFBs("{fbNameID = \"A\"}", 1)) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withFBs("{fbNameID = \"A\", fbTypeID = \"pkg::Any\"}", 2)) == EMGMResponse::BadParams);
    // parameters
    BOOST_TEST(createResponse(withFBs("{fbNameID = \"A\", fbTypeID = \"pkg::Any\"}", 1,
                                      "{fbNum = 0, diNameID = \"PV\", paramValue = \"'a\\\"b'\"}", 1)) ==
               EMGMResponse::Ready);
    BOOST_TEST(createResponse(withFBs("{fbNameID = \"A\", fbTypeID = \"pkg::Any\"}", 1,
                                      "{fbNum = 1, diNameID = \"PV\", paramValue = \"1\"}", 1)) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withFBs("{fbNameID = \"A\", fbTypeID = \"pkg::Any\"}", 1,
                                      "{fbNum = -1, diNameID = \"PV\", paramValue = \"1\"}", 1)) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withFBs("{fbNameID = \"A\", fbTypeID = \"pkg::Any\"}", 1,
                                      "{fbNum = 0, diNameID = \"PV\", paramValue = 1}", 1)) == EMGMResponse::BadParams);
    // adapter connections: only from an own plug, named by the plug's name, or between internal FBs
    const auto withAdapterConnection = [](const std::string &paConnection) {
      return "return {" + cEmptyInterface +
             ", fbnSpec = {internalFBs = {{fbNameID = \"A\", fbTypeID = \"pkg::Any\"}}, parameters = {}, "
             "eventConnections = {}, dataConnections = {}, adapterConnections = {" +
             paConnection + "}, numFBs = 1, numECons = 0, numDCons = 0, numAdpCons = 1, numParams = 0}}";
    };
    BOOST_TEST(createResponse(withAdapterConnection(
                   "{srcID = \"P\", srcFBNum = 65536, dstID = \"S\", dstFBNum = 0}")) == EMGMResponse::Ready);
    BOOST_TEST(createResponse(withAdapterConnection("{srcID = \"P\", srcFBNum = 0, dstID = \"S\", dstFBNum = 0}")) ==
               EMGMResponse::Ready);
    BOOST_TEST(createResponse(withAdapterConnection(
                   "{srcID = \"X\", srcFBNum = 65536, dstID = \"S\", dstFBNum = 0}")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withAdapterConnection(
                   "{srcID = \"S\", srcFBNum = 65537, dstID = \"S\", dstFBNum = 0}")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withAdapterConnection(
                   "{srcID = \"P\", srcFBNum = 0, dstID = \"S\", dstFBNum = 65537}")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withAdapterConnection("{srcID = \"P\", srcFBNum = 0, dstID = \"S\", dstFBNum = -1}")) ==
               EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(instanceErrors) {
    registerCounterTypes();
    const auto single = [](const std::string &paType, const std::string &paParameters, std::size_t paNumParameters,
                           const std::string &paEvents = "", std::size_t paNumEvents = 0) {
      return "return {interfaceSpec = {numEIs = 0, EINames = {}, EIWith = {}, EIWithIndexes = {}, numEOs = 0, "
             "EONames = {}, EOWith = {}, EOWithIndexes = {}, numDIs = 0, DINames = {}, DIDataTypeNames = {}, "
             "numDOs = 0, DONames = {}, DODataTypeNames = {}, numAdapters = 0, adapterInstanceDefinition = {}}, "
             "fbnSpec = {internalFBs = {{fbNameID = \"A\", fbTypeID = \"" +
             paType + "\"}}, parameters = {" + paParameters + "}, eventConnections = {" + paEvents +
             "}, dataConnections = {}, adapterConnections = {}, numFBs = 1, numECons = " + std::to_string(paNumEvents) +
             ", numDCons = 0, numAdpCons = 0, numParams = " + std::to_string(paNumParameters) + "}}";
    };
    registerCFB("luacfbtest::Fine"_STRID, single("luacfbtest::Counter", "", 0));
    registerCFB("luacfbtest::UnknownType"_STRID, single("luacfbtest::Missing", "", 0));
    registerCFB("luacfbtest::UnknownInput"_STRID,
                single("luacfbtest::Counter", "{fbNum = 0, diNameID = \"NOPE\", paramValue = \"1\"}", 1));
    registerCFB("luacfbtest::BadValue"_STRID,
                single("luacfbtest::Counter", "{fbNum = 0, diNameID = \"PV\", paramValue = \"abc\"}", 1));
    registerCFB(
        "luacfbtest::UnknownPort"_STRID,
        single("luacfbtest::Counter", "", 0, "{srcID = \"NOPE\", srcFBNum = 0, dstID = \"REQ\", dstFBNum = 0}", 1));
    // LoopB contains LoopA, which contains LoopB
    registerCFB("luacfbtest::LoopB"_STRID, single("luacfbtest::LoopA", "", 0));
    registerCFB("luacfbtest::LoopA"_STRID, single("luacfbtest::LoopB", "", 0));

    for (const StringId type :
         {"luacfbtest::UnknownType"_STRID, "luacfbtest::UnknownInput"_STRID, "luacfbtest::BadValue"_STRID,
          "luacfbtest::UnknownPort"_STRID, "luacfbtest::LoopA"_STRID}) {
      BOOST_TEST_INFO(type.data());
      BOOST_TEST(createInstance("Broken"_STRID, type) == nullptr);
    }
    const auto entry = getRuntimeFBTypeEntry("luacfbtest::LoopA"_STRID);
    BOOST_TEST(entry->getInstanceCount() == 0U);
    SInstance fine("Fine"_STRID, "luacfbtest::Fine"_STRID);
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::lua::test
