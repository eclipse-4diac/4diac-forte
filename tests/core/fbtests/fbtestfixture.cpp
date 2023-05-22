/*******************************************************************************
 * Copyright (c) 2011 - 2014, 2018 ACIN, fortiss GmbH
 *               2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *   Alois Zoitl - migrated fb tests to boost test infrastructure
 *******************************************************************************/
#include "fbtestfixture.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "fbtestfixture_gen.cpp"
#endif
#include "fbtesterglobalfixture.h"
#include <device.h>
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
    CFunctionBlock(CFBTestDataGlobalFixture::getResource(), nullptr, 0), mTypeId(paTypeId),
        mFBUnderTest(CTypeLib::createFB(paTypeId, paTypeId, getResourcePtr())) {
}

bool CFBTestFixtureBase::initialize() {
  if(!CFunctionBlock::initialize()) {
    return false;
  }
  changeFBExecutionState(EMGMCommandType::Reset);
  changeFBExecutionState(EMGMCommandType::Start);
  //assure that we are in running state
  BOOST_REQUIRE_EQUAL(CFunctionBlock::E_FBStates::Running, getState());
  BOOST_REQUIRE(nullptr != mFBUnderTest);
  return true;
}

CFBTestFixtureBase::~CFBTestFixtureBase(){
  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  for(size_t i = 0; i < interfaceSpec->m_nNumEOs; i++) {
   CEventConnection *eventCon = mFBUnderTest->getEOConnection(interfaceSpec->m_aunEONames[i]);
   BOOST_CHECK_EQUAL(EMGMResponse::Ready, eventCon->disconnect(this, interfaceSpec->m_aunEONames[i]));
  }

  for(size_t i = 0; i < interfaceSpec->m_nNumDOs; ++i) {
   CDataConnection *dataCon = mFBUnderTest->getDOConnection(interfaceSpec->m_aunDONames[i]);
   //set it to zero so that when the FB under test is deleted it will not delete our test output data
   dataCon->setValue(nullptr);

   BOOST_CHECK_EQUAL(EMGMResponse::Ready, dataCon->disconnect(this, interfaceSpec->m_aunDONames[i]));
  }

  for(size_t i = 0; i < interfaceSpec->m_nNumDIs; ++i) {
   BOOST_CHECK_EQUAL(EMGMResponse::Ready, mDIConnections[i]->disconnect(mFBUnderTest, interfaceSpec->m_aunDINames[i]));
  }

  for_each(mDIConnections.begin(), mDIConnections.end(), SDeleteFunctor());

  performFBDeleteTests();

  if(nullptr != m_pstInterfaceSpec){
    freeAllData();  //clean the interface and connections first.
    delete m_pstInterfaceSpec;
    m_pstInterfaceSpec = nullptr; //this stops the base classes from any wrong clean-up
  }
}

void CFBTestFixtureBase::performFBDeleteTests() {
  BOOST_CHECK(!mFBUnderTest->isCurrentlyDeleteable());

  BOOST_CHECK_EQUAL(EMGMResponse::Ready, mFBUnderTest->changeFBExecutionState(EMGMCommandType::Stop));

  BOOST_CHECK(mFBUnderTest->isCurrentlyDeleteable());

  BOOST_CHECK(CTypeLib::deleteFB(mFBUnderTest));
  mFBUnderTest = nullptr;
}

void CFBTestFixtureBase::setup(const char* pa_acConfigString){
  if(pa_acConfigString != nullptr) {
    mFBUnderTest->configureFB(pa_acConfigString);
  }
  BOOST_ASSERT(initialize());

  setupTestInterface();
  performDataInterfaceTests();

  mFBUnderTest->changeFBExecutionState(EMGMCommandType::Start);

  createEventOutputConnections();
  createDataInputConnections();
  createDataOutputConnections();
}

void CFBTestFixtureBase::executeEvent(TEventID paEIID){
  CCriticalRegion criticalRegion(mOutputEventLock);
  mFBOutputEvents.push_back(paEIID);
}

