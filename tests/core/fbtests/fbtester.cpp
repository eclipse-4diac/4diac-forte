/*******************************************************************************
 * Copyright (c) 2011 - 2015 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "fbtester.h"
#include <boost/test/unit_test.hpp>
#include "stringdict.h"
#include <stdio.h>
#include <algorithm>
#include <ecet.h>
#include <criticalregion.h>
#include <if2indco.h>

#ifdef WIN32
#include <windows.h>
#define usleep(x) Sleep((x)/1000)
#else
#include <unistd.h>
#endif
#include <stdio.h>

/**Helper functor for deleting stuff in containers
 *
 */
struct SDeleteFunctor{
    template<class T>
    void operator()(T* paVal) const{
      delete paVal;
    }
};

//! Helper class allowing to access the can be connected function from the FBTester
class CFBTestConn : public CDataConnection{
  public:
    static bool canBeConnected(const CIEC_ANY *pa_poSrcDataPoint,
        const CIEC_ANY *pa_poDstDataPoint){
      bool bSpecialCast;
      return CDataConnection::canBeConnected(pa_poSrcDataPoint, pa_poDstDataPoint, bSpecialCast);
    }
  private:
    //you should not be allowed to create this class their for constructor and destructor are private
    CFBTestConn();
    virtual ~CFBTestConn();
};

CFBTester::CFBTester(CResource *pa_poTestResource) :
    CFunctionBlock(pa_poTestResource, 0, 0, 0, 0),
        mFBUnderTest(0), m_nNumSuccesfulTestCases(0), m_nNumUnsuccesfulTestCases(0){

  changeFBExecutionState(cg_nMGM_CMD_Reset);
  changeFBExecutionState(cg_nMGM_CMD_Start);
  //assure that we are in running state
  BOOST_CHECK_EQUAL(CFunctionBlock::e_RUNNING, getState());
}

CFBTester::~CFBTester(){
}

void CFBTester::executeTests(){
  printf("*****************************************************\n");
  printf("*** Testing FB: %s\n",
      CStringDictionary::getInstance().get(getFBTypeId()));
  printf("*** \n");

  if(prepareTests()){
    evaluateTestResult(testCaseWrongInputEvent(), "Wrong Input Event");
    executeAllTests();
  }
  cleanupTestData();

  printf("***\n");
  printf("*** Test Summary:\n");
  printf("*** Executed %d test cases (%d successful, %d unsuccessful)\n",
      m_nNumSuccesfulTestCases + m_nNumUnsuccesfulTestCases,
      m_nNumSuccesfulTestCases, m_nNumUnsuccesfulTestCases);
  printf("*****************************************************\n");

}

bool CFBTester::prepareTests(){
  bool retVal = false;

  m_nNumSuccesfulTestCases = 0;
  m_nNumUnsuccesfulTestCases = 0;

  performCreationTest();

  if(0 != mFBUnderTest){
    performDataInterfaceTests();

    mFBUnderTest->changeFBExecutionState(cg_nMGM_CMD_Start);

    createEventOutputConnections();
    if(createDataInputConnections()){
      if(createDataOutputConnections()){
        retVal = true;
      }
    }
    if(false == retVal){
      //Inform test runner that we had an issue
      m_nNumUnsuccesfulTestCases++;
    }
  }

  return retVal;
}

void CFBTester::cleanupTestData(){

  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  for(TPortId i = 0; i < interfaceSpec->m_nNumEOs; i++){
    CEventConnection *eventCon = mFBUnderTest->getEOConection(interfaceSpec->m_aunEONames[i]);
    BOOST_CHECK_EQUAL(e_RDY, eventCon->disconnect(this, interfaceSpec->m_aunEONames[i]));
  }

  for(unsigned int i = 0; i < interfaceSpec->m_nNumDOs; ++i){
    CDataConnection *dataCon = mFBUnderTest->getDOConnection(interfaceSpec->m_aunDONames[i]);
    //set it to zero so that when the FB under test is deleted it will not delete our test output data
    dataCon->setValue(0);

    BOOST_CHECK_EQUAL(e_RDY, dataCon->disconnect(this, interfaceSpec->m_aunDONames[i]));
  }

  for(unsigned int i = 0; i < interfaceSpec->m_nNumDIs; ++i){
    BOOST_CHECK_EQUAL(e_RDY, mDIConnections[i]->disconnect(mFBUnderTest, interfaceSpec->m_aunDINames[i]));
  }

  for_each(mDIConnections.begin(), mDIConnections.end(), SDeleteFunctor());

  performFBDeleteTests();
}

