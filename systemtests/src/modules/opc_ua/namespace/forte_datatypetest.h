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

#ifndef _FORTE_DATATYPETEST_H_
#define _FORTE_DATATYPETEST_H_

#include "forte_struct.h"
#include "forte_string.h"
#include "forte_sint.h"
#include "forte_bool.h"

class CIEC_DataTypeTest : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(DataTypeTest)

  public:
    CIEC_DataTypeTest();

    virtual ~CIEC_DataTypeTest() {
    }

    CIEC_STRING &Name() {
      return *static_cast<CIEC_STRING*>(&getMembers()[0]);
    }

    CIEC_SINT &Age() {
      return *static_cast<CIEC_SINT*>(&getMembers()[1]);
    }

    CIEC_BOOL &IsRegistered() {
      return *static_cast<CIEC_BOOL*>(&getMembers()[2]);
    }

  private:
    static const CStringDictionary::TStringId scmElementTypes[];
    static const CStringDictionary::TStringId scmElementNames[];
};

#endif //_FORTE_DATATYPETEST_H_
