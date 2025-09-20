/*******************************************************************************
 * Copyright (c) 2012, 2017 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include <termios.h>
#include "forte/cominfra/serialcomlayerbase.h"
#include "forte/arch/gensockhand.h"

namespace forte::arch {
  class CPosixSerCommLayer : public CSerialComLayerBase<FORTE_SOCKET_TYPE, FORTE_INVALID_SOCKET> {
    public:
      CPosixSerCommLayer(CComLayer *paUpperLayer, forte::com_infra::CBaseCommFB *paFB);
      ~CPosixSerCommLayer() override;

      forte::com_infra::EComResponse sendData(void *paData, unsigned int paSize) override;
      forte::com_infra::EComResponse recvData(const void *paData, unsigned int paSize) override;

    protected:
    private:
      forte::com_infra::EComResponse openSerialConnection(
          const SSerialParameters &paSerialParameters,
          CSerialComLayerBase<FORTE_SOCKET_TYPE, FORTE_INVALID_SOCKET>::TSerialHandleType *paHandleResult) override;
      void closeConnection() override;

      struct termios mOldTIO; //!< buffer for the existing sercom settings
  };
} // namespace forte::arch