void CFBTester::performFBDeleteTests(){
  bool bTestResult = true;

  if(0 != mFBUnderTest){
    if(mFBUnderTest->isCurrentlyDeleteable()){
      bTestResult = false;
    }

    if(e_RDY != mFBUnderTest->changeFBExecutionState(cg_nMGM_CMD_Stop)){
      bTestResult = false;
    }

    if(!mFBUnderTest->isCurrentlyDeleteable()){
      bTestResult = false;
    }

    if(!CTypeLib::deleteFB(mFBUnderTest)){
      bTestResult = false;
    }
  }
  else{
    bTestResult = false;
  }

  mFBUnderTest = 0;

  evaluateTestResult(bTestResult, "Type deletion");
}

void CFBTester::executeEvent(int pa_nEIID){
  CCriticalRegion criticalRegion(mOutputEventLock);
  mFBOutputEvents.push_back(pa_nEIID);
}

void CFBTester::triggerEvent(TPortId pa_nEIId){
  CEventChainExecutionThread *execThread = getResource().getResourceEventExecution();
  SEventEntry entry(mFBUnderTest, pa_nEIId);

  execThread->startEventChain(&entry);

  //Wait till event execution for this input event has finished
  do{
    usleep(1);
  } while(execThread->isProcessingEvents());

}

void CFBTester::performCreationTest(){
  bool testResult = false;

  mFBUnderTest = CTypeLib::createFB(getFBTypeId(), getFBTypeId(), getResourcePtr());

  if(0 != mFBUnderTest){
    if((CFunctionBlock::e_IDLE == mFBUnderTest->getState()) &&
        (getFBTypeId() == mFBUnderTest->getFBTypeId()) &&
        (getFBTypeId() == mFBUnderTest->getInstanceNameId())){
      const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

      SFBInterfaceSpec* testerInterfaceSpec = new SFBInterfaceSpec;
      testerInterfaceSpec->m_nNumEIs = interfaceSpec->m_nNumEOs;
      testerInterfaceSpec->m_aunEINames = interfaceSpec->m_aunEONames;
      testerInterfaceSpec->m_anEIWith = 0;
      testerInterfaceSpec->m_anEIWithIndexes = 0;
      testerInterfaceSpec->m_nNumEOs = 0;
      testerInterfaceSpec->m_aunEONames = 0;
      testerInterfaceSpec->m_anEOWith = 0;
      testerInterfaceSpec->m_anEOWithIndexes = 0;
      testerInterfaceSpec->m_nNumDIs = interfaceSpec->m_nNumDOs;
      testerInterfaceSpec->m_aunDINames = interfaceSpec->m_aunDONames;
      testerInterfaceSpec->m_aunDIDataTypeNames = interfaceSpec->m_aunDODataTypeNames;
      testerInterfaceSpec->m_nNumDOs = 0;
      testerInterfaceSpec->m_aunDONames = 0;
      testerInterfaceSpec->m_aunDODataTypeNames = 0;
      testerInterfaceSpec->m_nNumAdapters = 0;
      testerInterfaceSpec->m_pstAdapterInstanceDefinition = 0;

      setupFBInterface(testerInterfaceSpec,
          (0 != testerInterfaceSpec->m_nNumDIs) ?
              new TForteByte[genFBConnDataSize(testerInterfaceSpec->m_nNumEOs,
                  testerInterfaceSpec->m_nNumDIs, testerInterfaceSpec->m_nNumDOs)] :
              0,
          (0 != testerInterfaceSpec->m_nNumDIs) ?
              new TForteByte[genFBVarsDataSize(testerInterfaceSpec->m_nNumDIs, testerInterfaceSpec->m_nNumDOs)] :
              0,
          true);
      testResult = true;
    }
  }

  evaluateTestResult(testResult, "Type creation");
}

