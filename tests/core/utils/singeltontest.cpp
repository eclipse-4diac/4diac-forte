/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
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
#include "testsingleton.h"

BOOST_AUTO_TEST_SUITE(Singleton_Test)

  BOOST_AUTO_TEST_CASE(singleton_SingleInstanceTest){

    TestSingleton *pSingleton1 = &(TestSingleton::getInstance());
    TestSingleton *pSingleton2 = &(TestSingleton::getInstance());

    BOOST_CHECK_EQUAL(pSingleton1, pSingleton2);

    BOOST_CHECK_EQUAL(1, TestSingleton::getInstance().getSingletonID());
  }

  BOOST_AUTO_TEST_CASE(singleton_DifferentFunctionTest){
    BOOST_CHECK_EQUAL(1, TestSingleton::getInstance().getSingletonID());
  }


BOOST_AUTO_TEST_SUITE_END()

