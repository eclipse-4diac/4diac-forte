/*************************************************************************
 *** Copyright (c) 2013 fortiss GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.0.0.202511261532!
 ***
 *** Name: E_TimeOut
 *** Description: Simple implementation of the timeout services
 *** Version:
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 *************************************************************************/

#pragma once

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "forte/datatypes/forte_time.h"
#include "forte/iec61499/events/ATimeOut_adp.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/iec61499/events/E_DELAY_fbt.h"

namespace forte::iec61499::events {
  class FORTE_E_TimeOut final : public CCompositeFB {
      DECLARE_FIRMWARE_FB(FORTE_E_TimeOut)

    private:
      CInternalFB<forte::iec61499::events::FORTE_E_DELAY> fb_DLY;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_E_TimeOut(StringId paInstanceNameId, CFBContainer &paContainer);

      forte::CSocketPin<forte::iec61499::events::FORTE_ATimeOut_Socket> var_TimeOutSocket;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      forte::ISocketPin *getSocketPinUnchecked(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
      CDataConnection *getIf2InConUnchecked(TPortId) override;
  };
} // namespace forte::iec61499::events
