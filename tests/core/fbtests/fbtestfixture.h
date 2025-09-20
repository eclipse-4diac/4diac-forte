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

#include "forte/datatypes/forte_any.h"
#include "forte/genfb.h"
#include "forte/arch/forte_sync.h"
#include <boost/test/unit_test.hpp>
#include <vector>
#include <deque>

#include "forte_boost_output_support.h"

namespace forte::test {
  class CFBTestFixtureBase : public CGenFunctionBlock<CFunctionBlock> {
    public:
      ~CFBTestFixtureBase();

      bool initialize() override;

      StringId getFBTypeId() const override {
        return mTypeId;
      }

    protected:
      explicit CFBTestFixtureBase(StringId paTypeId);

      void setup();

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

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

      void setInputData(std::initializer_list<CIEC_ANY *> paInputData);
      void setOutputData(std::initializer_list<CIEC_ANY *> paOutputData);

      void createGenInputData() override;
      CIEC_ANY *getDI(size_t paIndex) override {
        return mGenDIs[paIndex].get();
      }

    private:
      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

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

      StringId mTypeId;
      std::string mConfigString;
      CFunctionBlock *mFBUnderTest;
      std::vector<std::unique_ptr<CDataConnection>> mDIConnections;
      std::vector<std::unique_ptr<CIEC_ANY>> mGenDIs;

      /*! \brief list for storing the output events received from the testee
       *
       * TODO add timestamps to the list
       */
      std::deque<TEventID> mFBOutputEvents;

      arch::CSyncObject mOutputEventLock;

      std::vector<CIEC_ANY *> mInputDataBuffers;
      std::vector<CIEC_ANY *> mOutputDataBuffers;
  };
} // namespace forte::test
#endif /* TESTS_CORE_FBTESTS_FBTESTFIXTURE_H_ */
