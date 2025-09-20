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

#ifndef _PUBLISHEVENTOPCUA_H_
#define _PUBLISHEVENTOPCUA_H_

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "ArrowheadPublishAdp_adp.h"

class FORTE_PublishEventOpcUa : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_PublishEventOpcUa)

  private:
    FORTE_ArrowheadPublishAdp &publishEvent() {
      return (*static_cast<FORTE_ArrowheadPublishAdp *>(getAdapterUnchecked(0)));
    };
    static const int scm_npublishEventAdpNum = 0;

  public:
    FORTE_PublishEventOpcUa(const forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_PublishEventOpcUa() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