void CFBTester::evaluateTestResult(bool pa_bSuccess,
    const char* pa_acTestCaseName){
  if(pa_bSuccess){
    ++m_nNumSuccesfulTestCases;
  }
  else{
    ++m_nNumUnsuccesfulTestCases;
  }

  printf("*** Test case: %s: %s\n", pa_acTestCaseName,
      (pa_bSuccess) ? "ok" : "error");
}

int CFBTester::pullFirstChainEventID(){
  CCriticalRegion criticalRegion(mOutputEventLock);
  int retVal = mFBOutputEvents.front();
  mFBOutputEvents.pop_front();
  return retVal;
}

bool CFBTester::eventChainEmpty(){
  CCriticalRegion criticalRegion(mOutputEventLock);
  bool retVal = mFBOutputEvents.empty();
  return retVal;
}

void CFBTester::clearEventChain(){
  mFBOutputEvents.clear();
}

bool CFBTester::checkForSingleOutputEventOccurence(int pa_nExpectedEOId){
  bool bResult = (pullFirstChainEventID() == pa_nExpectedEOId);

  //Test if event chain is empty
  if(!eventChainEmpty()){
    bResult = false;
  }

  return bResult;
}

void CFBTester::setInputData(TIEC_ANYPtr paInputData[], size_t paLenght){
  mInputDataBuffers.reserve(paLenght);
  mInputDataBuffers.assign(paInputData, paInputData + paLenght);
}

void CFBTester::setOutputData(TIEC_ANYPtr paOutputData[], size_t paLenght){
  mOutputDataBuffers.reserve(paLenght);
  mOutputDataBuffers.assign(paOutputData, paOutputData + paLenght);
}

void CFBTester::createEventOutputConnections(){
  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  for(TPortId i = 0; i < interfaceSpec->m_nNumEOs; i++){
    CEventConnection *eventCon = mFBUnderTest->getEOConection(interfaceSpec->m_aunEONames[i]);
    BOOST_CHECK_EQUAL(e_RDY, eventCon->connect(this, interfaceSpec->m_aunEONames[i]));
  }
}

bool CFBTester::createDataInputConnections(){
  bool retVal = true;

  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  if(interfaceSpec->m_nNumDIs == mInputDataBuffers.size()){
    for(TPortId i = 0; i < interfaceSpec->m_nNumDIs; ++i){
      CInterface2InternalDataConnection *con = new CInterface2InternalDataConnection();
      mDIConnections.push_back(con);
      con->setValue(mInputDataBuffers[i]);
      if(e_RDY != con->connect(mFBUnderTest, interfaceSpec->m_aunDINames[i])){
        printf("*** FB test preparation: Given input data value for input %s can not be connected!\n",
            CStringDictionary::getInstance().get(interfaceSpec->m_aunDINames[i]));
        retVal = false;
        break;
      }
    }
  }
  else{
    printf(
        "*** FB test preparation: Wrong amount of input data given! Expected: %d, got: %lu\n",
        interfaceSpec->m_nNumDIs, mInputDataBuffers.size());
    retVal = false;
  }

  return retVal;
}

bool CFBTester::createDataOutputConnections(){
  bool retVal = true;

  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  if(interfaceSpec->m_nNumDOs == mOutputDataBuffers.size()){
    for(unsigned int i = 0; i < interfaceSpec->m_nNumDOs; ++i){
      if(CFBTestConn::canBeConnected(mOutputDataBuffers[i], mFBUnderTest->getDataOutput(interfaceSpec->m_aunDONames[i]))){
        CDataConnection *dataCon = mFBUnderTest->getDOConnection(interfaceSpec->m_aunDONames[i]);
        dataCon->setValue(mOutputDataBuffers[i]);
        if(e_RDY != dataCon->connect(this, interfaceSpec->m_aunDONames[i])){
          printf("*** FB test preparation: Cannot create data connection for output %s!\n",
              CStringDictionary::getInstance().get(interfaceSpec->m_aunDONames[i]));
          retVal = false;
          break;
        }
      }
      else{
        printf("*** FB test preparation: Given output data value for output %s can not be connected!\n",
            CStringDictionary::getInstance().get(interfaceSpec->m_aunDONames[i]));
        retVal = false;
        break;
      }
    }
  }
  else{
    printf("*** FB test preparation: Wrong amount of output data given! Expected: %d, got: %lu\n",
        interfaceSpec->m_nNumDOs, mOutputDataBuffers.size());
    retVal = false;
  }

  return retVal;

}

