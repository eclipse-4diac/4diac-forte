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

#ifndef _ARROWHEADPUBLISHADP_H_
#define _ARROWHEADPUBLISHADP_H_

#include "forte/adapter.h"
#include "forte/typelib.h"
#include <forte_publishevent.h>
#include "forte/datatypes/forte_wstring.h"

class FORTE_ArrowheadPublishAdp : public CAdapter {
    DECLARE_ADAPTER_TYPE(FORTE_ArrowheadPublishAdp)

  private:
  private:
  public:
    CIEC_PublishEvent &publishEvent() {
      return *static_cast<CIEC_PublishEvent *>((isSocket()) ? getDO(0) : getDI(0));
    };

    CIEC_WSTRING &endpoint() {
      return *static_cast<CIEC_WSTRING *>((isSocket()) ? getDO(1) : getDI(1));
    };

  public:
    static const TEventID scmEventpublishedID = 0;
    int published() {
      return mParentAdapterListEventID + scmEventpublishedID;
    }

  private:
  public:
    static const TEventID scmEventpublishID = 0;
    int publish() {
      return mParentAdapterListEventID + scmEventpublishID;
    }

  private:
  public:
    ADAPTER_CTOR(FORTE_ArrowheadPublishAdp) {};

    ~FORTE_ArrowheadPublishAdp() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
