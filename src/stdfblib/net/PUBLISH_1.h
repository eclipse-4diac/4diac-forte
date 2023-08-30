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

#include "GEN_PUBLISH.h"

class FORTE_PUBLISH_1 : public GEN_PUBLISH {
  DECLARE_GENERIC_FIRMWARE_FB(FORTE_PUBLISH_1)

  protected:
    static const TEventID scmEventREQID = scmSendNotificationEventID;

  public:
    FORTE_PUBLISH_1(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);
    ~FORTE_PUBLISH_1() override = default;

    void evt_INIT(const CIEC_BOOL &pa_QI, const CIEC_WSTRING &pa_ID, const CIEC_ANY &pa_SD_1, CIEC_BOOL &pa_QO, CIEC_WSTRING &pa_STATUS) {
      QI() = pa_QI;
      ID() = pa_ID;
      getSDs()[0]->setValue(pa_SD_1.unwrap());
      receiveInputEvent(scmEventINITID, nullptr);
      pa_QO = QO();
      pa_STATUS = STATUS();
    }
    void evt_REQ(const CIEC_BOOL &pa_QI, const CIEC_WSTRING &pa_ID, const CIEC_ANY &pa_SD_1, CIEC_BOOL &pa_QO, CIEC_WSTRING &pa_STATUS) {
      QI() = pa_QI;
      ID() = pa_ID;
      getSDs()[0]->setValue(pa_SD_1.unwrap());
      receiveInputEvent(scmEventREQID, nullptr);
      pa_QO = QO();
      pa_STATUS = STATUS();
    }
    void operator()(const CIEC_BOOL &pa_QI, const CIEC_WSTRING &pa_ID, const CIEC_ANY &pa_SD_1, CIEC_BOOL &pa_QO, CIEC_WSTRING &pa_STATUS) {
      evt_INIT(pa_QI, pa_ID, pa_SD_1, pa_QO, pa_STATUS);
    }
};
