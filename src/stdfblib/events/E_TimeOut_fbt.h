/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "core/adapterconn.h"
#include "core/esfb.h"
#include "core/typelib.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"
#include "ATimeOut_adp.h"

#include "arch/timerha.h"

class FORTE_E_TimeOut final : public CEventSourceFB {
    DECLARE_FIRMWARE_FB(FORTE_E_TimeOut)

  private:
    static const int scmTimeOutSocketAdpNum = 0;


    bool mActive; //!> flag to indicate that the timed fb is currently active

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;

  public:
    FORTE_E_TimeOut(forte::core::StringId paInstanceNameId, forte::core::CFBContainer &paContainer);
    bool initialize() override;

    forte::CSocketPin<FORTE_ATimeOut_Socket> var_TimeOutSocket;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    forte::ISocketPin *getSocketPinUnchecked(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;

  protected:
    void setInitialValues() override;
};
