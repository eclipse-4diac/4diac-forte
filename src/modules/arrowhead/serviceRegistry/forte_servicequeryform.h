/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _FORTE_SERVICEQUERYFORM_H_
#define _FORTE_SERVICEQUERYFORM_H_

#include "forte_struct.h"
#include "forte_arrowheadservice.h"
#include <forte_bool.h>
#include "forte_dint.h"

class CIEC_ServiceQueryForm : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(ServiceQueryForm)

  public:
    CIEC_ServiceQueryForm();

    virtual ~CIEC_ServiceQueryForm() {
    }

    CIEC_ArrowheadService &service() {
      return *static_cast<CIEC_ArrowheadService*>(&getMembers()[0]);
    }

    CIEC_BOOL &metadataSearch() {
      return *static_cast<CIEC_BOOL*>(&getMembers()[1]);
    }


    CIEC_BOOL &pingProviders() {
      return *static_cast<CIEC_BOOL*>(&getMembers()[2]);
    }

    CIEC_DINT &version() {
      return *static_cast<CIEC_DINT*>(&getMembers()[3]);
    }

  private:
    static const CStringDictionary::TStringId scmElementTypes[];
    static const CStringDictionary::TStringId scmElementNames[];
};

#endif //_FORTE_SERVICEQUERYFORM_H_
