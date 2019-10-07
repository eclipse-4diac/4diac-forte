/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "../../src/core/funcbloc.h"

class CFunctionBlockMock : public CFunctionBlock{
  public:
    CFunctionBlockMock() :
        CFunctionBlock(0, 0, 0, 0, 0){
      //mimick the behavior provided by typelib
      changeFBExecutionState(cg_nMGM_CMD_Reset);
    }

    virtual CStringDictionary::TStringId getFBTypeId(void) const{
      return CStringDictionary::scm_nInvalidStringId;
    }

  private:
    virtual void executeEvent(int){
    }
};

BOOST_AUTO_TEST_SUITE(ManagedObjectStateMachine)

  BOOST_AUTO_TEST_CASE(idleTest){
    CFunctionBlockMock testee;

    BOOST_CHECK_EQUAL(CFunctionBlock::e_IDLE, testee.getState());

    BOOST_CHECK(testee.isCurrentlyDeleteable());

    BOOST_CHECK_EQUAL(e_INVALID_STATE, testee.changeFBExecutionState(cg_nMGM_CMD_Stop));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_IDLE, testee.getState());

    BOOST_CHECK_EQUAL(e_INVALID_STATE, testee.changeFBExecutionState(cg_nMGM_CMD_Kill));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_IDLE, testee.getState());

    BOOST_CHECK_EQUAL(e_INVALID_STATE, testee.changeFBExecutionState(cg_nMGM_CMD_Reset));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_IDLE, testee.getState());

    BOOST_CHECK_EQUAL(e_RDY, testee.changeFBExecutionState(cg_nMGM_CMD_Start));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_RUNNING, testee.getState());
  }

  void putTesteeIntoRun(CFunctionBlock &paTestee){
    paTestee.changeFBExecutionState(cg_nMGM_CMD_Start);
  }

  BOOST_AUTO_TEST_CASE(runningTest){
    CFunctionBlockMock testee;
    putTesteeIntoRun(testee);

    BOOST_CHECK_EQUAL(false, testee.isCurrentlyDeleteable());

    BOOST_CHECK_EQUAL(CFunctionBlock::e_RUNNING, testee.getState());

    //we should not be able to reset it
    BOOST_CHECK_EQUAL(e_INVALID_STATE, testee.changeFBExecutionState(cg_nMGM_CMD_Reset));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_RUNNING, testee.getState());

    //we should not be able to start it
    BOOST_CHECK_EQUAL(e_INVALID_STATE, testee.changeFBExecutionState(cg_nMGM_CMD_Start));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_RUNNING, testee.getState());

    //we should be able to stop it
    BOOST_CHECK_EQUAL(e_RDY, testee.changeFBExecutionState(cg_nMGM_CMD_Stop));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_STOPPED, testee.getState());

    //we shold be able to kill it, use a new testee to have a clean running state
    CFunctionBlockMock killTestee;
    putTesteeIntoRun(killTestee);

    BOOST_CHECK_EQUAL(e_RDY, killTestee.changeFBExecutionState(cg_nMGM_CMD_Kill));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_KILLED, killTestee.getState());
  }

  void putTesteeIntoStopped(CFunctionBlock &paTestee){
    putTesteeIntoRun(paTestee);
    paTestee.changeFBExecutionState(cg_nMGM_CMD_Stop);
  }

  BOOST_AUTO_TEST_CASE(stoppedTest){
    CFunctionBlockMock testee;
    putTesteeIntoStopped(testee);

    BOOST_CHECK(testee.isCurrentlyDeleteable());

    //we should not be able to kill it
    BOOST_CHECK_EQUAL(e_INVALID_STATE, testee.changeFBExecutionState(cg_nMGM_CMD_Kill));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_STOPPED, testee.getState());

    //we should not be able to stop it
    BOOST_CHECK_EQUAL(e_INVALID_STATE, testee.changeFBExecutionState(cg_nMGM_CMD_Stop));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_STOPPED, testee.getState());

    //we should be able to start it
    BOOST_CHECK_EQUAL(e_RDY, testee.changeFBExecutionState(cg_nMGM_CMD_Start));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_RUNNING, testee.getState());

    //we should be able to reset it, use new testeee for a clean stopped state
    CFunctionBlockMock resetTestee;
    putTesteeIntoStopped(resetTestee);

    BOOST_CHECK_EQUAL(e_RDY, resetTestee.changeFBExecutionState(cg_nMGM_CMD_Reset));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_IDLE, resetTestee.getState());

  }

  void putTesteeIntoKilled(CFunctionBlock &paTestee){
    putTesteeIntoRun(paTestee);
    paTestee.changeFBExecutionState(cg_nMGM_CMD_Kill);
  }

  BOOST_AUTO_TEST_CASE(killedTest){
    CFunctionBlockMock testee;
    putTesteeIntoKilled(testee);

    BOOST_CHECK(testee.isCurrentlyDeleteable());

    //we should not be able to kill it
    BOOST_CHECK_EQUAL(e_INVALID_STATE, testee.changeFBExecutionState(cg_nMGM_CMD_Kill));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_KILLED, testee.getState());

    //we should not be able to stop it
    BOOST_CHECK_EQUAL(e_INVALID_STATE, testee.changeFBExecutionState(cg_nMGM_CMD_Stop));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_KILLED, testee.getState());

    //we should not be able to start it
    BOOST_CHECK_EQUAL(e_INVALID_STATE, testee.changeFBExecutionState(cg_nMGM_CMD_Start));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_KILLED, testee.getState());

    //we should be able to reset it
    BOOST_CHECK_EQUAL(e_RDY, testee.changeFBExecutionState(cg_nMGM_CMD_Reset));
    BOOST_CHECK_EQUAL(CFunctionBlock::e_IDLE, testee.getState());
  }

  void testAllOtherCommands(CFunctionBlock &paTestee, CFunctionBlock::E_FBStates paState){
    for(unsigned int i = 0; i <= cg_nMGM_CMD_INVALID; i++){
      if((i != cg_nMGM_CMD_Start) && (i != cg_nMGM_CMD_Stop) && (i != cg_nMGM_CMD_Kill) && (i != cg_nMGM_CMD_Reset)){
        BOOST_CHECK_EQUAL(e_INVALID_OPERATION, paTestee.changeFBExecutionState(static_cast<EMGMCommandType>(i)));
        BOOST_CHECK_EQUAL(paState, paTestee.getState());
      }
    }
  }

  BOOST_AUTO_TEST_CASE(testOtherCommands){
    CFunctionBlockMock testee;

    //test for idle
    testAllOtherCommands(testee, CFunctionBlock::e_IDLE);

    //test for running
    testee.changeFBExecutionState(cg_nMGM_CMD_Start);
    testAllOtherCommands(testee, CFunctionBlock::e_RUNNING);

    //test for stopped
    testee.changeFBExecutionState(cg_nMGM_CMD_Stop);
    testAllOtherCommands(testee, CFunctionBlock::e_STOPPED);

    //test for killed
    testee.changeFBExecutionState(cg_nMGM_CMD_Start);
    testee.changeFBExecutionState(cg_nMGM_CMD_Kill);
    testAllOtherCommands(testee, CFunctionBlock::e_KILLED);
  }

  BOOST_AUTO_TEST_SUITE_END()
