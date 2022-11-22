/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#pragma once

#ifndef _FORTE_BOOST_OUTPUT_SUPPORT_H_
#define _FORTE_BOOST_OUTPUT_SUPPORT_H_

#include <iostream>
#include "forte_string.h"

inline std::ostream& boost_test_print_type(std::ostream &out, const CIEC_STRING &string) {
  char outputString[string.getToStringBufferSize()];
  string.toString(outputString, sizeof(outputString));
  out << outputString;
  return out;
}

#endif