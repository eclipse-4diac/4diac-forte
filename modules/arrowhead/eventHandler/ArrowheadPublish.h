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

#ifndef _ARROWHEADPUBLISH_H_
#define _ARROWHEADPUBLISH_H_

#include "forte/cfb.h"
#include "forte/typelib.h"
#include <forte_publishevent.h>
#include "forte/datatypes/forte_wstring.h"
#include "ArrowheadPublishAdp_adp.h"

class FORTE_ArrowheadPublish : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_ArrowheadPublish)

  private:
    CIEC_PublishEvent &publishEvent() {
      return *static_cast<CIEC_PublishEvent *>(getDI(0));
    };

    CIEC_WSTRING &endpoint() {
      return *static_cast<CIEC_WSTRING *>(getDI(1));
    };

    static const TEventID scmEventpublishID = 0;

    static const TEventID scmEventpublishedID = 0;

    FORTE_ArrowheadPublishAdp &ArrowheadPublishAdp() {
      return (*static_cast<FORTE_ArrowheadPublishAdp *>(getAdapterUnchecked(0)));
    };
    static const int scmArrowheadPublishAdpAdpNum = 0;

  public:
    FORTE_ArrowheadPublish(const forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_ArrowheadPublish() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
