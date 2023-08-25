/*******************************************************************************
 * Copyright (c) 2022 - Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

#include "forte_st_iterator.h"

#include "forte_uint.h"
#include "forte_int.h"

BOOST_AUTO_TEST_SUITE(ST_FOR_Iterator)

BOOST_AUTO_TEST_CASE(ST_ITER_CIEC_INT_IMPLICIT_BY)
{
    CIEC_INT runner;
    CIEC_INT start(0);
    CIEC_INT to(42);
    for (auto st_lv_synthetic_1 : ST_FOR_ITER<CIEC_INT>(runner, start, to)) {
    }
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(runner) == (static_cast<CIEC_INT::TValueType>(to) + 1));
}

BOOST_AUTO_TEST_CASE(ST_ITER_CIEC_INT_EXPLICIT_BY)
{
    CIEC_INT runner;
    CIEC_INT start(0);
    CIEC_INT to(42);
    CIEC_INT by(2);
    for (auto st_lv_synthetic_1 : ST_FOR_ITER<CIEC_INT>(runner, start, to, by)) {
    }
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(runner) == (static_cast<CIEC_INT::TValueType>(to) + 2));
}

BOOST_AUTO_TEST_CASE(ST_ITER_CIEC_UINT_IMPLICIT_BY)
{
    CIEC_UINT runner;
    CIEC_UINT start(0);
    CIEC_UINT to(42);
    for (auto st_lv_synthetic_1 : ST_FOR_ITER<CIEC_UINT>(runner, start, to)) {
    }
    BOOST_TEST(static_cast<CIEC_UINT::TValueType>(runner) == (static_cast<CIEC_UINT::TValueType>(to) + 1));
}

BOOST_AUTO_TEST_CASE(ST_ITER_CIEC_UINT_EXPLICIT_BY)
{
    CIEC_UINT runner;
    CIEC_UINT start(0);
    CIEC_UINT to(42);
    CIEC_UINT by(2);
    for (auto st_lv_synthetic_1 : ST_FOR_ITER<CIEC_UINT>(runner, start, to, by)) {
    }
    BOOST_TEST(static_cast<CIEC_UINT::TValueType>(runner) == (static_cast<CIEC_UINT::TValueType>(to) + 2));
}

BOOST_AUTO_TEST_CASE(ST_ITER_CIEC_INT_IMPLICIT_BY_OVERFLOW)
{
    CIEC_INT runner;
    CIEC_INT start(CIEC_INT::scmMaxVal - 2);
    CIEC_INT to(CIEC_INT::scmMaxVal);
    for (auto st_lv_synthetic_1 : ST_FOR_ITER<CIEC_INT>(runner, start, to)) {
    }
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(runner) == CIEC_INT::scmMaxVal);
}

BOOST_AUTO_TEST_CASE(ST_ITER_CIEC_INT_EXPLICIT_BY_OVERFLOW)
{
    CIEC_INT runner;
    CIEC_INT start(CIEC_INT::scmMaxVal - 2);
    CIEC_INT to(CIEC_INT::scmMaxVal);
    CIEC_INT by(1);
    for (auto st_lv_synthetic_1 : ST_FOR_ITER<CIEC_INT>(runner, start, to, by)) {
    }
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(runner) == CIEC_INT::scmMaxVal);
}

BOOST_AUTO_TEST_CASE(ST_ITER_CIEC_UINT_IMPLICIT_BY_OVERFLOW)
{
    CIEC_UINT runner;
    CIEC_UINT start(CIEC_UINT::scmMaxVal - 2);
    CIEC_UINT to(CIEC_UINT::scmMaxVal);
    for (auto st_lv_synthetic_1 : ST_FOR_ITER<CIEC_UINT>(runner, start, to)) {
    }
    BOOST_TEST(static_cast<CIEC_UINT::TValueType>(runner) == CIEC_UINT::scmMaxVal);
}

BOOST_AUTO_TEST_CASE(ST_ITER_CIEC_UINT_EXPLICIT_BY_OVERFLOW)
{
    CIEC_UINT runner;
    CIEC_UINT start(CIEC_UINT::scmMaxVal - 2);
    CIEC_UINT to(CIEC_UINT::scmMaxVal);
    CIEC_UINT by(1);
    for (auto st_lv_synthetic_1 : ST_FOR_ITER<CIEC_UINT>(runner, start, to, by)) {
    }
    BOOST_TEST(static_cast<CIEC_UINT::TValueType>(runner) == CIEC_UINT::scmMaxVal);
}

BOOST_AUTO_TEST_CASE(ST_ITER_CIEC_INT_EXPLICIT_BY_UNDERFLOW)
{
    CIEC_INT runner;
    CIEC_INT start(CIEC_INT::scmMinVal + 2);
    CIEC_INT to(CIEC_INT::scmMinVal);
    CIEC_INT by(-1);
    for (auto st_lv_synthetic_1 : ST_FOR_ITER<CIEC_INT>(runner, start, to, by)) {
    }
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(runner) == CIEC_INT::scmMinVal);
}

BOOST_AUTO_TEST_CASE(ST_ITER_CIEC_UINT_EXPLICIT_BY_UNDERFLOW)
{
    CIEC_UINT runner;
    CIEC_UINT start(CIEC_UINT::scmMinVal + 2);
    CIEC_UINT to(CIEC_UINT::scmMinVal);
    CIEC_INT by(-1);
    for (auto st_lv_synthetic_1 : ST_FOR_ITER<CIEC_UINT, CIEC_INT>(runner, start, to, by)) {
    }
    BOOST_TEST(static_cast<CIEC_UINT::TValueType>(runner) == 0);
}

BOOST_AUTO_TEST_CASE(ST_ITER_CIEC_INT_ABORT_BEFORE_FIRST_LOOP)
{
    CIEC_INT runner;
    CIEC_INT start(0);
    CIEC_INT to(-1);
    CIEC_INT by(1);
    for (auto st_lv_synthetic_1 : ST_FOR_ITER<CIEC_INT>(runner, start, to, by)) {
    }
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(runner) == 0);
}

BOOST_AUTO_TEST_SUITE_END()
