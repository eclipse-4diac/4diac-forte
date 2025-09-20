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

#ifndef _FIELDSTOPUBLISHEVENT_H_
#define _FIELDSTOPUBLISHEVENT_H_

#include "forte/basicfb.h"
#include <forte_publishevent.h>
#include <forte_arrowheadsystem.h>
#include <forte_arrowheadevent.h>
#include "forte/datatypes/forte_wstring.h"

class FORTE_FieldsToPublishEvent : public CBasicFB {
    DECLARE_FIRMWARE_FB(FORTE_FieldsToPublishEvent)

  private:
    CIEC_ArrowheadSystem &source() {
      return *static_cast<CIEC_ArrowheadSystem *>(getDI(0));
    };

    CIEC_ArrowheadEvent &event() {
      return *static_cast<CIEC_ArrowheadEvent *>(getDI(1));
    };

    CIEC_WSTRING &deliveryCompleteUri() {
      return *static_cast<CIEC_WSTRING *>(getDI(2));
    };

    CIEC_PublishEvent &publishEvent() {
      return *static_cast<CIEC_PublishEvent *>(getDO(0));
    };

    static const TEventID scmEventREQID = 0;

    static const TEventID scmEventCNFID = 0;

    void alg_REQ();
    static const TForteInt16 scmStateSTART = 0;
    static const TForteInt16 scmStateREQ = 1;

    void enterStateSTART(CEventChainExecutionThread *const paECET);
    void enterStateREQ(CEventChainExecutionThread *const paECET);

    virtual void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  public:
    FORTE_FieldsToPublishEvent(forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_FieldsToPublishEvent() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
