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
#include "fbtestsmanager.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "fbtestsmanager_gen.cpp"
#endif
#include "fbtester.h"
#include <boost/test/unit_test.hpp>
#include "../../src/core/device.h"
#include <stdio.h>

CFBTesterEntry::CFBTesterEntry(CStringDictionary::TStringId pa_nTypeNameId,
    TFBTesterCreateFunc pa_pfuncCreateTester) :
    CTypeLib::CTypeEntry(pa_nTypeNameId), m_pfuncCreateTester(pa_pfuncCreateTester){
  CFBTestsManager::getInstance().addFBTester(this);
}

/*******************************************************************************/
DEFINE_SINGLETON(CFBTestsManager)

CFBTestsManager::CFBTestsManager(): m_nNumSuccessfulTestCases(0), m_nNumUnsuccessfulTestCases(0){
}

CFBTestsManager::~CFBTestsManager(){
}

unsigned int CFBTestsManager::runAllTests(CResource *paRessource){
  clearTestLog();

  printf("*****************************************************\n");
  printf("*** Running all FB Tests!");
  printf("*** \n");

  for(TFBTesterList::Iterator itRunner = m_lstroFBTesterList.begin();
      itRunner != m_lstroFBTesterList.end(); ++itRunner){
    executeFBTests(*itRunner, paRessource);
  }

  printf("*****************************************************\n");
  printf("*** Overall Test Summary:\n");
  printf("*** Executed %u test cases (%u successful, %u unsuccessful)\n", m_nNumSuccessfulTestCases
      + m_nNumUnsuccessfulTestCases, m_nNumSuccessfulTestCases, m_nNumUnsuccessfulTestCases);
  printf("*****************************************************\n");

  return m_nNumUnsuccessfulTestCases;
}

void CFBTestsManager::addFBTester(CFBTesterEntry* pa_poFBTester){
  m_lstroFBTesterList.pushBack(pa_poFBTester);
}

void CFBTestsManager::executeFBTests(CFBTesterEntry* pa_poFBTestEntery, CResource *paRessource){
  CFBTester* poTester = pa_poFBTestEntery->createFBInstance(paRessource);
  poTester->executeTests();

  m_nNumSuccessfulTestCases += poTester->getNumSuccesfullTestCases();
  m_nNumUnsuccessfulTestCases += poTester->getNumUnsuccesfullTestCases();

  delete poTester;
}

void CFBTestsManager::clearTestLog(){
  m_nNumSuccessfulTestCases = 0;
  m_nNumUnsuccessfulTestCases = 0;
}

BOOST_AUTO_TEST_SUITE(FBTests)

  const SFBInterfaceSpec scm_stFBInterfaceSpec = { 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0,
      0 };

  BOOST_AUTO_TEST_CASE(runallFBTests){
    CDevice dev(&scm_stFBInterfaceSpec, CStringDictionary::scm_nInvalidStringId, 0, 0);
    //mimick the behavior provided by typelib
  dev.changeFBExecutionState(cg_nMGM_CMD_Reset);

    CResource *res = (CResource *)CTypeLib::createFB(g_nStringIdEMB_RES, g_nStringIdEMB_RES, &dev);
    BOOST_CHECK(0 != res);

    if(res != 0){
      dev.addFB(res);

      BOOST_CHECK_EQUAL(1, dev.startDevice());
      CFunctionBlock::E_FBStates state = dev.getState();
      BOOST_CHECK_EQUAL(CFunctionBlock::e_RUNNING, state);

      if(CFunctionBlock::e_RUNNING == state){
        //only run fb tests if the environment is correctly up and running
        BOOST_CHECK_EQUAL(0, CFBTestsManager::getInstance().runAllTests(res));
      }

      BOOST_CHECK_EQUAL(false, res->isCurrentlyDeleteable());
      BOOST_CHECK_EQUAL(e_RDY, dev.changeFBExecutionState(cg_nMGM_CMD_Stop));
      BOOST_CHECK_EQUAL(true, res->isCurrentlyDeleteable());
      //as we added the resource to the device's object handler it will be deleted by it
    }

  }

  BOOST_AUTO_TEST_SUITE_END()
