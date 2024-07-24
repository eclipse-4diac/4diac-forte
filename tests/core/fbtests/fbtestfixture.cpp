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
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "fbtestfixture_gen.cpp"
#endif
#include "fbtesterglobalfixture.h"
#include "device.h"
#include <criticalregion.h>
#include <ecet.h>

#ifdef WIN32
# define usleep(x) Sleep((x)/1000)
#endif

/**Helper functor for deleting stuff in containers
 *
 */
struct SDeleteFunctor {
    template<class T>
    void operator()(T* paVal) const {
      delete paVal;
    }
};

//! Helper class allowing to access the can be connected function from the FBTester
class CFBTestConn : public CDataConnection {
  public:
    static bool canBeConnected(const CIEC_ANY *paSrcDataPoint, const CIEC_ANY *paDstDataPoint) {
      return CDataConnection::canBeConnected(paSrcDataPoint, paDstDataPoint);
    }
  private:
    //you are not allowed to create this class therefor constructor and destructor are private
    CFBTestConn();
    virtual ~CFBTestConn();
};

CFBTestFixtureBase::CFBTestFixtureBase(CStringDictionary::TStringId paTypeId) :
        CGenFunctionBlock<CFunctionBlock>(CFBTestDataGlobalFixture::getResource(), nullptr, 0), mTypeId(paTypeId),
        mFBUnderTest(CTypeLib::createFB(paTypeId, paTypeId, CFBTestDataGlobalFixture::getResource())) {
}

bool CFBTestFixtureBase::initialize() {
  if(!CFunctionBlock::initialize()) {
    return false;
  }
  changeExecutionState(EMGMCommandType::Reset);
  changeExecutionState(EMGMCommandType::Start);
  //assure that we are in running state
  BOOST_REQUIRE_EQUAL(CFunctionBlock::E_FBStates::Running, getState());
  BOOST_REQUIRE(nullptr != mFBUnderTest);
  return true;
}

CFBTestFixtureBase::~CFBTestFixtureBase(){
  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  performFBResetTests();

  for(size_t i = 0; i < interfaceSpec->mNumEOs; i++) {
   CEventConnection *eventCon = mFBUnderTest->getEOConnection(interfaceSpec->mEONames[i]);
   BOOST_CHECK_EQUAL(EMGMResponse::Ready, eventCon->disconnect(this, interfaceSpec->mEONames[i]));
  }

  for(size_t i = 0; i < interfaceSpec->mNumDOs; ++i) {
   CDataConnection *dataCon = mFBUnderTest->getDOConnection(interfaceSpec->mDONames[i]);
   //set it to zero so that when the FB under test is deleted it will not delete our test output data
   dataCon->setValue(nullptr);

   BOOST_CHECK_EQUAL(EMGMResponse::Ready, dataCon->disconnect(this, interfaceSpec->mDONames[i]));
  }

  for(size_t i = 0; i < interfaceSpec->mNumDIs; ++i) {
   BOOST_CHECK_EQUAL(EMGMResponse::Ready, mDIConnections[i]->disconnect(mFBUnderTest, interfaceSpec->mDINames[i]));
  }

  for_each(mDIConnections.begin(), mDIConnections.end(), SDeleteFunctor());

  performFBDeleteTests();

  if(nullptr != mInterfaceSpec){
    freeFBInterfaceData();  //clean the interface and connections first.
    delete mInterfaceSpec;
    mInterfaceSpec = nullptr; //this stops the base classes from any wrong clean-up
  }
}

void CFBTestFixtureBase::performFBResetTests() {
  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  BOOST_CHECK_EQUAL(EMGMResponse::Ready, mFBUnderTest->changeExecutionState(EMGMCommandType::Stop));
  BOOST_CHECK_EQUAL(EMGMResponse::Ready, mFBUnderTest->changeExecutionState(EMGMCommandType::Reset));

  CFunctionBlock *freshInstance = CTypeLib::createFB(mTypeId, mTypeId, *getResource());
  BOOST_REQUIRE(freshInstance != nullptr);

  if(!mConfigString.empty()) {
    freshInstance->configureFB(mConfigString.c_str());
  }

  for(size_t i = 0; i < interfaceSpec->mNumDIs; ++i) {
    BOOST_TEST(mFBUnderTest->getDI(i)->equals(*freshInstance->getDI(i)));
  }
  for(size_t i = 0; i < interfaceSpec->mNumDOs; ++i) {
    BOOST_TEST(mFBUnderTest->getDO(i)->equals(*freshInstance->getDO(i)));
  }

  BOOST_CHECK(CTypeLib::deleteFB(freshInstance));

  BOOST_CHECK_EQUAL(EMGMResponse::Ready, mFBUnderTest->changeExecutionState(EMGMCommandType::Start));
}

