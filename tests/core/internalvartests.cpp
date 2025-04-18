/*******************************************************************************
 * Copyright (c) 2018 Johannes Kepler University
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <boost/test/unit_test.hpp>
#include <basicfb.h>
#include "fbcontainermock.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CV);
USE_STRING_ID(DT);
USE_STRING_ID(QD);
USE_STRING_ID(QU);
USE_STRING_ID(UINT);

const SFBInterfaceSpec gcEmptyInterface = {0,       nullptr, nullptr, nullptr, nullptr, 0,       nullptr,
                                           nullptr, nullptr, nullptr, 0,       nullptr, nullptr, 0,
                                           nullptr, nullptr, 0,       nullptr, 0,       nullptr};

class CInternalVarTestFB : public CBasicFB {

  public:
    CInternalVarTestFB(const SInternalVarsInformation *paVarInternals) :
        CBasicFB(CFBContainerMock::smDefaultFBContMock,
                 gcEmptyInterface,
                 CStringDictionary::scmInvalidStringId,
                 paVarInternals),
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

    CStringDictionary::TStringId getFBTypeId() const override {
      return CStringDictionary::scmInvalidStringId;
    }

    void executeEvent(TEventID, CEventChainExecutionThread *const) override {
      // nothing to do here
    }

    void readInputData(TEventID) override {
    }

    void writeOutputData(TEventID) override {
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

BOOST_AUTO_TEST_CASE(checkNullInternalVarsAreAllowed) {
  // check that we can create an FB where we have a 0 internal struct which has all parts set to zero
  CStringDictionary::TStringId namelist[1] = {STRID(DT)};

  CInternalVarTestFB testFB(nullptr);
  BOOST_CHECK(nullptr == testFB.getVar(namelist, 1));
  // check that we should at least get the ECC variable
  namelist[0] = CStringDictionary::insert("!ECC");
  BOOST_CHECK(nullptr != testFB.getVar(namelist, 1));
}

BOOST_AUTO_TEST_CASE(checkEmptyInternalVarsAreAllowed) {
  // check that we can create an FB where we have a var internal struct which has all parts set to zero
  SInternalVarsInformation varData = {0, nullptr, nullptr};
  CStringDictionary::TStringId namelist[1] = {STRID(DT)};

  CInternalVarTestFB testFB(&varData);
  BOOST_CHECK(nullptr == testFB.getVar(namelist, 1));
  // check that we should at least get the ECC variable
  namelist[0] = CStringDictionary::insert("!ECC");
  BOOST_CHECK(nullptr != testFB.getVar(namelist, 1));
}

BOOST_AUTO_TEST_CASE(sampleInteralVarList) {

  CStringDictionary::TStringId varInternalNames[] = {STRID(QU), STRID(QD), STRID(CV)};
  CStringDictionary::TStringId varInternalTypeIds[] = {STRID(BOOL), STRID(BOOL), STRID(UINT)};

  SInternalVarsInformation varData{3, varInternalNames, varInternalTypeIds};

  CInternalVarTestFB testFB(&varData);
  BOOST_ASSERT(testFB.initialize());

  for (size_t i = 0; i < varData.mNumIntVars; i++) {
    CIEC_ANY *var = testFB.getVar(&(varInternalNames[i]), 1);
    BOOST_CHECK(nullptr != var);
    BOOST_CHECK_EQUAL(var, testFB.getVarInternal(static_cast<unsigned int>(i)));
  }

  BOOST_CHECK_EQUAL(CIEC_ANY::e_BOOL, testFB.getVarInternal(0)->getDataTypeID());
  BOOST_CHECK_EQUAL(CIEC_ANY::e_BOOL, testFB.getVarInternal(1)->getDataTypeID());
  BOOST_CHECK_EQUAL(CIEC_ANY::e_UINT, testFB.getVarInternal(2)->getDataTypeID());
}

BOOST_AUTO_TEST_CASE(testToStringWithInternalVariables) {

  CStringDictionary::TStringId varInternalNames[] = {STRID(QU), STRID(QD), STRID(CV)};
  CStringDictionary::TStringId varInternalTypeIds[] = {STRID(BOOL), STRID(BOOL), STRID(UINT)};

  SInternalVarsInformation varData{3, varInternalNames, varInternalTypeIds};

  CInternalVarTestFB testFB(&varData);
  BOOST_ASSERT(testFB.initialize());
  constexpr char result[] = "(QU:=FALSE, QD:=FALSE, CV:=0)";
  char buffer[50];
  BOOST_TEST(testFB.toString(buffer, sizeof(buffer)) == strlen(result));
  BOOST_TEST(buffer == result);
}

BOOST_AUTO_TEST_SUITE_END()
