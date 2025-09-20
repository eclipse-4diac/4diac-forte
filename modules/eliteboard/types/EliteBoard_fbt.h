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

#include "forte/funcbloc.h"
#include "PortAdapter_adp.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

#include "../handler/EliteBoardDeviceController.h"
#include "forte/io/configFB/io_configFB_controller.h"

using namespace forte::io;

class FORTE_EliteBoard final : public IOConfigFBController {
    DECLARE_FIRMWARE_FB(FORTE_EliteBoard)

  private:
    static const TEventID scmEventMAPID = 0;
    static const TEventID scmEventMAPOID = 0;
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

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

    int mCurrentAdapterIndex = 0;
    static const int mAdapterCount = scmPortKAdpNum + 1;

    FORTE_PortAdapter &getPortAdapterByIndex(int index);
    bool configurePortFB(int index, CEventChainExecutionThread *const paECET);
    int configPorts(CEventChainExecutionThread *const paECET);

    // IOConfigFBController
    EliteBoardDeviceController &mEventHandler;
    IODeviceController *createDeviceController(CDeviceExecution &paDeviceExecution) override;
    void setConfig() override {
    }

  public:
    FORTE_EliteBoard(forte::StringId paInstanceNameId, CFBContainer &paContainer);

    forte::CPlugPin<FORTE_PortAdapter_Plug> var_PortA;
    forte::CPlugPin<FORTE_PortAdapter_Plug> var_PortB;
    forte::CPlugPin<FORTE_PortAdapter_Plug> var_PortC;
    forte::CPlugPin<FORTE_PortAdapter_Plug> var_PortD;
    forte::CPlugPin<FORTE_PortAdapter_Plug> var_PortE;
    forte::CPlugPin<FORTE_PortAdapter_Plug> var_PortF;
    forte::CPlugPin<FORTE_PortAdapter_Plug> var_PortG;
    forte::CPlugPin<FORTE_PortAdapter_Plug> var_PortH;
    forte::CPlugPin<FORTE_PortAdapter_Plug> var_PortI;
    forte::CPlugPin<FORTE_PortAdapter_Plug> var_PortJ;
    forte::CPlugPin<FORTE_PortAdapter_Plug> var_PortK;

    CEventConnection conn_MAPO;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    forte::IPlugPin *getPlugPinUnchecked(size_t) override;
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
