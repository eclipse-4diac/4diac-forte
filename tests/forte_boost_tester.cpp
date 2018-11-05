/*******************************************************************************
 * Copyright (c) 2011, 2018 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Ingo Hegny - initial API and implementation and/or initial documentation
 *   Jose Cabral - Add dummy function for throw_exception
 *******************************************************************************/
#define BOOST_TEST_MODULE FORTE_TESTS
#include <boost/test/included/unit_test.hpp>

void boost::throw_exception(std::exception const&) {
  //dummy
}
