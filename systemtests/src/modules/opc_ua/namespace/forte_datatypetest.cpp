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
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_datatypetest_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(DataTypeTest, g_nStringIdDataTypeTest);

CIEC_DataTypeTest::CIEC_DataTypeTest() :
    CIEC_STRUCT(g_nStringIdDataTypeTest, 2, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_DataTypeTest::scmElementTypes[] = { g_nStringIdSTRING, g_nStringIdSINT, g_nStringIdBOOL };
const CStringDictionary::TStringId CIEC_DataTypeTest::scmElementNames[] = { g_nStringIdName, g_nStringIdAge, g_nStringIdIsRegistered };
