/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _FORTE_SERVICEREQUESTFORM_H_
#define _FORTE_SERVICEREQUESTFORM_H_

#include "forte_struct.h"
#include "forte_arrowheadsystem.h"
#include "forte_arrowheadcloud.h"
#include "forte_arrowheadservice.h"
#include "forte_wstring.h"
#include "forte_dint.h"
#include "forte_array.h"
#include "forte_preferredprovider.h"

class CIEC_ServiceRequestForm : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(ServiceRequestForm)

  public:
    CIEC_ServiceRequestForm();

    virtual ~CIEC_ServiceRequestForm() {
    }

    CIEC_ArrowheadSystem &requesterSystem() {
      return *static_cast<CIEC_ArrowheadSystem*>(&getMembers()[0]);
    }

    CIEC_ArrowheadCloud &requesterCloud() {
      return *static_cast<CIEC_ArrowheadCloud*>(&getMembers()[1]);
    }

    CIEC_ArrowheadService &requestedService() {
      return *static_cast<CIEC_ArrowheadService*>(&getMembers()[2]);
    }

    CIEC_WSTRING *orchestrationFlags() {
      return static_cast<CIEC_WSTRING*>((*static_cast<CIEC_ARRAY *>(&getMembers()[3]))[0]); //the first element marks the start of the array
    }

    CIEC_PreferredProvider *preferredProviders() {
      return static_cast<CIEC_PreferredProvider*>((*static_cast<CIEC_ARRAY *>(&getMembers()[4]))[0]); //the first element marks the start of the array
    }

    CIEC_WSTRING *requestedQoS() {
      return static_cast<CIEC_WSTRING*>((*static_cast<CIEC_ARRAY *>(&getMembers()[5]))[0]); //the first element marks the start of the array
    }

  private:
    static const CStringDictionary::TStringId scmElementTypes[];
    static const CStringDictionary::TStringId scmElementNames[];
};

#endif //_FORTE_SERVICEREQUESTFORM_H_

