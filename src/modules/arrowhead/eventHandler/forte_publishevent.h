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

#ifndef _FORTE_PUBLISHEVENT_H_
#define _FORTE_PUBLISHEVENT_H_

#include "forte_struct.h"
#include "forte_wstring.h"
#include "forte_arrowheadsystem.h"
#include "forte_arrowheadevent.h"

class CIEC_PublishEvent : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(PublishEvent)

  public:
    CIEC_PublishEvent();

    virtual ~CIEC_PublishEvent() {
    }

    CIEC_ArrowheadSystem &source() {
      return *static_cast<CIEC_ArrowheadSystem*>(&getMembers()[0]);
    }

    CIEC_ArrowheadEvent &event() {
      return *static_cast<CIEC_ArrowheadEvent*>(&getMembers()[1]);
    }

    CIEC_WSTRING &deliveryCompleteUri() {
      return *static_cast<CIEC_WSTRING*>(&getMembers()[2]);
    }

  private:
    static const CStringDictionary::TStringId scmElementTypes[];
    static const CStringDictionary::TStringId scmElementNames[];
};

#endif //_FORTE_PUBLISHEVENT_H_
