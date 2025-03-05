/*******************************************************************************
 * Copyright (c) 2021, 2022 Jonathan Lainer (kontakt@lainer.co.at)
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *******************************************************************************/

#pragma once

#include "funcbloc.h"
#include "PortAdapter_adp.h"
#include "forte_dword.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

#include "../handler/EliteBoardDeviceController.h"

class FORTE_EliteBoard final : public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_EliteBoard)

  private:
    static const TEventID scmEventMAPID = 0;
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventMAPOID = 0;
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];
    static const int scmPortAAdpNum = 0;
    static const int scmPortBAdpNum = 1;
    static const int scmPortCAdpNum = 2;
    static const int scmPortDAdpNum = 3;
    static const int scmPortEAdpNum = 4;
    static const int scmPortFAdpNum = 5;
    static const int scmPortGAdpNum = 6;
    static const int scmPortHAdpNum = 7;
    static const int scmPortIAdpNum = 8;
    static const int scmPortJAdpNum = 9;
    static const int scmPortKAdpNum = 10;
    static const SAdapterInstanceDef scmAdapterInstances[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;

    int mCurrentAdapterIndex = 0;
    static const int mAdapterCount = scmPortKAdpNum + 1;

    FORTE_PortAdapter &getPortAdapterByIndex(int index);
    bool configurePortFB(int index, CEventChainExecutionThread * const paECET);
    int configPorts(CEventChainExecutionThread *const paECET);

  public:
    FORTE_EliteBoard(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);
    bool initialize() override;

    FORTE_PortAdapter var_PortA;
    FORTE_PortAdapter var_PortB;
    FORTE_PortAdapter var_PortC;
    FORTE_PortAdapter var_PortD;
    FORTE_PortAdapter var_PortE;
    FORTE_PortAdapter var_PortF;
    FORTE_PortAdapter var_PortG;
    FORTE_PortAdapter var_PortH;
    FORTE_PortAdapter var_PortI;
    FORTE_PortAdapter var_PortJ;
    FORTE_PortAdapter var_PortK;

    CEventConnection conn_MAPO;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CAdapter *getAdapterUnchecked(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_MAP() {
      executeEvent(scmEventMAPID, nullptr);
    }

    void operator()() {
      evt_MAP();
    }
};

