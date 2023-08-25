/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Jobst - initial implementation
 *******************************************************************************/

#pragma once

#include "GEN_SUBSCRIBE.h"

class FORTE_SUBSCRIBE_1 : public GEN_SUBSCRIBE {
  DECLARE_GENERIC_FIRMWARE_FB(FORTE_SUBSCRIBE_1)

  protected:
    static const TEventID scmEventRSPID = scmSendNotificationEventID;

  public:
    FORTE_SUBSCRIBE_1(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);
    ~FORTE_SUBSCRIBE_1() override = default;

    void evt_INIT(const CIEC_BOOL &pa_QI, const CIEC_WSTRING &pa_ID, CIEC_BOOL &pa_QO, CIEC_WSTRING &pa_STATUS, CIEC_ANY &pa_RD_1) {
      QI() = pa_QI;
      ID() = pa_ID;
      receiveInputEvent(scmEventINITID, nullptr);
      pa_QO = QO();
      pa_STATUS = STATUS();
      pa_RD_1.setValue(getRDs()[0]->unwrap());
    }
    void evt_RSP(const CIEC_BOOL &pa_QI, const CIEC_WSTRING &pa_ID, CIEC_BOOL &pa_QO, CIEC_WSTRING &pa_STATUS, CIEC_ANY &pa_RD_1) {
      QI() = pa_QI;
      ID() = pa_ID;
      receiveInputEvent(scmEventRSPID, nullptr);
      pa_QO = QO();
      pa_STATUS = STATUS();
      pa_RD_1.setValue(getRDs()[0]->unwrap());
    }
    void operator()(const CIEC_BOOL &pa_QI, const CIEC_WSTRING &pa_ID, CIEC_BOOL &pa_QO, CIEC_WSTRING &pa_STATUS, CIEC_ANY &pa_RD_1) {
      evt_INIT(pa_QI, pa_ID, pa_QO, pa_STATUS, pa_RD_1);
    }
};
