/*******************************************************************************
 * Copyright (c) 2019, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _FORTE_ARROWHEADSEVENT_H_
#define _FORTE_ARROWHEADSEVENT_H_

#include "forte/datatypes/forte_struct.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_date_and_time.h"

class CIEC_ArrowheadEvent : public CIEC_STRUCT {
    DECLARE_FIRMWARE_DATATYPE(ArrowheadEvent)

  public:
    CIEC_ArrowheadEvent();

    ~CIEC_ArrowheadEvent() override = default;

    CIEC_WSTRING &type() {
      return *static_cast<CIEC_WSTRING *>(&getMembers()[0]);
    }

    CIEC_WSTRING &payload() {
      return *static_cast<CIEC_WSTRING *>(&getMembers()[1]);
    }

    CIEC_DATE_AND_TIME &timestamp() {
      return *static_cast<CIEC_DATE_AND_TIME *>(&getMembers()[2]);
    }

    CIEC_ARRAY<> *eventMetadata_array() {
      return static_cast<CIEC_ARRAY<> *>(&getMembers()[3]); // the first element marks the start of the array
    }

    CIEC_WSTRING *eventMetadata() {
      return static_cast<CIEC_WSTRING *>(
          (*static_cast<CIEC_ARRAY<> *>(&getMembers()[3]))[0]); // the first element marks the start of the array
    }

  private:
    static const forte::StringId scmElementTypes[];
    static const forte::StringId scmElementNames[];
};

#endif //_FORTE_ARROWHEADSEVENT_H_
