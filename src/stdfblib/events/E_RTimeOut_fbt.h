/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
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

#include "core/cfb.h"
#include "core/typelib.h"
#include "ARTimeOut_adp.h"
#include "core/datatypes/forte_time.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"
#include "E_RDELAY_fbt.h"

class FORTE_E_RTimeOut final : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_E_RTimeOut)

  private:
    forte::core::CInternalFB<FORTE_E_RDELAY> fb_DLY;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_E_RTimeOut(forte::core::StringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    forte::CSocketPin<FORTE_ARTimeOut_Socket> var_TimeOutSocket;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    forte::ISocketPin *getSocketPinUnchecked(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;
    CDataConnection *getIf2InConUnchecked(TPortId) override;
};
