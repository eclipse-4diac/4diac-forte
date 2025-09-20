/*******************************************************************************
 * Copyright (c) 2014, 2025 fortiss GmbH, HR Agrartechnik GmbH,
 *                          Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Waldemar Eisenmenger, Monika Wenger
 *                    - initial API and implementation and/or initial documentation
 *   Franz Hoepfinger - copied over IX to IE, and removed the IN
 *   Alois Zoitl      - copied to core/io and adjusted to core/io process
 *                      interface needs
 *******************************************************************************/

#pragma once

#include "forte/io/processinterfacefb.h"

class FORTE_IE final : public forte::io::CProcessInterfaceFB {
    DECLARE_FIRMWARE_FB(FORTE_IE)

  private:
    static const TEventID scmEventINDID = 2;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void writeOutputData(TEventID paEIID) override;

  public:
    FORTE_IE(forte::StringId paInstanceNameId, forte::CFBContainer &paContainer);

    CEventConnection conn_IND;

    CEventConnection *getEOConUnchecked(TPortId) override;

    CIEC_ANY::EDataTypeID getIOObserverDataType() override {
      return CIEC_ANY::e_Max;
    }

    forte::io::IOMapper::Direction getDirection() override {
      return forte::io::IOMapper::In;
    }

    void evt_REQ(const CIEC_BOOL &paQI, const CIEC_STRING &paPARAMS, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS) {
      var_QI = paQI;
      var_PARAMS = paPARAMS;
      receiveInputEvent(scmEventREQID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }
};
