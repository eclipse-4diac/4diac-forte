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
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Alois Zoitl - migrated fb tests to boost test infrastructure
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

#define SETUP_INPUTDATA(...)                                                \
  {                                                                         \
    TIEC_ANYPtr inputData[] = {__VA_ARGS__};                                \
    setInputData(inputData, (sizeof(inputData) / sizeof(inputData[0])) );   \
  }                                                                         \


#define SETUP_OUTPUTDATA(...)                                                \
  {                                                                          \
    TIEC_ANYPtr outputData[] = {__VA_ARGS__};                                \
    setOutputData(outputData, (sizeof(outputData) / sizeof(outputData[0])) ); \
  }                                                                          \


class CFBTestFixtureBase : public CFunctionBlock{
  public:
    ~CFBTestFixtureBase();

    CStringDictionary::TStringId getFBTypeId(void) const {
      return mTypeId;
    }

  protected:
    explicit CFBTestFixtureBase(CStringDictionary::TStringId paTypeId);

    void setup();

    /*!\brief invoke the FB under Test with the given event id
     *
     * @param paEIId input event id to be sent to the fb under test
     */
    void triggerEvent(TPortId paEIId);

    int pullFirstChainEventID();

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
    bool checkForSingleOutputEventOccurence(int paExpectedEOId);

    void setInputData(TIEC_ANYPtr paInputData[], size_t paLenght);
    void setOutputData(TIEC_ANYPtr paInputData[], size_t paLenght);

  private:
    virtual void executeEvent(int paEIID);

    void setupTestInterface();
    void performFBDeleteTests();

    /* Check if all data inputs and data outputs as given in the interface struct can be accessed and
     * if the data types are equal to the data provided by the fb tester
     */
    void performDataInterfaceTests();

    void createEventOutputConnections();
    void createDataInputConnections();
    void createDataOutputConnections();

    CStringDictionary::TStringId mTypeId;
    CFunctionBlock *mFBUnderTest;
    std::vector<CInterface2InternalDataConnection *> mDIConnections;

    /*! \brief list for storing the output events received from the testee
     *
     * TODO add timestamps to the list
     */
    std::deque<int> mFBOutputEvents;

    CSyncObject mOutputEventLock;

    std::vector<CIEC_ANY *> mInputDataBuffers;
    std::vector<CIEC_ANY *> mOutputDataBuffers;

    TForteByte *mFBConnData;
    TForteByte *mFBVarsData;

};



#endif /* TESTS_CORE_FBTESTS_FBTESTFIXTURE_H_ */
