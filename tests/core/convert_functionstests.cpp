/*******************************************************************************
* Copyright (c) 2018 TU Vienna/ACIN
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*
* Contributors:
*   Martin Melik Merkumians - initial API and implementation and/or initial documentation
*******************************************************************************/

#include <boost/test/unit_test.hpp>

#include "convert_functions.h"

#include "forte_date_and_time.h"
#include "forte_time_of_day.h"

BOOST_AUTO_TEST_SUITE(convert_functions)

BOOST_AUTO_TEST_CASE(CONVERT_DT_TO_TOD)
{
  CIEC_DATE_AND_TIME date;
  date.fromString("DT#2018-07-10-12:38:50.123");

  CIEC_TIME_OF_DAY tod(DT_TO_TOD(date));
  BOOST_TEST((TForteUInt64)tod == (12*3600 + 38*60 + 50)*1000ULL + 123);
}

BOOST_AUTO_TEST_SUITE_END()