void CFBTestFixtureBase::performFBDeleteTests() {
  BOOST_CHECK(!mFBUnderTest->isCurrentlyDeleteable());

  BOOST_CHECK_EQUAL(EMGMResponse::Ready, mFBUnderTest->changeExecutionState(EMGMCommandType::Stop));

  BOOST_CHECK(mFBUnderTest->isCurrentlyDeleteable());

  BOOST_CHECK(CTypeLib::deleteFB(mFBUnderTest));
  mFBUnderTest = nullptr;
}

void CFBTestFixtureBase::setup(const char* paConfigString){
  if(paConfigString != nullptr) {
    mConfigString = paConfigString;
    mFBUnderTest->configureFB(paConfigString);
  }
  BOOST_ASSERT(initialize());

  setupTestInterface();
  performDataInterfaceTests();

  mFBUnderTest->changeExecutionState(EMGMCommandType::Start);

  createEventOutputConnections();
  createDataInputConnections();
  createDataOutputConnections();
}

bool CFBTestFixtureBase::createInterfaceSpec(const char *, SFBInterfaceSpec &paInterfaceSpec) {
  paInterfaceSpec = *mInterfaceSpec;
  return true;
}

void CFBTestFixtureBase::executeEvent(TEventID paEIID, CEventChainExecutionThread *const) {
  CCriticalRegion criticalRegion(mOutputEventLock);
  mFBOutputEvents.push_back(paEIID);
}

void CFBTestFixtureBase::triggerEvent(TPortId paEIId) {
  CEventChainExecutionThread *execThread = getResource()->getResourceEventExecution();
  TEventEntry entry(mFBUnderTest, paEIId);

  execThread->startEventChain(entry);

  //Wait till event execution for this input event has finished
  do {
    usleep(1);
  } while(execThread->isProcessingEvents());

}

TEventID CFBTestFixtureBase::pullFirstChainEventID() {
  CCriticalRegion criticalRegion(mOutputEventLock);
  TEventID retVal = mFBOutputEvents.front();
  mFBOutputEvents.pop_front();
  return retVal;
}

bool CFBTestFixtureBase::eventChainEmpty() {
  CCriticalRegion criticalRegion(mOutputEventLock);
  return mFBOutputEvents.empty();
}

void CFBTestFixtureBase::clearEventChain() {
  mFBOutputEvents.clear();
}

bool CFBTestFixtureBase::checkForSingleOutputEventOccurence(TEventID paExpectedEOId) {
  //Test if event chain is not empty, that the first entry is the expected one and that no furhter events are there
  return (!eventChainEmpty() && (pullFirstChainEventID() == paExpectedEOId) && eventChainEmpty());
}

void CFBTestFixtureBase::setInputData(std::initializer_list<CIEC_ANY*> paInputData){
  mInputDataBuffers.assign(paInputData);
}

void CFBTestFixtureBase::setOutputData(std::initializer_list<CIEC_ANY*> paOutputData){
  mOutputDataBuffers.assign(paOutputData);
}

void CFBTestFixtureBase::setupTestInterface(){
  BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Idle, mFBUnderTest->getState());
  if (mFBUnderTest->getFBTypeId() < g_nStringIdNextFreeId) { // not a generic block
    BOOST_CHECK_EQUAL(getFBTypeId(), mFBUnderTest->getFBTypeId());
  }
  BOOST_CHECK_EQUAL(getFBTypeId(), mFBUnderTest->getInstanceNameId());

  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  SFBInterfaceSpec* testerInterfaceSpec = new SFBInterfaceSpec;
  testerInterfaceSpec->mNumEIs = interfaceSpec->mNumEOs;
  testerInterfaceSpec->mEINames = interfaceSpec->mEONames;
  testerInterfaceSpec->mEIWith = nullptr;
  testerInterfaceSpec->mEIWithIndexes = nullptr;
  testerInterfaceSpec->mNumEOs = 0;
  testerInterfaceSpec->mEONames = nullptr;
  testerInterfaceSpec->mEOWith = nullptr;
  testerInterfaceSpec->mEOWithIndexes = nullptr;
  testerInterfaceSpec->mNumDIs = interfaceSpec->mNumDOs;
  testerInterfaceSpec->mDINames = interfaceSpec->mDONames;
  testerInterfaceSpec->mDIDataTypeNames = interfaceSpec->mDODataTypeNames;
  testerInterfaceSpec->mNumDOs = 0;
  testerInterfaceSpec->mDONames = nullptr;
  testerInterfaceSpec->mDODataTypeNames = nullptr;
  testerInterfaceSpec->mNumDIOs = 0;
  testerInterfaceSpec->mDIONames = nullptr;
  testerInterfaceSpec->mNumAdapters = 0;
  testerInterfaceSpec->mAdapterInstanceDefinition = nullptr;

  setupFBInterface(testerInterfaceSpec);
}

