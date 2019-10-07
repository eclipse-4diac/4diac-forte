/*******************************************************************************
 * Copyright (c) 2013 - 2014 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Monika Wenger - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "../../../src/core/datatypes/forte_any.h"

BOOST_AUTO_TEST_SUITE(CIEC_ARRAY_function_test)
  BOOST_AUTO_TEST_CASE(Type_test){
    CIEC_ANY *poTest = CIEC_ANY::createDataType(0);
    BOOST_CHECK_EQUAL(poTest->getDataTypeID(), CIEC_ANY::e_ANY);
    delete poTest;
  }

  BOOST_AUTO_TEST_CASE(Conversion_test){
    CIEC_ANY *poTest = CIEC_ANY::createDataType(0);
    char cBuffer[20];
    BOOST_CHECK_EQUAL(poTest->fromString("Any string is invalid in this case"), -1);
    BOOST_CHECK_EQUAL(poTest->toString(cBuffer, 20), strlen("ND (ANY)"));
    BOOST_CHECK_EQUAL(0, strcmp(cBuffer, "ND (ANY)"));
    delete poTest;
  }

  BOOST_AUTO_TEST_CASE(Any_Typisation_negativtest){
    CIEC_ANY *poTest = CIEC_ANY::createDataType(0);

    BOOST_CHECK_EQUAL(poTest->fromString("Hansi#Any string is invalid in this case"), -1);
    BOOST_CHECK_EQUAL(poTest->getDataTypeID(), CIEC_ANY::e_ANY);

    BOOST_CHECK_EQUAL(poTest->fromString("E_RESTART#Any string is invalid in this case"), -1);
    BOOST_CHECK_EQUAL(poTest->getDataTypeID(), CIEC_ANY::e_ANY);
    delete poTest;
  }

  void checkTypisation(CIEC_ANY *pa_roTestee, const char* pa_acString, CIEC_ANY::EDataTypeID pa_eTypeId){
    BOOST_CHECK_EQUAL(pa_roTestee->fromString(pa_acString), strlen(pa_acString));
    BOOST_CHECK_EQUAL(pa_roTestee->getDataTypeID(), pa_eTypeId);
  }

  BOOST_AUTO_TEST_CASE(Any_Typisation_test){
    CIEC_ANY *poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "BOOL#TRUE", CIEC_ANY::e_BOOL);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "SINT#123", CIEC_ANY::e_SINT);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "INT#5754", CIEC_ANY::e_INT);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "DINT#112323", CIEC_ANY::e_DINT);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "USINT#255", CIEC_ANY::e_USINT);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "UINT#25754", CIEC_ANY::e_UINT);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "UDINT#3112323", CIEC_ANY::e_UDINT);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "BYTE#253", CIEC_ANY::e_BYTE);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "WORD#25754", CIEC_ANY::e_WORD);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "DWORD#3112323", CIEC_ANY::e_DWORD);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "DATE#2008-04-03", CIEC_ANY::e_DATE);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "TIME_OF_DAY#15:36:55.36", CIEC_ANY::e_TIME_OF_DAY);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "DATE_AND_TIME#1994-06-22-14:23:54.8", CIEC_ANY::e_DATE_AND_TIME);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "TIME#311ms", CIEC_ANY::e_TIME);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "REAL#3.125", CIEC_ANY::e_REAL);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "LREAL#3.1251234", CIEC_ANY::e_LREAL);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "STRING#Hallo FORTE user!", CIEC_ANY::e_STRING);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "WSTRING#Hallo FORTE user!", CIEC_ANY::e_WSTRING);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "LINT#123123123123", CIEC_ANY::e_LINT);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "ULINT#123123123123123", CIEC_ANY::e_ULINT);
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkTypisation(poTestee, "LWORD#123123123123123", CIEC_ANY::e_LWORD);
    delete poTestee;

  }

  void checkSaveWrongTypisation(CIEC_ANY &pa_roTestee, const char* pa_acString){
    BOOST_CHECK_EQUAL(pa_roTestee.fromString(pa_acString), -1);
    BOOST_CHECK_EQUAL(pa_roTestee.getDataTypeID(), CIEC_ANY::e_ANY);
  }

  BOOST_AUTO_TEST_CASE(Any_SaveWrongTypisation_test){
    CIEC_ANY *poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "BOOL#231");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "SINT#-130");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "INT#40000");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "DINT#411232312345");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "USINT#257");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "UINT#6257541235");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "UDINT#7311232312325346");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "BYTE#256");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "WORD#62575412345");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "DWORD#831123235938695");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "DATE#tes2008-04");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "TIME_OF_DAY#test15:36.36");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "DATE_AND_TIME#Test-06-22-54.8");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "TIME#Test");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "REAL#test");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "LREAL#hansi");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "LINT#1231231231239485692939293");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "ULINT#1231231231927598267926798546723123");
    delete poTestee;

    poTestee = CIEC_ANY::createDataType(0);
    checkSaveWrongTypisation(*poTestee, "LWORD#823123123123123123468");
    delete poTestee;
  }
  BOOST_AUTO_TEST_SUITE_END()
