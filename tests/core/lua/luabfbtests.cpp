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
#include "../../../core/src/lua/luaengine.h"

#include "forte/ecet.h"
#include "forte/resource.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_int.h"
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

    // ACounter from the IDE examples: REQ samples PV, CNF carries Q and CV
    constexpr char cCounterAdapter[] = R"lua(
return {interfaceSpec = {
  numEIs = 2, EINames = {"REQ", "R"}, EIWith = {0, 65535}, EIWithIndexes = {0, -1},
  numEOs = 1, EONames = {"CNF"}, EOWith = {0, 1, 65535}, EOWithIndexes = {0},
  numDIs = 1, DINames = {"PV"}, DIDataTypeNames = {"INT"},
  numDOs = 2, DONames = {"Q", "CV"}, DODataTypeNames = {"BOOL", "INT"}
}}
)lua";

    // Counter from the IDE examples (docs/ide/forte-lua-type-deployment/examples/02-basic), with a qualified adapter
    constexpr char cCounter[] = R"lua(
local STfunc = require "STfunc"

local FB_STATE = 0
local ECC_START = 0
local ECC_INIT = 1
local ECC_COUNT = 2
local EI_INIT = 0
local EI_REQ = 1
local EO_INITO = 0
local EO_CNF = 1
local DI_PV = 33554432
local DO_Q = 67108864
local DO_CV = 67108865
local IN_STEP = 268435456

local function alg_initialize(fb)
  local ENV = {}
  ENV.fb_var_CV = 0
  ENV.fb_var_STEP = 1
  ENV.fb_var_Q = false
  fb[IN_STEP] = ENV.fb_var_STEP
  fb[DO_Q] = ENV.fb_var_Q
  fb[DO_CV] = ENV.fb_var_CV
end

local function alg_count(fb)
  local ENV = {}
  ENV.fb_var_PV = fb[DI_PV]
  ENV.fb_var_CV = fb[DO_CV]
  ENV.fb_var_STEP = fb[IN_STEP]
  ENV.fb_var_CV = (ENV.fb_var_CV + ENV.fb_var_STEP)
  ENV.fb_var_Q = ENV.fb_var_CV >= ENV.fb_var_PV
  fb[DO_Q] = ENV.fb_var_Q
  fb[DO_CV] = ENV.fb_var_CV
end

local function enterECC_START(fb)
  fb[FB_STATE] = ECC_START
  return true
end

local function enterECC_INIT(fb)
  fb[FB_STATE] = ECC_INIT
  alg_initialize(fb)
  fb(EO_INITO)
  return true
end

local function enterECC_COUNT(fb)
  fb[FB_STATE] = ECC_COUNT
  alg_count(fb)
  fb(EO_CNF)
  return true
end

local function transition(fb, id)
  local STATE = fb[FB_STATE]
  local VAR_PV = fb[DI_PV]
  local VAR_CV = fb[DO_CV]
  if ECC_START == STATE then
  if EI_INIT == id and true then return enterECC_INIT(fb)
  elseif EI_REQ == id and (VAR_CV < VAR_PV) then return enterECC_COUNT(fb)
  else return false
  end
  elseif ECC_INIT == STATE then
  if true and 1 then return enterECC_START(fb)
  else return false
  end
  elseif ECC_COUNT == STATE then
  if true and 1 then return enterECC_START(fb)
  else return false
  end
  else return false
  end
end

local function executeEvent(fb, id)
  local modified = transition(fb, id)
  while modified do
    modified = transition(fb, -1)
  end
end

local interfaceSpec = {
  numEIs = 2,
  EINames = {"INIT", "REQ"},
  EIWith = {0, 65535, 0, 65535},
  EIWithIndexes = {0, 2},
  numEOs = 2,
  EONames = {"INITO", "CNF"},
  EOWith = {1, 65535, 0, 1, 65535},
  EOWithIndexes = {0, 2},
  numDIs = 1,
  DINames = {"PV"},
  DIDataTypeNames = {"INT"},
  numDOs = 2,
  DONames = {"Q", "CV"},
  DODataTypeNames = {"BOOL", "INT"},
  numAdapters = 1,
  adapterInstanceDefinition = {
    {adapterNameID = "NEXT", adapterTypeNameID = "luabfbtest::ACounter", isPlug = true}
  }
}

