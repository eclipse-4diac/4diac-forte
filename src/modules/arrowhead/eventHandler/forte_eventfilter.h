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

#ifndef _FORTE_EVENTFILTER_H_
#define _FORTE_EVENTFILTER_H_

#include "forte_struct.h"
#include "forte_array.h"
#include "forte_wstring.h"
#include "forte_arrowheadsystem.h"
#include "forte_date_and_time.h"
#include "forte_bool.h"

class CIEC_EventFilter : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(EventFilter)

  public:
    CIEC_EventFilter();

    virtual ~CIEC_EventFilter() {
    }

    CIEC_WSTRING &eventType() {
      return *static_cast<CIEC_WSTRING*>(&getMembers()[0]);
    }

    CIEC_ArrowheadSystem &consumer() {
      return *static_cast<CIEC_ArrowheadSystem*>(&getMembers()[1]);
    }

    CIEC_ARRAY *sources_array() {
      return static_cast<CIEC_ARRAY *>(&getMembers()[2]); //the first element marks the start of the array
    }

    CIEC_ArrowheadSystem *sources() {
      return static_cast<CIEC_ArrowheadSystem*>((*static_cast<CIEC_ARRAY *>(&getMembers()[2]))[0]); //the first element marks the start of the array
    }

    CIEC_DATE_AND_TIME &startDate() {
      return *static_cast<CIEC_DATE_AND_TIME*>(&getMembers()[3]);
    }
    CIEC_DATE_AND_TIME &endDate() {
      return *static_cast<CIEC_DATE_AND_TIME*>(&getMembers()[4]);
    }

    CIEC_ARRAY *filterMetadata_array() {
      return static_cast<CIEC_ARRAY *>(&getMembers()[5]); //the first element marks the start of the array
    }

    CIEC_WSTRING *filterMetadata() {
      return static_cast<CIEC_WSTRING*>((*static_cast<CIEC_ARRAY *>(&getMembers()[5]))[0]); //the first element marks the start of the array
    }

    CIEC_WSTRING &notifyUri() {
      return *static_cast<CIEC_WSTRING*>(&getMembers()[6]);
    }
    CIEC_BOOL &matchMetadata() {
      return *static_cast<CIEC_BOOL*>(&getMembers()[7]);
    }

  private:
    static const CStringDictionary::TStringId scmElementTypes[];
    static const CStringDictionary::TStringId scmElementNames[];
};

#endif //_FORTE_EVENTFILTER_H_
