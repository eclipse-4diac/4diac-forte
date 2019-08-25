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

#ifndef _FORTE_PREFEREDPROVIDER_H_
#define _FORTE_PREFEREDPROVIDER_H_

#include "forte_struct.h"
#include "forte_arrowheadsystem.h"
#include "forte_arrowheadcloud.h"

class CIEC_PreferredProvider : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(PreferredProvider)

  public:
    CIEC_PreferredProvider();

    virtual ~CIEC_PreferredProvider() {
    }

    CIEC_ArrowheadSystem &providerSystem() {
      return *static_cast<CIEC_ArrowheadSystem*>(&getMembers()[0]);
    }

    CIEC_ArrowheadCloud &providerCloud() {
      return *static_cast<CIEC_ArrowheadCloud*>(&getMembers()[1]);
    }

  private:
    static const CStringDictionary::TStringId scmElementTypes[];
    static const CStringDictionary::TStringId scmElementNames[];
};

#endif //_FORTE_PREFEREDPROVIDER_H_
