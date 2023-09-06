/*******************************************************************************
 * Copyright (c) 2023 - Primetals Technologies Austria GmbH
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

#include "E_CTUD.h"


BOOST_AUTO_TEST_SUITE(FUNCBLOC)

BOOST_AUTO_TEST_CASE(FB_TO_STRING_TEST){
    FORTE_E_CTUD testFb(0, nullptr); // Dummy FB, do not use for anything else than testing toString
    char buffer[50];
    testFb.toString(buffer, sizeof(buffer));
    BOOST_TEST(buffer == "(PV:=0, QU:=FALSE, QD:=FALSE, CV:=0)");
}

BOOST_AUTO_TEST_SUITE_END()