void CFBTestFixtureBase::triggerEvent(TPortId paEIId) {
  CEventChainExecutionThread *execThread = getResource().getResourceEventExecution();
  TEventEntry entry(mFBUnderTest, paEIId);

  execThread->startEventChain(entry);

  //Wait till event execution for this input event has finished
  do {
    usleep(1);
  } while(execThread->isProcessingEvents());

}

int CFBTestFixtureBase::pullFirstChainEventID() {
  CCriticalRegion criticalRegion(mOutputEventLock);
  int retVal = mFBOutputEvents.front();
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

bool CFBTestFixtureBase::checkForSingleOutputEventOccurence(int paExpectedEOId) {
  //Test if event chain is not empty, that the first entry is the expected one and that no furhter events are there
  return (!eventChainEmpty() && (pullFirstChainEventID() == paExpectedEOId) && eventChainEmpty());
}

void CFBTestFixtureBase::setInputData(std::initializer_list<TIEC_ANYPtr> paInputData){
  mInputDataBuffers.assign(paInputData);
}

void CFBTestFixtureBase::setOutputData(std::initializer_list<TIEC_ANYPtr> paOutputData){
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
  testerInterfaceSpec->m_nNumEIs = interfaceSpec->m_nNumEOs;
  testerInterfaceSpec->m_aunEINames = interfaceSpec->m_aunEONames;
  testerInterfaceSpec->m_anEIWith = nullptr;
  testerInterfaceSpec->m_anEIWithIndexes = nullptr;
  testerInterfaceSpec->m_nNumEOs = 0;
  testerInterfaceSpec->m_aunEONames = nullptr;
  testerInterfaceSpec->m_anEOWith = nullptr;
  testerInterfaceSpec->m_anEOWithIndexes = nullptr;
  testerInterfaceSpec->m_nNumDIs = interfaceSpec->m_nNumDOs;
  testerInterfaceSpec->m_aunDINames = interfaceSpec->m_aunDONames;
  testerInterfaceSpec->m_aunDIDataTypeNames = interfaceSpec->m_aunDODataTypeNames;
  testerInterfaceSpec->m_nNumDOs = 0;
  testerInterfaceSpec->m_aunDONames = nullptr;
  testerInterfaceSpec->m_aunDODataTypeNames = nullptr;
  testerInterfaceSpec->m_nNumAdapters = 0;
  testerInterfaceSpec->m_pstAdapterInstanceDefinition = nullptr;

  setupFBInterface(testerInterfaceSpec);
}

void CFBTestFixtureBase::performDataInterfaceTests() {
  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  BOOST_REQUIRE_EQUAL(interfaceSpec->m_nNumDIs, mInputDataBuffers.size());

  BOOST_CHECK(nullptr == mFBUnderTest->getDataInput(CStringDictionary::scm_nInvalidStringId));
  BOOST_CHECK_EQUAL(cg_unInvalidPortId, mFBUnderTest->getDIID(CStringDictionary::scm_nInvalidStringId));

  for(TPortId i = 0; i < interfaceSpec->m_nNumDIs; ++i) {
    CIEC_ANY *val = mFBUnderTest->getDataInput(interfaceSpec->m_aunDINames[i]);
    BOOST_REQUIRE(nullptr != val);
    //either we have the same datatypeid or the input is any
    BOOST_REQUIRE((mInputDataBuffers[i]->getDataTypeID() == val->getDataTypeID()) || (CIEC_ANY::e_ANY == val->getDataTypeID()));

    BOOST_CHECK_EQUAL(val, mFBUnderTest->getDIFromPortId(i));
    CStringDictionary::TStringId stringIdBuf = interfaceSpec->m_aunDINames[i];
    BOOST_CHECK_EQUAL(val, mFBUnderTest->getVar(&stringIdBuf, 1));

    BOOST_CHECK_EQUAL(i, mFBUnderTest->getDIID(interfaceSpec->m_aunDINames[i]));

    //we should not be able to get a data output with a data input name
    BOOST_CHECK(nullptr == mFBUnderTest->getDataOutput(interfaceSpec->m_aunDINames[i]));
    BOOST_CHECK_EQUAL(cg_unInvalidPortId, mFBUnderTest->getDOID(interfaceSpec->m_aunDINames[i]));
  }

  for(TPortId i = interfaceSpec->m_nNumDIs; i <= cg_unInvalidPortId; ++i) {
    BOOST_CHECK(nullptr == mFBUnderTest->getDIFromPortId(i));
  }

  BOOST_CHECK_EQUAL(interfaceSpec->m_nNumDOs, mOutputDataBuffers.size());

  BOOST_CHECK(nullptr == mFBUnderTest->getDataOutput(CStringDictionary::scm_nInvalidStringId));
  BOOST_CHECK_EQUAL(cg_unInvalidPortId, mFBUnderTest->getDOID(CStringDictionary::scm_nInvalidStringId));

  for(TPortId i = 0; i < interfaceSpec->m_nNumDOs; ++i) {
    CIEC_ANY *val = mFBUnderTest->getDataOutput(interfaceSpec->m_aunDONames[i]);
    BOOST_REQUIRE(nullptr != val);
    //either we have the same datatypeid or the input is any
    BOOST_REQUIRE((mOutputDataBuffers[i]->getDataTypeID() == val->getDataTypeID()) || (CIEC_ANY::e_ANY == val->getDataTypeID()));

    CStringDictionary::TStringId stringIdBuf = interfaceSpec->m_aunDONames[i];
    BOOST_CHECK_EQUAL(val, mFBUnderTest->getVar(&stringIdBuf, 1));

    BOOST_CHECK_EQUAL(i, mFBUnderTest->getDOID(interfaceSpec->m_aunDONames[i]));

    //we should not be able to get a data out with a data output name
    BOOST_CHECK(nullptr == mFBUnderTest->getDataInput(interfaceSpec->m_aunDONames[i]));
    BOOST_CHECK_EQUAL(cg_unInvalidPortId, mFBUnderTest->getDIID(interfaceSpec->m_aunDONames[i]));
  }
}

void CFBTestFixtureBase::createEventOutputConnections() {
  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  for(TPortId i = 0; i < interfaceSpec->m_nNumEOs; i++) {
    CEventConnection *eventCon = mFBUnderTest->getEOConnection(interfaceSpec->m_aunEONames[i]);
    BOOST_REQUIRE_EQUAL(EMGMResponse::Ready, eventCon->connect(this, interfaceSpec->m_aunEONames[i]));
  }
}

void CFBTestFixtureBase::createDataInputConnections() {
  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  for(size_t i = 0; i < interfaceSpec->m_nNumDIs; ++i) {
    CInterface2InternalDataConnection *con = new CInterface2InternalDataConnection();
    mDIConnections.push_back(con);
    con->setValue(mInputDataBuffers[i]);
    BOOST_REQUIRE_EQUAL(EMGMResponse::Ready, con->connect(mFBUnderTest, interfaceSpec->m_aunDINames[i]));
  }
}

void CFBTestFixtureBase::createDataOutputConnections() {
  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  for(size_t i = 0; i < interfaceSpec->m_nNumDOs; ++i) {
    if(CFBTestConn::canBeConnected(mOutputDataBuffers[i], mFBUnderTest->getDataOutput(interfaceSpec->m_aunDONames[i]))) {
      CDataConnection *dataCon = mFBUnderTest->getDOConnection(interfaceSpec->m_aunDONames[i]);
      BOOST_REQUIRE_EQUAL(EMGMResponse::Ready, dataCon->connect(this, interfaceSpec->m_aunDONames[i]));
      std::destroy_at(dataCon->getValue());
      dataCon->setValue(mOutputDataBuffers[i]);
    }
  }
}
