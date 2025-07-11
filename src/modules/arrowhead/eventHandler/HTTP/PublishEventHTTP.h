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

#ifndef _PUBLISHEVENTHTTP_H_
#define _PUBLISHEVENTHTTP_H_

#include "core/cfb.h"
#include "core/typelib.h"
#include "ArrowheadPublishAdp_adp.h"

class FORTE_PublishEventHTTP : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_PublishEventHTTP)

  private:
    FORTE_ArrowheadPublishAdp &publishEvent() {
      return (*static_cast<FORTE_ArrowheadPublishAdp *>(getAdapterUnchecked(0)));
    };
    static const int scm_npublishEventAdpNum = 0;

  public:
    FORTE_PublishEventHTTP(const CStringDictionary::TStringId paInstanceNameId,
                           forte::core::CFBContainer &paContainer) :

        ~FORTE_PublishEventHTTP() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