void CFBTestFixtureBase::performDataInterfaceTests() {
  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  BOOST_REQUIRE_EQUAL(interfaceSpec->mNumDIs, mInputDataBuffers.size());

  BOOST_CHECK(nullptr == mFBUnderTest->getDataInput(CStringDictionary::scmInvalidStringId));
  BOOST_CHECK_EQUAL(cgInvalidPortId, mFBUnderTest->getDIID(CStringDictionary::scmInvalidStringId));

  for(TPortId i = 0; i < interfaceSpec->mNumDIs; ++i) {
    CIEC_ANY *val = mFBUnderTest->getDataInput(interfaceSpec->mDINames[i]);
    BOOST_REQUIRE(nullptr != val);
    //either we have the same datatypeid or the input is any
    BOOST_REQUIRE((mInputDataBuffers[i]->getDataTypeID() == val->getDataTypeID()) || (CIEC_ANY::e_ANY == val->getDataTypeID()));

    BOOST_CHECK_EQUAL(val, mFBUnderTest->getDIFromPortId(i));
    CStringDictionary::TStringId stringIdBuf = interfaceSpec->mDINames[i];
    BOOST_CHECK_EQUAL(val, mFBUnderTest->getVar(&stringIdBuf, 1));

    BOOST_CHECK_EQUAL(i, mFBUnderTest->getDIID(interfaceSpec->mDINames[i]));

    //we should not be able to get a data output with a data input name
    BOOST_CHECK(nullptr == mFBUnderTest->getDataOutput(interfaceSpec->mDINames[i]));
    BOOST_CHECK_EQUAL(cgInvalidPortId, mFBUnderTest->getDOID(interfaceSpec->mDINames[i]));
  }

  for(TPortId i = interfaceSpec->mNumDIs; i <= cgInvalidPortId; ++i) {
    BOOST_CHECK(nullptr == mFBUnderTest->getDIFromPortId(i));
  }

  BOOST_CHECK_EQUAL(interfaceSpec->mNumDOs, mOutputDataBuffers.size());

  BOOST_CHECK(nullptr == mFBUnderTest->getDataOutput(CStringDictionary::scmInvalidStringId));
  BOOST_CHECK_EQUAL(cgInvalidPortId, mFBUnderTest->getDOID(CStringDictionary::scmInvalidStringId));

  for(TPortId i = 0; i < interfaceSpec->mNumDOs; ++i) {
    CIEC_ANY *val = mFBUnderTest->getDataOutput(interfaceSpec->mDONames[i]);
    BOOST_REQUIRE(nullptr != val);
    //either we have the same datatypeid or the input is any
    BOOST_REQUIRE((mOutputDataBuffers[i]->getDataTypeID() == val->getDataTypeID()) || (CIEC_ANY::e_ANY == val->getDataTypeID()));

    CStringDictionary::TStringId stringIdBuf = interfaceSpec->mDONames[i];
    BOOST_CHECK_EQUAL(val, mFBUnderTest->getVar(&stringIdBuf, 1));

    BOOST_CHECK_EQUAL(i, mFBUnderTest->getDOID(interfaceSpec->mDONames[i]));

    //we should not be able to get a data out with a data output name
    BOOST_CHECK(nullptr == mFBUnderTest->getDataInput(interfaceSpec->mDONames[i]));
    BOOST_CHECK_EQUAL(cgInvalidPortId, mFBUnderTest->getDIID(interfaceSpec->mDONames[i]));
  }
}

void CFBTestFixtureBase::createEventOutputConnections() {
  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  for(TPortId i = 0; i < interfaceSpec->mNumEOs; i++) {
    CEventConnection *eventCon = mFBUnderTest->getEOConnection(interfaceSpec->mEONames[i]);
    BOOST_REQUIRE_EQUAL(EMGMResponse::Ready, eventCon->connect(this, interfaceSpec->mEONames[i]));
  }
}

void CFBTestFixtureBase::createDataInputConnections() {
  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  for(size_t i = 0; i < interfaceSpec->mNumDIs; ++i) {
    CInterface2InternalDataConnection *con = new CInterface2InternalDataConnection();
    mDIConnections.push_back(con);
    con->setValue(mInputDataBuffers[i]);
    BOOST_REQUIRE_EQUAL(EMGMResponse::Ready, con->connect(mFBUnderTest, interfaceSpec->mDINames[i]));
  }
}

void CFBTestFixtureBase::createDataOutputConnections() {
  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  for(size_t i = 0; i < interfaceSpec->mNumDOs; ++i) {
    if(CFBTestConn::canBeConnected(mOutputDataBuffers[i], mFBUnderTest->getDataOutput(interfaceSpec->mDONames[i]))) {
      CDataConnection *dataCon = mFBUnderTest->getDOConnection(interfaceSpec->mDONames[i]);
      BOOST_REQUIRE_EQUAL(EMGMResponse::Ready, dataCon->connect(this, interfaceSpec->mDONames[i]));
      std::destroy_at(dataCon->getValue());
      dataCon->setValue(mOutputDataBuffers[i]);
    }
  }
}
