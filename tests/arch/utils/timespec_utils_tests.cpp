/*******************************************************************************
* Copyright (c) 2018 ACIN
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*
* Contributors:
*   Martin Melik Merkumians - initial test implementation and/or initial documentation
*******************************************************************************/

#include <boost/test/unit_test.hpp>

#include "../../../src/arch/utils/timespec_utils.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(TimeSpecUtils)

BOOST_AUTO_TEST_CASE(AddTwoTimeSpecs)
{
  struct timespec start = { 0, 0 };
  struct timespec end = { 1, 26548 };
  struct timespec result = { 0, 0 };
  timespecAdd(&start, &end, &result);
  BOOST_TEST(result.tv_sec == 1);
  BOOST_TEST(result.tv_nsec == 26548);
}

BOOST_AUTO_TEST_CASE(AddTwoTimeSpecsWithOverflow)
{
  struct timespec start = { 0, 1000000000 };
  struct timespec end = { 1, 26548 };
  struct timespec result = { 0, 0 };
  timespecAdd(&start, &end, &result);
  BOOST_TEST(result.tv_sec == 2);
  BOOST_TEST(result.tv_nsec == 26548);
}

BOOST_AUTO_TEST_CASE(SubtractTwoTimeSpecs)
{
  struct timespec start = { 5, 543254 };
  struct timespec end = { 1, 26548 };
  struct timespec result = { 0, 0 };
  timespecSub(&start, &end, &result);
  BOOST_TEST(result.tv_sec == 4);
  BOOST_TEST(result.tv_nsec == 516706);
}

BOOST_AUTO_TEST_CASE(SubtractTwoTimeSpecsWithOverflow)
{
  struct timespec start = { 5, 26548};
  struct timespec end = { 1, 543254 };
  struct timespec result = { 0, 0 };
  timespecSub(&start, &end, &result);
  BOOST_TEST(result.tv_sec == 3);
  BOOST_TEST(result.tv_nsec == 999483294);
}

BOOST_AUTO_TEST_SUITE_END()
