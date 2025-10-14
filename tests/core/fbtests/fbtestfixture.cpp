/*******************************************************************************
 * Copyright (c) 2011, 2023 ACIN, fortiss GmbH
 *                          Johannes Kepler University
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *   Alois Zoitl - migrated fb tests to boost test infrastructure
 *   Martin Jobst - add reset tests
 *******************************************************************************/
#include "fbtestfixture.h"
#include "fbtesterglobalfixture.h"
#include "forte/util/criticalregion.h"
#include "forte/ecet.h"
#include "../../core/src/typelib_internal.h"

#ifdef WIN32
#define usleep(x) Sleep((x) / 1000)
#endif

namespace forte::test {
  //! Helper class allowing to access the can be connected function from the FBTester
  class CFBTestConn : public CDataConnection {
    public:
      static bool canBeConnected(const CIEC_ANY &paSrcDataPoint, const CIEC_ANY &paDstDataPoint) {
        return CDataConnection::canBeConnected(paSrcDataPoint, paDstDataPoint);
      }

      // you are not allowed to create this class
      CFBTestConn() = delete;
  };

  class CFBTestInputDataConn final : public CDataConnection {
    public:
      CFBTestInputDataConn(CFunctionBlock &paSrcFB, CIEC_ANY &paValue) : CDataConnection(paSrcFB, {}), mValue(paValue) {
      }

      void writeData(const CIEC_ANY &paValue) override {
        mValue.setValue(paValue.unwrap());
      }

      void readData(CIEC_ANY &paValue) const override {
        paValue.setValue(mValue.unwrap());
      }

      CIEC_ANY &getValue() override {
        return mValue;
      }

    private:
      CIEC_ANY &mValue;
  };

  CFBTestFixtureBase::CFBTestFixtureBase(StringId paTypeId) :
      CGenFunctionBlock<CFunctionBlock>(CFBTestDataGlobalFixture::getResource(), {}),
      mTypeId(paTypeId) {
    mFBUnderTest = forte::createFB(paTypeId, paTypeId, CFBTestDataGlobalFixture::getResource());
  }

  bool CFBTestFixtureBase::initialize() {
    if (!CFunctionBlock::initialize()) {
      return false;
    }
    changeExecutionState(EMGMCommandType::Reset);
    changeExecutionState(EMGMCommandType::Start);
    // assure that we are in running state
    BOOST_REQUIRE_EQUAL(CFunctionBlock::E_FBStates::Running, getState());
    BOOST_REQUIRE(nullptr != mFBUnderTest);
    return true;
  }

  CFBTestFixtureBase::~CFBTestFixtureBase() {
    const SFBInterfaceSpec &interfaceSpec(mFBUnderTest->getFBInterfaceSpec());

    performFBResetTests();

    for (size_t i = 0; i < interfaceSpec.getNumEOs(); i++) {
      CEventConnection *eventCon = mFBUnderTest->getEOConnection(interfaceSpec.mEONames[i]);
      BOOST_CHECK_EQUAL(EMGMResponse::Ready, eventCon->disconnect(*this, std::array{interfaceSpec.mEONames[i]}));
    }

    for (size_t i = 0; i < interfaceSpec.getNumDOs(); ++i) {
      CDataConnection *dataCon = mFBUnderTest->getDOConnection(interfaceSpec.mDONames[i]);

      BOOST_CHECK_EQUAL(EMGMResponse::Ready, dataCon->disconnect(*this, std::array{interfaceSpec.mDONames[i]}));
    }

    for (size_t i = 0; i < interfaceSpec.getNumDIs(); ++i) {
      BOOST_CHECK_EQUAL(EMGMResponse::Ready,
                        mDIConnections[i]->disconnect(*mFBUnderTest, std::array{interfaceSpec.mDINames[i]}));
    }

    performFBDeleteTests();
  }