bool CFBTester::testCaseWrongInputEvent(){
  bool bResult = true;

  for(TPortId i = mFBUnderTest->getFBInterfaceSpec()->m_nNumEIs; i < cg_nExternalEventID; i++){
    triggerEvent(i);
    if(!eventChainEmpty()){
      bResult = false;
      break;
    }
  }

  return bResult;
}

void CFBTester::performDataInterfaceTests(){
  const SFBInterfaceSpec* interfaceSpec = mFBUnderTest->getFBInterfaceSpec();

  BOOST_CHECK_EQUAL(interfaceSpec->m_nNumDIs, mInputDataBuffers.size());

  BOOST_CHECK(0 == mFBUnderTest->getDataInput(CStringDictionary::scm_nInvalidStringId));
  BOOST_CHECK_EQUAL(cg_unInvalidPortId, mFBUnderTest->getDIID(CStringDictionary::scm_nInvalidStringId));

  for(TPortId i = 0; i < interfaceSpec->m_nNumDIs; ++i){
    CIEC_ANY *val = mFBUnderTest->getDataInput(interfaceSpec->m_aunDINames[i]);
    BOOST_CHECK(0 != val);
    //either we have the same datatypeid or the input is any
    BOOST_CHECK((mInputDataBuffers[i]->getDataTypeID(), val->getDataTypeID()) ||
        (CIEC_ANY::e_ANY == val->getDataTypeID()));

    BOOST_CHECK_EQUAL(val, mFBUnderTest->getDIFromPortId(i));
    CStringDictionary::TStringId stringIdBuf = interfaceSpec->m_aunDINames[i];
    BOOST_CHECK_EQUAL(val, mFBUnderTest->getVar(&stringIdBuf, 1));

    BOOST_CHECK_EQUAL(i, mFBUnderTest->getDIID(interfaceSpec->m_aunDINames[i]));

    //we should not be able to get a data output with a data input name
    BOOST_CHECK(0 == mFBUnderTest->getDataOutput(interfaceSpec->m_aunDINames[i]));
    BOOST_CHECK_EQUAL(cg_unInvalidPortId, mFBUnderTest->getDOID(interfaceSpec->m_aunDINames[i]));
  }

  for(TPortId i = interfaceSpec->m_nNumDIs; i <= cg_unInvalidPortId; ++i){
    BOOST_CHECK(0 == mFBUnderTest->getDIFromPortId(i));
  }

  BOOST_CHECK_EQUAL(interfaceSpec->m_nNumDOs, mOutputDataBuffers.size());

  BOOST_CHECK(0 == mFBUnderTest->getDataOutput(CStringDictionary::scm_nInvalidStringId));
  BOOST_CHECK_EQUAL(cg_unInvalidPortId, mFBUnderTest->getDOID(CStringDictionary::scm_nInvalidStringId));

  for(TPortId i = 0; i < interfaceSpec->m_nNumDOs; ++i){
    CIEC_ANY *val = mFBUnderTest->getDataOutput(interfaceSpec->m_aunDONames[i]);
    BOOST_CHECK(0 != val);
    //either we have the same datatypeid or the input is any
    BOOST_CHECK((mOutputDataBuffers[i]->getDataTypeID(), val->getDataTypeID()) ||
        (CIEC_ANY::e_ANY == val->getDataTypeID()));

    CStringDictionary::TStringId stringIdBuf = interfaceSpec->m_aunDONames[i];
    BOOST_CHECK_EQUAL(val, mFBUnderTest->getVar(&stringIdBuf, 1));

    BOOST_CHECK_EQUAL(i, mFBUnderTest->getDOID(interfaceSpec->m_aunDONames[i]));

    //we should not be able to get a data out with a data output name
    BOOST_CHECK(0 == mFBUnderTest->getDataInput(interfaceSpec->m_aunDONames[i]));
    BOOST_CHECK_EQUAL(cg_unInvalidPortId, mFBUnderTest->getDIID(interfaceSpec->m_aunDONames[i]));
  }
}
