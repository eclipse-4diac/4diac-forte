/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "forte_datatypetest.h"

using namespace forte::core::literals;

DEFINE_FIRMWARE_DATATYPE(DataTypeTest, "DataTypeTest"_STRID);

CIEC_DataTypeTest::CIEC_DataTypeTest() : CIEC_STRUCT(), var_Name(""), var_Age(0), var_IsRegistered(false) {
}

forte::core::StringId CIEC_DataTypeTest::getStructTypeNameID() const {
  return "DataTypeTest"_STRID;
}

CIEC_ANY *CIEC_DataTypeTest::getMember(size_t paMemberIndex) {
  switch (paMemberIndex) {
    case 0: return &var_Name;
    case 1: return &var_Age;
    case 2: return &var_IsRegistered;
  }
  return nullptr;
}

const CIEC_ANY *CIEC_DataTypeTest::getMember(size_t paMemberIndex) const {
  switch (paMemberIndex) {
    case 0: return &var_Name;
    case 1: return &var_Age;
    case 2: return &var_IsRegistered;
  }
  return nullptr;
}

const forte::core::StringId CIEC_DataTypeTest::scmElementNames[] = {"Name"_STRID, "Age"_STRID, "IsRegistered"_STRID};