  namespace {
    void checkVars(CIEC_ANY &paTestVar, CIEC_ANY &paFreshVar) {
      if (paFreshVar.getDataTypeID() == CIEC_ANY::e_ANY) {
        if (auto tempVar = std::unique_ptr<CIEC_ANY>(paTestVar.unwrap().clone(nullptr))) {
          tempVar->reset(); // reset the CIEC_ANY not the unique_ptr
          paFreshVar.setValue(*tempVar);
        }
      }
      BOOST_TEST(paTestVar.equals(paFreshVar));
    }
  } // namespace

  void CFBTestFixtureBase::performFBResetTests() {
    const SFBInterfaceSpec &interfaceSpec(mFBUnderTest->getFBInterfaceSpec());

    BOOST_CHECK_EQUAL(EMGMResponse::Ready, mFBUnderTest->changeExecutionState(EMGMCommandType::Stop));
    BOOST_CHECK_EQUAL(EMGMResponse::Ready, mFBUnderTest->changeExecutionState(EMGMCommandType::Reset));

    CFunctionBlock *freshInstance = forte::createFB(mTypeId, mTypeId, *getResource());
    BOOST_REQUIRE(freshInstance != nullptr);

    if (!mConfigString.empty()) {
      freshInstance->configureFB(mConfigString.c_str());
    }

    for (size_t i = 0; i < interfaceSpec.getNumDIs(); ++i) {
      checkVars(*mFBUnderTest->getDI(i), *freshInstance->getDI(i));
    }

    for (size_t i = 0; i < interfaceSpec.getNumDOs(); ++i) {
      checkVars(*mFBUnderTest->getDO(i), *freshInstance->getDO(i));
    }

    BOOST_CHECK(forte::deleteFB(freshInstance));

    BOOST_CHECK_EQUAL(EMGMResponse::Ready, mFBUnderTest->changeExecutionState(EMGMCommandType::Start));
  }

  void CFBTestFixtureBase::performFBDeleteTests() {
    BOOST_CHECK(!mFBUnderTest->isCurrentlyDeleteable());

    BOOST_CHECK_EQUAL(EMGMResponse::Ready, mFBUnderTest->changeExecutionState(EMGMCommandType::Stop));

    BOOST_CHECK(mFBUnderTest->isCurrentlyDeleteable());

    BOOST_CHECK(forte::deleteFB(mFBUnderTest));
    mFBUnderTest = nullptr;
  }

  void CFBTestFixtureBase::setup() {
    BOOST_REQUIRE(initialize());

    setupTestInterface();
    performDataInterfaceTests();

    mFBUnderTest->changeExecutionState(EMGMCommandType::Start);

    createEventOutputConnections();
    createDataInputConnections();
    createDataOutputConnections();
  }

  bool CFBTestFixtureBase::createInterfaceSpec(const char *, SFBInterfaceSpec &) {
    return true;
  }

  void CFBTestFixtureBase::executeEvent(TEventID paEIID, CEventChainExecutionThread *const) {
    util::CCriticalRegion criticalRegion(mOutputEventLock);
    mFBOutputEvents.push_back(paEIID);
  }

  void CFBTestFixtureBase::triggerEvent(TPortId paEIId) {
    CEventChainExecutionThread *execThread = getResource()->getResourceEventExecution();

    execThread->startEventChain(TEventEntry(*mFBUnderTest, paEIId));

    // Wait till event execution for this input event has finished
    do {
      usleep(1);
    } while (execThread->isProcessingEvents());

    for (TPortId index = 0; index < mOutputDataBuffers.size(); ++index) {
      mOutputDataBuffers[index]->setValue(
          mFBUnderTest->getDOConnection(mFBUnderTest->getFBInterfaceSpec().mDONames[index])->getValue().unwrap());
    }
  }

