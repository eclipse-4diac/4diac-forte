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

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
//don't add a space between # and include so the cmake script finds the line
#include "internalvartests_gen.cpp"
#else
# include "stringlist.h"
#endif

const SFBInterfaceSpec gcEmptyInterface = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

class CInternalVarTestFB : public CBasicFB{
  public:
    CInternalVarTestFB(const SInternalVarsInformation *paVarInternals, TForteByte *paBasicFBVarsData) :
      CBasicFB(0, &gcEmptyInterface, CStringDictionary::scm_nInvalidStringId,
            paVarInternals, 0, paBasicFBVarsData) {
    }

    CIEC_ANY *getVarInternal(unsigned int paVarIntNum){
      return CBasicFB::getVarInternal(paVarIntNum);
    }

    virtual CStringDictionary::TStringId getFBTypeId() const {
      return CStringDictionary::scm_nInvalidStringId;
    }

    virtual void executeEvent(int){
      //nothiing to do here
    }
};


BOOST_AUTO_TEST_SUITE(internal_vars)

BOOST_AUTO_TEST_CASE(checkNullInternalVarsAreAllowed){
  //check that we can create an FB where we have a 0 internal struct which has all parts set to zero
  TForteByte varsDataBuffer[10];
  CStringDictionary::TStringId namelist[1] = {g_nStringIdDT};

  CInternalVarTestFB testFB(0, varsDataBuffer);
  BOOST_CHECK(0 == testFB.getVar(namelist, 1));
  //check that we should at least get the ECC variable
  namelist[0] = CStringDictionary::getInstance().insert("$ECC");
  BOOST_CHECK(0 != testFB.getVar(namelist, 1));
}


BOOST_AUTO_TEST_CASE(checkEmptyInternalVarsAreAllowed){
  //check that we can create an FB where we have a var internal struct which has all parts set to zero
  SInternalVarsInformation varData = {0,0,0};
  TForteByte varsDataBuffer[10];
  CStringDictionary::TStringId namelist[1] = {g_nStringIdDT};

  CInternalVarTestFB testFB(&varData, varsDataBuffer);
  BOOST_CHECK(0 == testFB.getVar(namelist, 1));
  //check that we should at least get the ECC variable
  namelist[0] = CStringDictionary::getInstance().insert("$ECC");
  BOOST_CHECK(0 != testFB.getVar(namelist, 1));
}

BOOST_AUTO_TEST_CASE(sampleInteralVarList){

  CStringDictionary::TStringId varInternalNames[] = {g_nStringIdQU, g_nStringIdQD, g_nStringIdCV};
  CStringDictionary::TStringId varInternalTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUINT};

  SInternalVarsInformation varData{3, varInternalNames, varInternalTypeIds};
  TForteByte varsDataBuffer[CBasicFB::genBasicFBVarsDataSizeTemplate<0, 0, 3, 0>::value];

  CInternalVarTestFB testFB(&varData, varsDataBuffer);

  for(size_t i = 0; i < varData.m_nNumIntVars; i++){
    CIEC_ANY *var = testFB.getVar(&(varInternalNames[i]), 1);
    BOOST_CHECK(0 != var);
    BOOST_CHECK_EQUAL(var, testFB.getVarInternal(static_cast<unsigned int>(i)));
  }

  BOOST_CHECK_EQUAL(CIEC_ANY::e_BOOL, testFB.getVarInternal(0)->getDataTypeID());
  BOOST_CHECK_EQUAL(CIEC_ANY::e_BOOL, testFB.getVarInternal(1)->getDataTypeID());
  BOOST_CHECK_EQUAL(CIEC_ANY::e_UINT, testFB.getVarInternal(2)->getDataTypeID());
}


BOOST_AUTO_TEST_SUITE_END()


