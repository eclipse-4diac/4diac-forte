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
#include "forte_boost_output_support.h"
#include "../../src/core/funcbloc.h"

class CFunctionBlockMock : public CFunctionBlock{
  public:
    CFunctionBlockMock() :
        CFunctionBlock(nullptr, nullptr, 0){
    }

    bool initialize() override {
      if(!CFunctionBlock::initialize()) {
        return false;
      }
      //mimick the behavior provided by typelib
      changeFBExecutionState(EMGMCommandType::Reset);
      return true;
    }

    CStringDictionary::TStringId getFBTypeId() const override {
      return CStringDictionary::scmInvalidStringId;
    }

  private:
    void executeEvent(TEventID) override {
    }

    void readInputData(TEventID) override{
    }

    void writeOutputData(TEventID) override{
    }
};

BOOST_AUTO_TEST_SUITE(ManagedObjectStateMachine)

  BOOST_AUTO_TEST_CASE(idleTest){
    CFunctionBlockMock testee;
    BOOST_ASSERT(testee.initialize());

    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Idle, testee.getState());

    BOOST_CHECK(testee.isCurrentlyDeleteable());

    BOOST_CHECK_EQUAL(EMGMResponse::InvalidState, testee.changeFBExecutionState(EMGMCommandType::Stop));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Idle, testee.getState());

    BOOST_CHECK_EQUAL(EMGMResponse::InvalidState, testee.changeFBExecutionState(EMGMCommandType::Kill));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Idle, testee.getState());

    BOOST_CHECK_EQUAL(EMGMResponse::InvalidState, testee.changeFBExecutionState(EMGMCommandType::Reset));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Idle, testee.getState());

    BOOST_CHECK_EQUAL(EMGMResponse::Ready, testee.changeFBExecutionState(EMGMCommandType::Start));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Running, testee.getState());
  }

  void putTesteeIntoRun(CFunctionBlock &paTestee){
    paTestee.changeFBExecutionState(EMGMCommandType::Start);
  }

  BOOST_AUTO_TEST_CASE(runningTest){
    CFunctionBlockMock testee;
    BOOST_ASSERT(testee.initialize());
    putTesteeIntoRun(testee);

    BOOST_CHECK_EQUAL(false, testee.isCurrentlyDeleteable());

    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Running, testee.getState());

    //we should not be able to reset it
    BOOST_CHECK_EQUAL(EMGMResponse::InvalidState, testee.changeFBExecutionState(EMGMCommandType::Reset));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Running, testee.getState());

    //we should not be able to start it
    BOOST_CHECK_EQUAL(EMGMResponse::InvalidState, testee.changeFBExecutionState(EMGMCommandType::Start));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Running, testee.getState());

    //we should be able to stop it
    BOOST_CHECK_EQUAL(EMGMResponse::Ready, testee.changeFBExecutionState(EMGMCommandType::Stop));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Stopped, testee.getState());

    //we shold be able to kill it, use a new testee to have a clean running state
    CFunctionBlockMock killTestee;
    BOOST_ASSERT(killTestee.initialize());
    putTesteeIntoRun(killTestee);

    BOOST_CHECK_EQUAL(EMGMResponse::Ready, killTestee.changeFBExecutionState(EMGMCommandType::Kill));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Killed, killTestee.getState());
  }

  void putTesteeIntoStopped(CFunctionBlock &paTestee){
    putTesteeIntoRun(paTestee);
    paTestee.changeFBExecutionState(EMGMCommandType::Stop);
  }

  BOOST_AUTO_TEST_CASE(stoppedTest){
    CFunctionBlockMock testee;
    BOOST_ASSERT(testee.initialize());
    putTesteeIntoStopped(testee);

    BOOST_CHECK(testee.isCurrentlyDeleteable());

    //we should not be able to kill it
    BOOST_CHECK_EQUAL(EMGMResponse::InvalidState, testee.changeFBExecutionState(EMGMCommandType::Kill));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Stopped, testee.getState());

    //we should not be able to stop it
    BOOST_CHECK_EQUAL(EMGMResponse::InvalidState, testee.changeFBExecutionState(EMGMCommandType::Stop));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Stopped, testee.getState());

    //we should be able to start it
    BOOST_CHECK_EQUAL(EMGMResponse::Ready, testee.changeFBExecutionState(EMGMCommandType::Start));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Running, testee.getState());

    //we should be able to reset it, use new testeee for a clean stopped state
    CFunctionBlockMock resetTestee;
    BOOST_ASSERT(resetTestee.initialize());
    putTesteeIntoStopped(resetTestee);

    BOOST_CHECK_EQUAL(EMGMResponse::Ready, resetTestee.changeFBExecutionState(EMGMCommandType::Reset));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Idle, resetTestee.getState());

  }

  void putTesteeIntoKilled(CFunctionBlock &paTestee){
    putTesteeIntoRun(paTestee);
    paTestee.changeFBExecutionState(EMGMCommandType::Kill);
  }

  BOOST_AUTO_TEST_CASE(killedTest){
    CFunctionBlockMock testee;
    BOOST_ASSERT(testee.initialize());
    putTesteeIntoKilled(testee);

    BOOST_CHECK(testee.isCurrentlyDeleteable());

    //we should not be able to kill it
    BOOST_CHECK_EQUAL(EMGMResponse::InvalidState, testee.changeFBExecutionState(EMGMCommandType::Kill));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Killed, testee.getState());

    //we should not be able to stop it
    BOOST_CHECK_EQUAL(EMGMResponse::InvalidState, testee.changeFBExecutionState(EMGMCommandType::Stop));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Killed, testee.getState());

    //we should not be able to start it
    BOOST_CHECK_EQUAL(EMGMResponse::InvalidState, testee.changeFBExecutionState(EMGMCommandType::Start));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Killed, testee.getState());

    //we should be able to reset it
    BOOST_CHECK_EQUAL(EMGMResponse::Ready, testee.changeFBExecutionState(EMGMCommandType::Reset));
    BOOST_CHECK_EQUAL(CFunctionBlock::E_FBStates::Idle, testee.getState());
  }

  void testAllOtherCommands(CFunctionBlock &paTestee, CFunctionBlock::E_FBStates paState){
    for(unsigned int i = 0; i <= static_cast<unsigned int>(EMGMCommandType::INVALID); i++) {
      auto type = static_cast<EMGMCommandType>(i);
      if ((type != EMGMCommandType::Start) && (type != EMGMCommandType::Stop) && (type != EMGMCommandType::Kill) &&
          (type != EMGMCommandType::Reset)) {
        BOOST_CHECK_EQUAL(EMGMResponse::InvalidOperation, paTestee.changeFBExecutionState(type));
        BOOST_CHECK_EQUAL(paState, paTestee.getState());
      }
    }
  }

  BOOST_AUTO_TEST_CASE(testOtherCommands){
    CFunctionBlockMock testee;
    BOOST_ASSERT(testee.initialize());

    //test for idle
    testAllOtherCommands(testee, CFunctionBlock::E_FBStates::Idle);

    //test for running
    testee.changeFBExecutionState(EMGMCommandType::Start);
    testAllOtherCommands(testee, CFunctionBlock::E_FBStates::Running);

    //test for stopped
    testee.changeFBExecutionState(EMGMCommandType::Stop);
    testAllOtherCommands(testee, CFunctionBlock::E_FBStates::Stopped);

    //test for killed
    testee.changeFBExecutionState(EMGMCommandType::Start);
    testee.changeFBExecutionState(EMGMCommandType::Kill);
    testAllOtherCommands(testee, CFunctionBlock::E_FBStates::Killed);
  }

  BOOST_AUTO_TEST_SUITE_END()