  TEventID CFBTestFixtureBase::pullFirstChainEventID() {
    util::CCriticalRegion criticalRegion(mOutputEventLock);
    if (mFBOutputEvents.empty()) {
      return cgInvalidEventID;
    }
    TEventID retVal = mFBOutputEvents.front();
    mFBOutputEvents.pop_front();
    return retVal;
  }

  bool CFBTestFixtureBase::eventChainEmpty() {
    util::CCriticalRegion criticalRegion(mOutputEventLock);
    return mFBOutputEvents.empty();
  }

  void CFBTestFixtureBase::clearEventChain() {
    mFBOutputEvents.clear();
  }

  bool CFBTestFixtureBase::checkForSingleOutputEventOccurence(TEventID paExpectedEOId) {
    // Test if event chain is not empty, that the first entry is the expected one and that no furhter events are there
    return (!eventChainEmpty() && (pullFirstChainEventID() == paExpectedEOId) && eventChainEmpty());
  }

  void CFBTestFixtureBase::setInputData(std::initializer_list<CIEC_ANY *> paInputData) {
    mInputDataBuffers.assign(paInputData);
  }

  void CFBTestFixtureBase::setOutputData(std::initializer_list<CIEC_ANY *> paOutputData) {
    mOutputDataBuffers.assign(paOutputData);
  }

  void CFBTestFixtureBase::createGenInputData() {
    size_t numDOsTestee = mFBUnderTest->getFBInterfaceSpec().getNumDOs();
    mGenDIs.reserve(numDOsTestee);
    for (size_t i = 0; i < numDOsTestee; i++) {
      mGenDIs.emplace_back(mFBUnderTest->getDO(i)->clone(nullptr));
    }
  }

  void CFBTestFixtureBase::setupTestInterface() {
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Idle, mFBUnderTest->getState());
    BOOST_CHECK_EQUAL(getFBTypeId(), mFBUnderTest->getInstanceNameId());

    const SFBInterfaceSpec &interfaceSpec(mFBUnderTest->getFBInterfaceSpec());

    SFBInterfaceSpec &testerInterfaceSpec(getGenInterfaceSpec());
    testerInterfaceSpec.mEINames = interfaceSpec.mEONames;
    testerInterfaceSpec.mEONames = {};
    testerInterfaceSpec.mDINames = interfaceSpec.mDONames;
    testerInterfaceSpec.mDONames = {};
    testerInterfaceSpec.mDIONames = {};
    testerInterfaceSpec.mSocketNames = {};
    testerInterfaceSpec.mPlugNames = {};