local internalVarsInformation = {
  numIntVars = 1,
  intVarsNames = {"STEP"},
  intVarsDataTypeNames = {"INT"}
}

return {ECC = executeEvent, interfaceSpec = interfaceSpec, internalVarsInformation = internalVarsInformation}
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
      {adapterNameID = "P", adapterTypeNameID = "luabfbtest::ACounter", isPlug = true}
    }
  }
}
)lua";

    // socket side of ACounter: GO sends X as PV with REQ, CNF delivers CV as RESULT with DONE
    constexpr char cClient[] = R"lua(
local EI_GO = 0
local EO_DONE = 0
local DI_X = 33554432
local DO_RESULT = 67108864
local AEI_S_CNF = 134217728
local AEO_S_REQ = 134217728
local ADO_S_PV = 167772160
local ADI_S_CV = 201326593

return {
  ECC = function(fb, id)
    if id == EI_GO then
      fb[ADO_S_PV] = fb[DI_X]
      fb(AEO_S_REQ)
    elseif id == AEI_S_CNF then
      fb[DO_RESULT] = fb[ADI_S_CV]
      fb(EO_DONE)
    end
  end,
  interfaceSpec = {
    numEIs = 1, EINames = {"GO"}, EIWith = {0, 65535}, EIWithIndexes = {0},
    numEOs = 1, EONames = {"DONE"}, EOWith = {0, 65535}, EOWithIndexes = {0},
    numDIs = 1, DINames = {"X"}, DIDataTypeNames = {"INT"},
    numDOs = 1, DONames = {"RESULT"}, DODataTypeNames = {"INT"},
    numAdapters = 1,
    adapterInstanceDefinition = {
      {adapterNameID = "S", adapterTypeNameID = "luabfbtest::ACounter", isPlug = false}
    }
  }
}
)lua";

    // every input event but OK fails in its own way
    constexpr char cFaulty[] = R"lua(
local DO_OUT = 67108864
return {
  ECC = function(fb, id)
    if id == 0 then
      local missing = fb[33554432 + 7]
    elseif id == 1 then
      fb[DO_OUT] = "text"
    elseif id == 2 then
      fb(5)
    elseif id == 3 then
      error("custom failure")
    elseif id == 4 then
      fb[268435456 + 65536] = 1
    else
      fb[DO_OUT] = 7
      fb(0)
    end
  end,
  interfaceSpec = {
    numEIs = 6, EINames = {"A", "B", "C", "D", "E", "OK"}, EIWith = {}, EIWithIndexes = {-1, -1, -1, -1, -1, -1},
    numEOs = 1, EONames = {"CNF"}, EOWith = {0, 65535}, EOWithIndexes = {0},
    numDIs = 0, DINames = {}, DIDataTypeNames = {},
    numDOs = 1, DONames = {"OUT"}, DODataTypeNames = {"INT"},
    numAdapters = 0,
    adapterInstanceDefinition = {
    }
  }
}
)lua";

    // every kind of variable with an initial value, R without one
    constexpr char cInitialValues[] = R"lua(
return {
  ECC = function(fb, id) end,
  interfaceSpec = {
    numEIs = 0, EINames = {}, EIWith = {}, EIWithIndexes = {},
    numEOs = 0, EONames = {}, EOWith = {}, EOWithIndexes = {},
    numDIs = 1, DINames = {"A"}, DIDataTypeNames = {"INT"}, DIInitialValues = {"5"},
    numDOs = 2, DONames = {"S", "B"}, DODataTypeNames = {"STRING", "BOOL"}, DOInitialValues = {"'it$'s'", ""},
    numAdapters = 0, adapterInstanceDefinition = {}
  },
  internalVarsInformation = {
    numIntVars = 3, intVarsNames = {"ARR", "T", "R"},
    intVarsDataTypeNames = {"ARRAY [1..3] OF INT", "TIME", "REAL"},
    intVarsInitialValues = {"[1, 2, 3]", "T#2s", ""}
  }
}
)lua";

    // REQ adds STEP to the in-out V, which is read with REQ and written back with CNF
    constexpr char cInOut[] = R"lua(
