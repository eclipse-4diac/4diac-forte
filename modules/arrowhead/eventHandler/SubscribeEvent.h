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

#ifndef _SUBSCRIBEEVENT_H_
#define _SUBSCRIBEEVENT_H_

#include "forte/cfb.h"
#include "forte/typelib.h"
#include <forte_eventfilter.h>
#include "forte/datatypes/forte_wstring.h"
#include "SubscribeEventAdp_adp.h"

class FORTE_SubscribeEvent : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_SubscribeEvent)

  private:
    CIEC_EventFilter &eventFilter() {
      return *static_cast<CIEC_EventFilter *>(getDI(0));
    };

    CIEC_WSTRING &endpoint() {
      return *static_cast<CIEC_WSTRING *>(getDI(1));
    };

    static const TEventID scmEventSubscribeID = 0;
    static const TEventID scmEventUnsubscribeID = 1;

    static const TEventID scmEventSubscribedID = 0;
    static const TEventID scmEventUnsubscribedID = 1;

    FORTE_SubscribeEventAdp &SubscribeEventAdp() {
      return (*static_cast<FORTE_SubscribeEventAdp *>(getAdapterUnchecked(0)));
    };
    static const int scmSubscribeEventAdpAdpNum = 0;

  public:
    FORTE_SubscribeEvent(const forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_SubscribeEvent() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