    setupFBInterface();
  }

  void CFBTestFixtureBase::performDataInterfaceTests() {
    const SFBInterfaceSpec &interfaceSpec(mFBUnderTest->getFBInterfaceSpec());

    BOOST_REQUIRE_EQUAL(interfaceSpec.getNumDIs(), mInputDataBuffers.size());

    BOOST_CHECK(nullptr == mFBUnderTest->getDataInput({}));
    BOOST_CHECK_EQUAL(cgInvalidPortId, interfaceSpec.getDIID({}));

    for (TPortId i = 0; i < interfaceSpec.getNumDIs(); ++i) {
      CIEC_ANY *val = mFBUnderTest->getDataInput(interfaceSpec.mDINames[i]);
      BOOST_REQUIRE(nullptr != val);
      // either we have the same datatypeid or the input is any
      BOOST_REQUIRE((mInputDataBuffers[i]->getDataTypeID() == val->getDataTypeID()) ||
                    (CIEC_ANY::e_ANY == val->getDataTypeID()));

      BOOST_CHECK_EQUAL(val, mFBUnderTest->getDIFromPortId(i));
      StringId stringIdBuf = interfaceSpec.mDINames[i];
      BOOST_CHECK_EQUAL(val, mFBUnderTest->getVar(std::array{stringIdBuf}));

      BOOST_CHECK_EQUAL(i, interfaceSpec.getDIID(interfaceSpec.mDINames[i]));

      // we should not be able to get a data output with a data input name
      BOOST_CHECK(nullptr == mFBUnderTest->getDataOutput(interfaceSpec.mDINames[i]));
      BOOST_CHECK_EQUAL(cgInvalidPortId, interfaceSpec.getDOID(interfaceSpec.mDINames[i]));
    }

    for (TPortId i = interfaceSpec.getNumDIs(); i <= cgInvalidPortId; ++i) {
      BOOST_CHECK(nullptr == mFBUnderTest->getDIFromPortId(i));
    }

    BOOST_CHECK_EQUAL(interfaceSpec.getNumDOs(), mOutputDataBuffers.size());

    BOOST_CHECK(nullptr == mFBUnderTest->getDataOutput({}));
    BOOST_CHECK_EQUAL(cgInvalidPortId, interfaceSpec.getDOID({}));

    for (TPortId i = 0; i < interfaceSpec.getNumDOs(); ++i) {
      CIEC_ANY *val = mFBUnderTest->getDataOutput(interfaceSpec.mDONames[i]);
      BOOST_REQUIRE(nullptr != val);
      // either we have the same datatypeid or the input is any
      BOOST_REQUIRE((mOutputDataBuffers[i]->getDataTypeID() == val->getDataTypeID()) ||
                    (CIEC_ANY::e_ANY == val->getDataTypeID()));

      StringId stringIdBuf = interfaceSpec.mDONames[i];
      BOOST_CHECK_EQUAL(val, mFBUnderTest->getVar(std::array{stringIdBuf}));

      BOOST_CHECK_EQUAL(i, interfaceSpec.getDOID(interfaceSpec.mDONames[i]));

      // we should not be able to get a data out with a data output name
      BOOST_CHECK(nullptr == mFBUnderTest->getDataInput(interfaceSpec.mDONames[i]));
      BOOST_CHECK_EQUAL(cgInvalidPortId, interfaceSpec.getDIID(interfaceSpec.mDONames[i]));
    }
  }

  void CFBTestFixtureBase::createEventOutputConnections() {
    const SFBInterfaceSpec &interfaceSpec(mFBUnderTest->getFBInterfaceSpec());

    for (TPortId i = 0; i < interfaceSpec.getNumEOs(); i++) {
      CEventConnection *eventCon = mFBUnderTest->getEOConnection(interfaceSpec.mEONames[i]);
      BOOST_REQUIRE_EQUAL(EMGMResponse::Ready, eventCon->connect(*this, std::array{interfaceSpec.mEONames[i]}));
    }
  }

  void CFBTestFixtureBase::createDataInputConnections() {
    const SFBInterfaceSpec &interfaceSpec(mFBUnderTest->getFBInterfaceSpec());
    mDIConnections.reserve(interfaceSpec.getNumDIs());

    for (size_t i = 0; i < interfaceSpec.getNumDIs(); ++i) {
      auto &con = mDIConnections.emplace_back(std::make_unique<CFBTestInputDataConn>(*this, *mInputDataBuffers[i]));
      BOOST_REQUIRE_EQUAL(EMGMResponse::Ready, con->connect(*mFBUnderTest, std::array{interfaceSpec.mDINames[i]}));
    }
  }

  void CFBTestFixtureBase::createDataOutputConnections() {
    const SFBInterfaceSpec &interfaceSpec(mFBUnderTest->getFBInterfaceSpec());

    for (size_t i = 0; i < interfaceSpec.getNumDOs(); ++i) {
      if (CFBTestConn::canBeConnected(*mOutputDataBuffers[i],
                                      *mFBUnderTest->getDataOutput(interfaceSpec.mDONames[i]))) {
        CDataConnection *dataCon = mFBUnderTest->getDOConnection(interfaceSpec.mDONames[i]);
        BOOST_REQUIRE_EQUAL(EMGMResponse::Ready, dataCon->connect(*this, std::array{interfaceSpec.mDONames[i]}));
      }
    }
  }
} // namespace forte::test