local DI_STEP = 33554432
local DIO_V = 100663296
return {
  ECC = function(fb, id)
    if id == 0 then
      fb[DIO_V] = fb[DIO_V] + fb[DI_STEP]
      fb(0)
    end
  end,
  interfaceSpec = {
    numEIs = 1, EINames = {"REQ"}, EIWith = {0, 1, 65535}, EIWithIndexes = {0},
    numEOs = 1, EONames = {"CNF"}, EOWith = {0, 65535}, EOWithIndexes = {0},
    numDIs = 1, DINames = {"STEP"}, DIDataTypeNames = {"INT"}, DIInitialValues = {"1"},
    numDOs = 0, DONames = {}, DODataTypeNames = {},
    numDIOs = 1, DIONames = {"V"}, DIODataTypeNames = {"INT"}, DIOInitialValues = {"10"},
    numAdapters = 0, adapterInstanceDefinition = {}
  }
}
)lua";

    EMGMResponse createBFB(StringId paName, const std::string &paSource, std::shared_ptr<CLuaBFBTypeEntry> &paEntry) {
      std::string error;
      const EMGMResponse response = CLuaBFBTypeEntry::create(definitionEngine(), paName, paSource, paEntry, error);
      BOOST_TEST_MESSAGE("create " << paName.data() << ": " << error);
      return response;
    }

    EMGMResponse createResponse(const std::string &paSource) {
      std::shared_ptr<CLuaBFBTypeEntry> entry;
      return createBFB("luabfbtest::Invalid"_STRID, paSource, entry);
    }

    //! Registers the type, returns the registered entry (an earlier one with the same definition is kept)
    std::shared_ptr<const CRuntimeFBTypeEntry> registerBFB(StringId paName, const std::string &paSource) {
      std::shared_ptr<CLuaBFBTypeEntry> entry;
      BOOST_REQUIRE(createBFB(paName, paSource, entry) == EMGMResponse::Ready);
      BOOST_REQUIRE(registerRuntimeFBType(entry) == EMGMResponse::Ready);
      return getRuntimeFBTypeEntry(paName);
    }

    void registerCounterAdapter() {
      std::shared_ptr<CLuaAdapterTypeEntry> entry;
      std::string error;
      BOOST_REQUIRE(CLuaAdapterTypeEntry::create(definitionEngine(), "luabfbtest::ACounter"_STRID, cCounterAdapter,
                                                 entry, error) == EMGMResponse::Ready);
      BOOST_REQUIRE(registerRuntimeAdapterType(entry) == EMGMResponse::Ready);
    }

    //! Starts an event chain at paFB in the test resource and waits for it to finish
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

    std::string text(const CIEC_ANY *paValue) {
      std::string result;
      BOOST_REQUIRE(paValue != nullptr);
      paValue->toString(result);
      return result;
    }

    //! An instance of a run-time type in the test resource, deleted at the end of the scope
    struct SInstance {
        SInstance(StringId paName, StringId paType) :
            mFB(forte::createFB(paName, paType, CFBTestDataGlobalFixture::getResource())) {
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

    struct SCounterTypes {
        SCounterTypes() {
          registerCounterAdapter();
          registerBFB("luabfbtest::Counter"_STRID, cCounter);
        }
    };

    class CLuaCounterFixture : private SCounterTypes, public CFBTestFixtureBase {
      public:
        CLuaCounterFixture() : CFBTestFixtureBase("luabfbtest::Counter"_STRID) {
          setInputData({&mPV});
          setOutputData({&mQ, &mCV});
          setup();
        }

      protected:
        CIEC_INT mPV;
        CIEC_BOOL mQ;
        CIEC_INT mCV;
    };

    struct SFaultyType {
        SFaultyType() {
          registerBFB("luabfbtest::Faulty"_STRID, cFaulty);
        }
    };

    class CLuaFaultyFixture : private SFaultyType, public CFBTestFixtureBase {
      public:
        CLuaFaultyFixture() : CFBTestFixtureBase("luabfbtest::Faulty"_STRID) {
          setOutputData({&mOut});
          setup();
        }

      protected:
        CIEC_INT mOut;
    };
  } // namespace

  BOOST_FIXTURE_TEST_SUITE(LuaBFBCounterTests, CLuaCounterFixture)

  BOOST_AUTO_TEST_CASE(initAndCount) {
    triggerEvent(0);
    BOOST_TEST(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(static_cast<TForteInt16>(mCV) == 0);
    BOOST_TEST(!static_cast<bool>(mQ));

    mPV = CIEC_INT(2);
    triggerEvent(1);
    BOOST_TEST(checkForSingleOutputEventOccurence(1));
    BOOST_TEST(static_cast<TForteInt16>(mCV) == 1);
    BOOST_TEST(!static_cast<bool>(mQ));

    triggerEvent(1);
    BOOST_TEST(checkForSingleOutputEventOccurence(1));
    BOOST_TEST(static_cast<TForteInt16>(mCV) == 2);
    BOOST_TEST(static_cast<bool>(mQ));

    // the guard CV < PV blocks further counting
    triggerEvent(1);
    BOOST_TEST(eventChainEmpty());
  }

  BOOST_AUTO_TEST_SUITE_END()

  BOOST_FIXTURE_TEST_SUITE(LuaBFBFaultTests, CLuaFaultyFixture)

  BOOST_AUTO_TEST_CASE(errorsAreContained) {
    for (TPortId event = 0; event < 5; ++event) {
      triggerEvent(event);
      BOOST_TEST(eventChainEmpty());
    }
    // the instance keeps working after errors
    triggerEvent(5);
    BOOST_TEST(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(static_cast<TForteInt16>(mOut) == 7);
  }

  BOOST_AUTO_TEST_SUITE_END()

  BOOST_AUTO_TEST_SUITE(LuaBFBTests)

  BOOST_AUTO_TEST_CASE(internalVariablesAndInstanceCount) {
    registerCounterAdapter();
    auto entry = registerBFB("luabfbtest::Counter"_STRID, cCounter);
    {
      SInstance counter("CountedCounter"_STRID, "luabfbtest::Counter"_STRID);
      BOOST_TEST(entry->getInstanceCount() == 1U);
      triggerAndWait(*counter.mFB, 0);
      BOOST_TEST(intValue(counter->getVar(std::array{"STEP"_STRID})) == 1);
      BOOST_TEST(counter->getVar(std::array{"NEXT"_STRID}) == nullptr);
      BOOST_TEST(counter->getPlugPin("NEXT"_STRID) != nullptr);
      BOOST_TEST(counter->getPlugPin("NEXT"_STRID)->getAdapterTypeId() == "luabfbtest::ACounter"_STRID);
    }
    BOOST_TEST(entry->getInstanceCount() == 0U);
  }

  BOOST_AUTO_TEST_CASE(initialValues) {
    registerBFB("luabfbtest::InitialValues"_STRID, cInitialValues);
    SInstance fb("Initial"_STRID, "luabfbtest::InitialValues"_STRID);
    const auto check = [&fb] {
      BOOST_TEST(intValue(fb->getDI(0)) == 5);
      BOOST_TEST(text(fb->getDO(0)) == "'it$'s'");
      BOOST_TEST(text(fb->getDO(1)) == "FALSE");
      BOOST_TEST(text(fb->getVar(std::array{"ARR"_STRID})) == "[1,2,3]");
      BOOST_TEST(text(fb->getVar(std::array{"T"_STRID})) == "T#2s");
      BOOST_TEST(text(fb->getVar(std::array{"R"_STRID})) == "0.0");
    };
    check();

    BOOST_REQUIRE(fb->getDI(0)->fromString("9") == 1);
    BOOST_REQUIRE(fb->getDO(0)->fromString("'x'") == 3);
    BOOST_REQUIRE(fb->getVar(std::array{"ARR"_STRID})->fromString("[7, 8, 9]") == 9);
    BOOST_REQUIRE(fb->getVar(std::array{"T"_STRID})->fromString("T#5s") == 4);
    BOOST_TEST(fb->changeExecutionState(EMGMCommandType::Stop) == EMGMResponse::Ready);
    BOOST_TEST(fb->changeExecutionState(EMGMCommandType::Reset) == EMGMResponse::Ready);
    check();
    BOOST_TEST(fb->changeExecutionState(EMGMCommandType::Start) == EMGMResponse::Ready);
  }

  BOOST_AUTO_TEST_CASE(initialValueErrors) {
    const auto withValues = [](const std::string &paDIValues, const std::string &paInternalValues) {
      return "return {ECC = function(fb, id) end, interfaceSpec = {numEIs = 0, EINames = {}, EIWith = {}, "
             "EIWithIndexes = {}, numEOs = 0, EONames = {}, EOWith = {}, EOWithIndexes = {}, numDIs = 2, "
             "DINames = {\"A\", \"B\"}, DIDataTypeNames = {\"INT\", \"ARRAY [0..1] OF BOOL\"}, " +
             paDIValues +
             "numDOs = 0, DONames = {}, DODataTypeNames = {}, numAdapters = 0, adapterInstanceDefinition = {}}, "
             "internalVarsInformation = {numIntVars = 1, intVarsNames = {\"I\"}, intVarsDataTypeNames = {\"TIME\"}" +
             paInternalValues + "}}";
    };
    BOOST_TEST(createResponse(withValues("", "")) == EMGMResponse::Ready);
    BOOST_TEST(createResponse(withValues("DIInitialValues = {\"\", \"\"}, ", ", intVarsInitialValues = {\"\"}")) ==
               EMGMResponse::Ready);
    BOOST_TEST(createResponse(withValues("DIInitialValues = {\"-7\", \"[TRUE, FALSE]\"}, ",
                                         ", intVarsInitialValues = {\"T#1h2m\"}")) == EMGMResponse::Ready);
    BOOST_TEST(createResponse(withValues("DIInitialValues = {\"1\"}, ", "")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withValues("DIInitialValues = \"1\", ", "")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withValues("DIInitialValues = {1, \"\"}, ", "")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withValues("DIInitialValues = {\"abc\", \"\"}, ", "")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withValues("DIInitialValues = {\"5x\", \"\"}, ", "")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withValues("DIInitialValues = {\"40000\", \"\"}, ", "")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withValues("DIInitialValues = {\"\", \"[1, 2, 3, 4]\"}, ", "")) ==
               EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withValues("DIInitialValues = {\"5\\0\", \"\"}, ", "")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withValues("", ", intVarsInitialValues = {\"TRUE\"}")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withValues("", ", intVarsInitialValues = {}")) == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(inOutVariables) {
    registerBFB("luabfbtest::InOut"_STRID, cInOut);
    SInstance first("First"_STRID, "luabfbtest::InOut"_STRID);
    SInstance second("Second"_STRID, "luabfbtest::InOut"_STRID);
    BOOST_TEST(first->getFBInterfaceSpec().getNumDIOs() == 1U);
    BOOST_TEST(intValue(first->getDIO(0)) == 10);
    BOOST_TEST(first->getDIO(1) == nullptr);

    // unconnected, the in-out variable works on its own value
    triggerAndWait(*first.mFB, 0);
    BOOST_TEST(intValue(first->getDIO(0)) == 11);
    BOOST_TEST(intValue(first->getDataInOut("V"_STRID)) == 11);

    // connected, the second FB reads and writes back the first one's value
    CInOutDataConnection *const connection = first->getDIOOutConnection("V"_STRID);
    BOOST_REQUIRE(connection != nullptr);
    BOOST_REQUIRE(connection->connect(*second.mFB, std::array{"V"_STRID}) == EMGMResponse::Ready);
    static_cast<CIEC_INT &>(*second->getDI(0)) = CIEC_INT(5);
    triggerAndWait(*second.mFB, 0);
    BOOST_TEST(intValue(second->getDIO(0)) == 16);
    BOOST_TEST(intValue(first->getDIO(0)) == 16);
    triggerAndWait(*first.mFB, 0);
    BOOST_TEST(intValue(first->getDIO(0)) == 17);
    triggerAndWait(*second.mFB, 0);
    BOOST_TEST(intValue(first->getDIO(0)) == 22);
    BOOST_TEST(connection->disconnect(*second.mFB, std::array{"V"_STRID}) == EMGMResponse::Ready);

    BOOST_TEST(first->changeExecutionState(EMGMCommandType::Stop) == EMGMResponse::Ready);
    BOOST_TEST(first->changeExecutionState(EMGMCommandType::Reset) == EMGMResponse::Ready);
    BOOST_TEST(intValue(first->getDIO(0)) == 10);
    BOOST_TEST(first->changeExecutionState(EMGMCommandType::Start) == EMGMResponse::Ready);
  }

  BOOST_AUTO_TEST_CASE(inOutErrors) {
    const auto withInOuts = [](const std::string &paEIWith, const std::string &paInOuts) {
      return "return {ECC = function(fb, id) end, interfaceSpec = {numEIs = 1, EINames = {\"REQ\"}, EIWith = {" +
             paEIWith +
             "}, EIWithIndexes = {0}, numEOs = 0, EONames = {}, EOWith = {}, EOWithIndexes = {}, numDIs = 1, "
             "DINames = {\"A\"}, DIDataTypeNames = {\"INT\"}, numDOs = 0, DONames = {}, DODataTypeNames = {}, " +
             paInOuts + "numAdapters = 0, adapterInstanceDefinition = {}}}";
    };
    const std::string oneInOut = "numDIOs = 1, DIONames = {\"V\"}, DIODataTypeNames = {\"INT\"}, ";
    BOOST_TEST(createResponse(withInOuts("0, 1, 65535", oneInOut)) == EMGMResponse::Ready);
    BOOST_TEST(createResponse(withInOuts("0, 65535", "")) == EMGMResponse::Ready);
    BOOST_TEST(createResponse(withInOuts("0, 1, 65535", "")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withInOuts("2, 65535", oneInOut)) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withInOuts("0, 65535", "numDIOs = 1, DIONames = {\"A\"}, "
                                                     "DIODataTypeNames = {\"INT\"}, ")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withInOuts("0, 65535", "numDIOs = 1, DIONames = {\"V\"}, "
                                                     "DIODataTypeNames = {\"NOPE\"}, ")) ==
               EMGMResponse::UnsupportedType);
    BOOST_TEST(createResponse(withInOuts("0, 65535", "numDIOs = 2, DIONames = {\"V\"}, "
                                                     "DIODataTypeNames = {\"INT\"}, ")) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(withInOuts("0, 65535", oneInOut + "DIOInitialValues = {\"x\"}, ")) ==
               EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(adapterRoundTrip) {
    registerCounterAdapter();
    registerBFB("luabfbtest::Doubler"_STRID, cDoubler);
    registerBFB("luabfbtest::Client"_STRID, cClient);
    SInstance server("Server"_STRID, "luabfbtest::Doubler"_STRID);
    SInstance client("Client"_STRID, "luabfbtest::Client"_STRID);
    BOOST_REQUIRE(server->getPlugPin("P"_STRID)->getAdapterCon().connect(*client.mFB, std::array{"S"_STRID}) ==
                  EMGMResponse::Ready);

    static_cast<CIEC_INT &>(*client->getDI(0)) = CIEC_INT(21);
    triggerAndWait(*client.mFB, 0);
    BOOST_TEST(intValue(client->getDO(0)) == 42);

    BOOST_TEST(server->getPlugPin("P"_STRID)->getAdapterCon().disconnect(*client.mFB, std::array{"S"_STRID}) ==
               EMGMResponse::Ready);
  }

  BOOST_AUTO_TEST_CASE(createErrors) {
    const std::string interface = "interfaceSpec = {numEIs = 0, EINames = {}, EIWith = {}, EIWithIndexes = {}, "
                                  "numEOs = 0, EONames = {}, EOWith = {}, EOWithIndexes = {}, numDIs = 0, "
                                  "DINames = {}, DIDataTypeNames = {}, numDOs = 0, DONames = {}, "
                                  "DODataTypeNames = {}, numAdapters = 0, adapterInstanceDefinition = {}}";
    const std::string ecc = "ECC = function(fb, id) end, ";
    BOOST_TEST(createResponse("return {" + ecc + interface + "}") == EMGMResponse::Ready);
    BOOST_TEST(createResponse("return {" + ecc + interface) == EMGMResponse::InvalidObject);
    BOOST_TEST(createResponse("return {" + interface + "}") == EMGMResponse::BadParams);
    BOOST_TEST(createResponse("return {ECC = 1, " + interface + "}") == EMGMResponse::BadParams);
    BOOST_TEST(createResponse("return {" + ecc + "}") == EMGMResponse::BadParams);
    // basic FBs need the adapter fields
    BOOST_TEST(createResponse("return {" + ecc +
                              "interfaceSpec = {numEIs = 0, EINames = {}, EIWith = {}, EIWithIndexes = {}, numEOs = 0, "
                              "EONames = {}, EOWith = {}, EOWithIndexes = {}, numDIs = 0, DINames = {}, "
                              "DIDataTypeNames = {}, numDOs = 0, DONames = {}, DODataTypeNames = {}}}") ==
               EMGMResponse::BadParams);
    // internal variables
    BOOST_TEST(createResponse("return {" + ecc + interface +
                              ", internalVarsInformation = {numIntVars = 1, intVarsNames = {\"I\"}, "
                              "intVarsDataTypeNames = {\"INT\"}}}") == EMGMResponse::Ready);
    BOOST_TEST(createResponse("return {" + ecc + interface +
                              ", internalVarsInformation = {numIntVars = 2, intVarsNames = {\"I\"}, "
                              "intVarsDataTypeNames = {\"INT\"}}}") == EMGMResponse::BadParams);
    BOOST_TEST(createResponse("return {" + ecc + interface +
                              ", internalVarsInformation = {numIntVars = 1, intVarsNames = {\"I\"}, "
                              "intVarsDataTypeNames = {\"NOT_A_TYPE\"}}}") == EMGMResponse::UnsupportedType);
    BOOST_TEST(createResponse("return {" + ecc + interface + ", internalVarsInformation = 3}") ==
               EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(adapterErrors) {
    const auto withAdapters = [](const std::string &paAdapters, std::size_t paCount) {
      return "return {ECC = function(fb, id) end, interfaceSpec = {numEIs = 0, EINames = {}, EIWith = {}, "
             "EIWithIndexes = {}, numEOs = 0, EONames = {}, EOWith = {}, EOWithIndexes = {}, numDIs = 0, "
             "DINames = {}, DIDataTypeNames = {}, numDOs = 0, DONames = {}, DODataTypeNames = {}, numAdapters = " +
             std::to_string(paCount) + ", adapterInstanceDefinition = {" + paAdapters + "}}}";
    };
    registerCounterAdapter();
    BOOST_TEST(createResponse(withAdapters(
                   "{adapterNameID = \"P\", adapterTypeNameID = \"luabfbtest::ACounter\", isPlug = true}", 1)) ==
               EMGMResponse::Ready);
    BOOST_TEST(createResponse(withAdapters(
                   "{adapterNameID = \"P\", adapterTypeNameID = \"luabfbtest::Missing\", isPlug = true}", 1)) ==
               EMGMResponse::UnsupportedType);
    BOOST_TEST(createResponse(withAdapters(
                   "{adapterNameID = \"P\", adapterTypeNameID = \"luabfbtest::ACounter\", isPlug = true}", 2)) ==
               EMGMResponse::BadParams);
    BOOST_TEST(
        createResponse(withAdapters("{adapterNameID = \"P\", adapterTypeNameID = \"luabfbtest::ACounter\", isPlug = 1}",
                                    1)) == EMGMResponse::BadParams);
    BOOST_TEST(createResponse(
                   withAdapters("{adapterNameID = \"P\", adapterTypeNameID = \"luabfbtest::ACounter\", isPlug = true}, "
                                "{adapterNameID = \"P\", adapterTypeNameID = \"luabfbtest::ACounter\", isPlug = false}",
                                2)) == EMGMResponse::BadParams);
    // compiled adapter types work as well
    BOOST_TEST(createResponse(withAdapters(
                   "{adapterNameID = \"T\", adapterTypeNameID = \"iec61499::events::ATimeOut\", isPlug = true}", 1)) ==
               EMGMResponse::Ready);
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::lua::test
