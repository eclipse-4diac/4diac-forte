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
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Alois Zoitl - migrated fb tests to boost test infrastructure
 *   Martin Jobst - add reset tests
 *******************************************************************************/
#ifndef TESTS_CORE_FBTESTS_FBTESTFIXTURE_H_
#define TESTS_CORE_FBTESTS_FBTESTFIXTURE_H_

#include <fortenew.h>
#include <funcbloc.h>
#include <if2indco.h>
#include <forte_sync.h>
#include <boost/test/unit_test.hpp>
#include <vector>
#include <deque>

#include "forte_boost_output_support.h"

class CFBTestFixtureBase : public CFunctionBlock{
  public:
    ~CFBTestFixtureBase();

    bool initialize() override;

    CStringDictionary::TStringId getFBTypeId() const {
      return mTypeId;
    }

  protected:
    explicit CFBTestFixtureBase(CStringDictionary::TStringId paTypeId);

    void setup(const char* paConfigString = nullptr);

    /*!\brief invoke the FB under Test with the given event id
     *
     * @param paEIId input event id to be sent to the fb under test
     */
    void triggerEvent(TPortId paEIId);

    TEventID pullFirstChainEventID();

    bool eventChainEmpty();

    /*!\brief Remove all events from the event chain bringing the tester into a defined state
     *
     */
    void clearEventChain();

    /** Check if only one output event of the given event output Id has been sent by the FB
     *
     * @param paExpectedEOId the output event Id to be checked for
     * @return true if only one event of the given Id is in the event queue
     */
    bool checkForSingleOutputEventOccurence(TEventID paExpectedEOId);

    void setInputData(std::initializer_list<TIEC_ANYPtr> paInputData);
    void setOutputData(std::initializer_list<TIEC_ANYPtr> paOutputData);

  private:
    void executeEvent(TEventID paEIID) override;

    void readInputData(TEventID) override {
    }

    void writeOutputData(TEventID) override {
    }

    void setupTestInterface();
    void performFBResetTests();
    void performFBDeleteTests();

    /* Check if all data inputs and data outputs as given in the interface struct can be accessed and
     * if the data types are equal to the data provided by the fb tester
     */
    void performDataInterfaceTests();

    void createEventOutputConnections();
    void createDataInputConnections();
    void createDataOutputConnections();

    CStringDictionary::TStringId mTypeId;
    std::string mConfigString;
    CFunctionBlock *mFBUnderTest;
    std::vector<CInterface2InternalDataConnection *> mDIConnections;

    /*! \brief list for storing the output events received from the testee
     *
     * TODO add timestamps to the list
     */
    std::deque<TEventID> mFBOutputEvents;

    CSyncObject mOutputEventLock;

    std::vector<CIEC_ANY *> mInputDataBuffers;
    std::vector<CIEC_ANY *> mOutputDataBuffers;
};



#endif /* TESTS_CORE_FBTESTS_FBTESTFIXTURE_H_ */
