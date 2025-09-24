/*******************************************************************************
 * Copyright (c) 2023, 2025 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>

#include "forte/iec61499/events/E_CTUD_fbt.h"
#include "forte/basicfb.h"

using namespace forte::literals;

#include "fbcontainermock.h"

namespace forte::test {

  BOOST_AUTO_TEST_SUITE(FUNCBLOC)

  BOOST_AUTO_TEST_CASE(FB_TO_STRING_TEST) {
    iec61499::events::FORTE_E_CTUD testFb(
        {}, CFBContainerMock::smDefaultFBContMock); // Dummy FB, do not use for anything else than testing toString
    constexpr char result[] = "(PV:=0, QU:=FALSE, QD:=FALSE, CV:=0)";
    std::string buffer;
    testFb.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, result);
  }

  BOOST_AUTO_TEST_CASE(FB_TO_STRING_BUFFER_SIZE_TEST_WITH_INRENAL_VAR) {

    // Test for FB with internal vars

    class CInternalVarTestFB : public CBasicFB {
        const SFBInterfaceSpec gcEmptyInterface = {};

      public:
        CInternalVarTestFB(std::span<const StringId> paVarInternalNames) :
            CBasicFB(CFBContainerMock::smDefaultFBContMock, gcEmptyInterface, {}, paVarInternalNames) {
        }

        CIEC_ANY *getVarInternal(size_t paVarIntNum) override {
          switch (paVarIntNum) {
            case 0: return &var_QU;
            case 1: return &var_QD;
            case 2: return &var_CV;
          }
          return nullptr;
        }

        StringId getFBTypeId() const override {
          return {};
        }

        void executeEvent(TEventID, CEventChainExecutionThread *const) override {
          // nothiing to do here
        }

        void readInputData(TEventID) override {
        }

        void writeOutputData(TEventID) override {
        }

        void setInitialValues() override {
          CBasicFB::setInitialValues();
        }

        CIEC_ANY *getDI(size_t) override {
          return nullptr;
        }

        CIEC_ANY *getDO(size_t) override {
          return nullptr;
        }

        CEventConnection *getEOConUnchecked(TPortId) override {
          return nullptr;
        }

        CDataConnection **getDIConUnchecked(TPortId) override {
          return nullptr;
        }

        CDataConnection *getDOConUnchecked(TPortId) override {
          return nullptr;
        }

      private:
        CIEC_BOOL var_QU;
        CIEC_BOOL var_QD;
        CIEC_UINT var_CV;
    };

    const auto cInternalsNames = std::array{"QU"_STRID, "QD"_STRID, "CV"_STRID};
    CInternalVarTestFB testFb(cInternalsNames);
    BOOST_REQUIRE(testFb.initialize());
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::test
