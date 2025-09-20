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

#ifndef _REGISTERSERVICEHTTP_H_
#define _REGISTERSERVICEHTTP_H_

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "RegisterServiceAdp_adp.h"

class FORTE_RegisterServiceHTTP : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_RegisterServiceHTTP)

  private:
    FORTE_RegisterServiceAdp &registerService() {
      return (*static_cast<FORTE_RegisterServiceAdp *>(getAdapterUnchecked(0)));
    };
    static const int scm_nregisterServiceAdpNum = 0;

  public:
    FORTE_RegisterServiceHTTP(const forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_RegisterServiceHTTP() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
