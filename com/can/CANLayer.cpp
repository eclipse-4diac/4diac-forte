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

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <memory>
#include <string>
#include <linux/can.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "forte/stringid.h"
#include "forte/cominfra/basecommfb.h"
#include "forte/cominfra/comlayersmanager.h"
#include "forte/cominfra/comtypes.h"
#include "forte/datatype.h"
#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/util/devlog.h"

#include "CANLayer.h"
#include "CANHandler.h"

using namespace forte::arch;
using namespace forte::literals;
using namespace forte::util;

namespace forte::com_infra {
  namespace {
    [[maybe_unused]] const ComLayerManager::EntryImpl<CCANComLayer> entry("can"_STRID);
  }

  CCANComLayer::CCANComLayer(CComLayer *paUpperLayer, CBaseCommFB *paComFB) : CComLayer(paUpperLayer, paComFB) {
  }

  EComResponse CCANComLayer::openConnection(char *paLayerParameter) {
    EComResponse eRetVal = e_InitInvalidId;
    CParameterParser parser(paLayerParameter, ';', eCANComParamterAmount);
    if (eCANComParamterAmount != parser.parseParameters()) {
      DEVLOG_ERROR("[CAN Layer] Parsing of the parameters failed.\n");
      return eRetVal;
    }

    SCANParameters can_params = setupCANInternals(parser);

    DEVLOG_DEBUG("[CAN Layer] Opening connection\n");

    CFDSelectHandler::TFileDescriptor fd = socket(PF_CAN, SOCK_RAW | SOCK_NONBLOCK, CAN_RAW);

    switch (mFb->getComServiceType()) {
      case e_Server:
        DEVLOG_ERROR("[CAN Layer] the layer only supports the PUB/SUB "
                     "pattern. Please use a Publisher block to send data.");
        eRetVal = e_InitTerminated;
        break;
      case e_Client:
        DEVLOG_ERROR("[CAN Layer] the layer only supports the PUB/SUB "
                     "pattern. Please use a Subscribe block to receive data.");
        eRetVal = e_InitTerminated;
        break;
      case e_Publisher:
        // is only sendData
        mFrame.can_id = can_params.CANId;
        break;
      case e_Subscriber:
        // is only recvData
        rfilter.can_id = can_params.CANId;
        rfilter.can_mask = CAN_SFF_MASK;
        setsockopt(fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
        break;

      default:
        DEVLOG_ERROR("[CAN Layer] Invalid communication service type.\n");
        eRetVal = e_InitTerminated;
        break;
    }

    if (eRetVal != e_InitInvalidId) {
      return eRetVal;
    }

    if (CFDSelectHandler::scmInvalidFileDescriptor == fd) {
      DEVLOG_ERROR("[CAN Layer] Socket can't be created: %s\n", strerror(errno));
      return e_InitInvalidId;
    }

    std::strncpy(mIfr.ifr_name, can_params.interfaceName.c_str(), IFNAMSIZ - 1);
    mIfr.ifr_name[IFNAMSIZ - 1] = '\0'; // Ensure that null termination is added if input is too long.

    if (-1 == ioctl(fd, SIOCGIFINDEX, &mIfr)) {
      DEVLOG_ERROR("[CAN Layer] Socket cannot be controlled. %s\n", strerror(errno));
      close(fd);
      return eRetVal;
    }

    mCANSocket = fd;
    mAddr.can_family = AF_CAN;
    mAddr.can_ifindex = mIfr.ifr_ifindex;

    if (-1 == bind(mCANSocket, (struct sockaddr *) &mAddr, sizeof(mAddr))) {
      DEVLOG_ERROR("[CAN Layer] Binding of the socket unsuccessful: %s\n", strerror(errno));
      close(mCANSocket);
      return eRetVal;
    }

    this->getExtEvHandler<CCANHandler>().addComCallback(mCANSocket, this);
    DEVLOG_DEBUG("[CAN Layer] Callback handler added.\n");

    eRetVal = e_InitOk;
    return eRetVal;
  }

  CCANComLayer::~CCANComLayer() {
  }

  void CCANComLayer::closeConnection() {
    if (mCANSocket != CFDSelectHandler::scmInvalidFileDescriptor) {
      DEVLOG_DEBUG("[CAN Layer] Closing socket\n");
      close(mCANSocket);
      DEVLOG_DEBUG("[CAN Layer] Unregister layer\n");
      this->getExtEvHandler<CCANHandler>().removeComCallback(mCANSocket);
    }
  }

  EComResponse CCANComLayer::sendData(void *paData, unsigned int) {
    EComResponse eRetVal = e_InitOk;

    CIEC_ANY **apoSDs = mFb->getSDs();
    size_t size = mFb->getNumSD();

    auto data = std::make_unique<uint8_t[]>(size);

    for (size_t i = 0; i < size; i++) {
      CIEC_ANY &sd(apoSDs[i]->unwrap());
      data[i] = *sd.getConstDataPtr();
      DEVLOG_DEBUG("[CAN Layer] send data[%d] = %d\n", i, data[i]);
    }

    eRetVal = e_ProcessDataOk;

    size_t offset = 0;
    while (offset < size && eRetVal == e_ProcessDataOk) {

      mFrame.can_dlc = static_cast<uint8_t>((size - offset > CAN_MAX_DLEN) ? CAN_MAX_DLEN : (size - offset));

      // Clear old data and copy new chunk
      std::memset(mFrame.data, 0, CAN_MAX_DLEN);
      std::memcpy(mFrame.data, &data[offset], mFrame.can_dlc);

      offset += mFrame.can_dlc;

      DEVLOG_DEBUG("[CAN Layer] Sending data ...\n");

      if (write(mCANSocket, &mFrame, sizeof(struct can_frame)) < 0) {
        DEVLOG_ERROR("CAN send failed: %s\n", strerror(errno));
        eRetVal = e_ProcessDataSendFailed;
        break;
      }
    }

    return e_ProcessDataOk;
  }

  EComResponse CCANComLayer::recvData(const void *paData, unsigned int paSize) {
    DEVLOG_DEBUG("[CAN Layer] Receive data.\n");

    m_eInterruptResp = e_Nothing;

    if (mFb->getComServiceType() == e_Subscriber) {
      const can_frame *frame = (can_frame *) (paData);
      memcpy(&mFrame, frame, sizeof(struct can_frame));

      mFb->interruptCommFB(this);
      m_eInterruptResp = e_ProcessDataOk;
    }

    return m_eInterruptResp;
  }

  EComResponse CCANComLayer::processInterrupt() {
    DEVLOG_DEBUG("[CAN Layer] Interupt process.\n");

    if (m_eInterruptResp == e_ProcessDataOk) {
      CIEC_ANY **apoRDs = mFb->getRDs();
      size_t size_rd = mFb->getNumRD();

      for (size_t i = 0; i < size_rd; i++) {
        CIEC_ANY &rd(apoRDs[i]->unwrap());

        if (i >= mFrame.can_dlc) {
          rd.setValue(CIEC_BYTE(0));
        } else {
          rd.setValue(CIEC_BYTE(mFrame.data[i]));
        }
      }
    }

    return m_eInterruptResp;
  }

  namespace {
    SCANParameters setupCANInternals(CParameterParser &parser) {
      SCANParameters can_params;

      can_params.interfaceName = std::string(parser[eInterface], strlen(parser[eInterface]));

      can_params.CANId = static_cast<TForteUInt32>(std::strtoul(parser[eCANId], nullptr, 10));

      DEVLOG_DEBUG("[CAN Layer] Using interface: %s and CAN-ID: %u\n", can_params.interfaceName.c_str(),
                   can_params.CANId);

      return can_params;
    }
  } // namespace

} // namespace forte::com_infra
