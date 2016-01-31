/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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

