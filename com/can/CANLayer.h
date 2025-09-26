/*******************************************************************************
 * Copyright (c) 2025 Malte Grave, LIT CPS Johannes Kepler Universität,
 *                                 github.com/gravemalte
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Malte Grave, Alexander Trojnin - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include <string>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>

#include "forte/arch/sockhand.h"
#include "forte/arch/gensockhand.h"
#include "forte/cominfra/comlayer.h"
#include "forte/datatype.h"
#include "forte/util/parameterParser.h"

using namespace forte::arch;
using namespace forte::util;

namespace forte::com_infra {

  class CCANComLayer : public CComLayer {
    public:
      typedef FORTE_SOCKET_TYPE TCANSocketHandle;

      CCANComLayer(CComLayer *paUpperLayer, CBaseCommFB *paComFB);
      ~CCANComLayer() override;

      EComResponse sendData(void *paData, unsigned int paSize) override;
      EComResponse recvData(const void *paData, unsigned int paSize) override;

      EComResponse processInterrupt() override;

    private:
      struct sockaddr_can mAddr;
      struct ifreq mIfr;
      struct can_frame mFrame;
      struct can_filter rfilter;

      CFDSelectHandler::TFileDescriptor mCANSocket = CFDSelectHandler::scmInvalidFileDescriptor;

      EComResponse m_eInterruptResp;

      EComResponse openConnection(char *paLayerParameter) override;
      void closeConnection() override;
  };

  namespace {
    enum ECANCommunicationParameter { eInterface = 0, eCANId = 1, eCANComParamterAmount = 2 };

    struct SCANParameters {
        std::string interfaceName;
        TForteUInt32 CANId;
    };
    SCANParameters setupCANInternals(CParameterParser &parser);

  } // namespace

} // namespace forte::com_infra
