/*******************************************************************************
 * Copyright (c) 2018, 2025 Johannes Kepler University,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Alois Zoitl - migrated data type toString to std::string
 *******************************************************************************/

#include <boost/test/unit_test.hpp>
#include "forte/basicfb.h"
#include "fbcontainermock.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_uint.h"

using namespace forte::literals;

namespace forte::test {

  const SFBInterfaceSpec gcEmptyInterface = {};

  class CInternalVarTestFB : public CBasicFB {

    public:
      CInternalVarTestFB(std::span<const StringId> paVarInternalNames) :
          CBasicFB(CFBContainerMock::smDefaultFBContMock, gcEmptyInterface, {}, paVarInternalNames),
          var_QU(false_BOOL),
          var_QD(false_BOOL),
          var_CV(0_UINT) {
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
        // nothing to do here
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

  BOOST_AUTO_TEST_SUITE(internal_vars)

  BOOST_AUTO_TEST_CASE(checkEmptyInternalVarsAreAllowed) {
    // check that we can create an FB where we have a var internal struct which has all parts set to zero
    StringId namelist[1] = {"DT"_STRID};

    CInternalVarTestFB testFB({});
    BOOST_CHECK(nullptr == testFB.getVar(namelist));
    // check that we should at least get the ECC variable
    namelist[0] = StringId::insert("!ECC");
    BOOST_CHECK(nullptr != testFB.getVar(namelist));
  }

  BOOST_AUTO_TEST_CASE(sampleInteralVarList) {
    auto varInternalNames = std::array{"QU"_STRID, "QD"_STRID, "CV"_STRID};

    CInternalVarTestFB testFB(varInternalNames);
    BOOST_REQUIRE(testFB.initialize());

    for (size_t i = 0; i < varInternalNames.size(); i++) {
      CIEC_ANY *var = testFB.getVar(std::array{varInternalNames[i]});
      BOOST_CHECK(nullptr != var);
      BOOST_CHECK_EQUAL(var, testFB.getVarInternal(static_cast<unsigned int>(i)));
    }

    BOOST_CHECK_EQUAL(CIEC_ANY::e_BOOL, testFB.getVarInternal(0)->getDataTypeID());
    BOOST_CHECK_EQUAL(CIEC_ANY::e_BOOL, testFB.getVarInternal(1)->getDataTypeID());
    BOOST_CHECK_EQUAL(CIEC_ANY::e_UINT, testFB.getVarInternal(2)->getDataTypeID());
  }

  BOOST_AUTO_TEST_CASE(testToStringWithInternalVariables) {
    auto varInternalNames = std::array{"QU"_STRID, "QD"_STRID, "CV"_STRID};

    CInternalVarTestFB testFB(varInternalNames);
    BOOST_REQUIRE(testFB.initialize());
    constexpr char result[] = "(QU:=FALSE, QD:=FALSE, CV:=0)";
    std::string buffer;
    testFB.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, result);
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::test
