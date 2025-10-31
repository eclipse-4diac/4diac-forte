/*******************************************************************************
 * Copyright (c) 2012, 2025 ACIN, fortiss GmbH, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "posixsercommlayer.h"
#include "forte/util/devlog.h"
#include "forte/cominfra/commfb.h"
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "forte/arch/sockhand.h"
#include "forte/util/criticalregion.h"
#include "forte/cominfra/comlayersmanager.h"

using namespace forte::literals;

// Missing MacOS flags. Just set them to 0.
#ifndef CMSPAR
#define CMSPAR 0
#endif
#ifndef IUCLC
#define IUCLC 0
#endif

namespace forte::arch {

  namespace {

    [[maybe_unused]] const com_infra::ComLayerManager::EntryImpl<CPosixSerCommLayer> entry("ser"_STRID);

  } // namespace

  CPosixSerCommLayer::CPosixSerCommLayer(CComLayer *paUpperLayer, com_infra::CBaseCommFB *paFB) :
      CSerialComLayerBase(paUpperLayer, paFB) {
  }

  CPosixSerCommLayer::~CPosixSerCommLayer() {
    closeConnection();
  }

  com_infra::EComResponse CPosixSerCommLayer::sendData(void *paData, unsigned int paSize) {
    if (CFDSelectHandler::scmInvalidFileDescriptor != getSerialHandler()) {
      ssize_t nToSend = paSize;
      while (0 < nToSend) {
        ssize_t nSentBytes = write(getSerialHandler(), paData, nToSend);
        if (nSentBytes <= 0) {
          DEVLOG_ERROR("CSerCommLayer: Send failed: %s\n", strerror(errno));
          return com_infra::e_ProcessDataSendFailed;
        }
        nToSend -= nSentBytes;
        paData = static_cast<char *>(paData) + nSentBytes;
      }
    }

    return com_infra::e_ProcessDataOk;
  }

  com_infra::EComResponse CPosixSerCommLayer::recvData(const void *, unsigned int) {
    util::CCriticalRegion lock(mRecvLock);
    ssize_t nReadCount = read(getSerialHandler(), &mRecvBuffer[mBufFillSize], cgIPLayerRecvBufferSize - mBufFillSize);

    switch (nReadCount) {
      case 0:
        DEVLOG_INFO("Connection closed by peer\n");
        mInterruptResp = com_infra::e_InitTerminated;
        closeConnection();
        break;
      case -1:
        DEVLOG_ERROR("CSerCommLayer: read failed: %s\n", strerror(errno));
        mInterruptResp = com_infra::e_ProcessDataRecvFaild;
        break;
      default:
        // we successfully received data
        mBufFillSize += unsigned(nReadCount);
        mInterruptResp = com_infra::e_ProcessDataOk;
        break;
    }

    mFb->interruptCommFB(this);
    return mInterruptResp;
  }

  com_infra::EComResponse CPosixSerCommLayer::openSerialConnection(
      const SSerialParameters &paSerialParameters,
      CSerialComLayerBase<FORTE_SOCKET_TYPE, FORTE_INVALID_SOCKET>::TSerialHandleType *paHandleResult) {

    // as first shot take the serial interface device as param (e.g., /dev/ttyS0 )
    CFDSelectHandler::TFileDescriptor fileDescriptor =
        open(paSerialParameters.mInterfaceName.c_str(), O_RDWR | O_NOCTTY);

    if (fileDescriptor == CFDSelectHandler::scmInvalidFileDescriptor) {
      DEVLOG_ERROR("CSerCommLayer: open failed: %s\n", strerror(errno));
      return com_infra::e_ProcessDataInvalidObject;
    }

    tcgetattr(fileDescriptor, &mOldTIO);
    struct termios newTIO = mOldTIO;
    newTIO.c_cflag = 0;

    speed_t speed = getSpeed(paSerialParameters);
    if (speed == B0) {
      return com_infra::e_InitInvalidId;
    }
    cfsetispeed(&newTIO, speed);
    cfsetospeed(&newTIO, speed);

    switch (paSerialParameters.mByteSize) {
      case e5: newTIO.c_cflag |= CS5; break;
      case e6: newTIO.c_cflag |= CS6; break;
      case e7: newTIO.c_cflag |= CS7; break;
      case e8: newTIO.c_cflag |= CS8; break;
      default: return com_infra::e_InitInvalidId; break;
    }

    switch (paSerialParameters.mStopBits) {
      case eOneBit: newTIO.c_cflag &= ~CSTOPB; break;
      case eTwoBits: newTIO.c_cflag |= CSTOPB; break;
      default: return com_infra::e_InitInvalidId; break;
    }

    switch (paSerialParameters.mParity) {
      case eNoParity: newTIO.c_cflag &= ~(PARENB | PARODD | CMSPAR); break;
      case eODD: newTIO.c_cflag |= PARENB | PARODD; break;
      case eEven:
        newTIO.c_cflag |= PARENB;
        newTIO.c_cflag &= ~PARODD;
        break;
      default: return com_infra::e_InitInvalidId; break;
    }

    newTIO.c_cflag &= ~CRTSCTS; // no hardware flow control

    newTIO.c_cflag |= (CLOCAL | CREAD); /* Local line - do not change "owner" of port |  Enable receiver*/

    newTIO.c_iflag |= IGNPAR; /* Map CR to NL | IGNPAR (was here before)  Ignore parity error. TODO: Should we
                                   delete this? It was on the old code*/

    newTIO.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    newTIO.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | INPCK |
                        IUCLC); // Disable any special handling of received bytes

    newTIO.c_oflag &= ~(OPOST | ONLCR | OCRNL);

    newTIO.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL | ECHOK | ISIG | IEXTEN | CRTSCTS);

    newTIO.c_cc[VINTR] = _POSIX_VDISABLE; /* Ctrl-c */
    newTIO.c_cc[VQUIT] = _POSIX_VDISABLE; /* Ctrl-\ */
    newTIO.c_cc[VERASE] = _POSIX_VDISABLE; /* del */
    newTIO.c_cc[VKILL] = _POSIX_VDISABLE; /* @ */
    newTIO.c_cc[VEOF] = _POSIX_VDISABLE; /* Ctrl-d */
    newTIO.c_cc[VTIME] = 10; /* inter-character timer unused */
    newTIO.c_cc[VMIN] = 1; /* blocking read until 1 character arrives */
#ifdef VSWTC
    newTIO.c_cc[VSWTC] = _POSIX_VDISABLE; /* '\0' */
#endif
    newTIO.c_cc[VSTART] = _POSIX_VDISABLE; /* Ctrl-q */
    newTIO.c_cc[VSTOP] = _POSIX_VDISABLE; /* Ctrl-s */
    newTIO.c_cc[VSUSP] = _POSIX_VDISABLE; /* Ctrl-z */
    newTIO.c_cc[VEOL] = _POSIX_VDISABLE; /* '\0' */
    newTIO.c_cc[VREPRINT] = _POSIX_VDISABLE; /* Ctrl-r */
    newTIO.c_cc[VDISCARD] = _POSIX_VDISABLE; /* Ctrl-u */
    newTIO.c_cc[VWERASE] = _POSIX_VDISABLE; /* Ctrl-w */
    newTIO.c_cc[VLNEXT] = _POSIX_VDISABLE; /* Ctrl-v */
    newTIO.c_cc[VEOL2] = _POSIX_VDISABLE; /* '\0' */

    tcflush(fileDescriptor, TCIFLUSH);
    tcsetattr(fileDescriptor, TCSANOW, &newTIO);

    getExtEvHandler<CFDSelectHandler>().addComCallback(fileDescriptor, this);
    *paHandleResult = fileDescriptor;
    return com_infra::e_InitOk;
  }

  void CPosixSerCommLayer::closeConnection() {
    CFDSelectHandler::TFileDescriptor fileDescriptor = getSerialHandler();
    if (CFDSelectHandler::scmInvalidFileDescriptor != fileDescriptor) {
      getExtEvHandler<CFDSelectHandler>().removeComCallback(fileDescriptor);
      tcsetattr(fileDescriptor, TCSANOW, &mOldTIO);
      close(fileDescriptor);
    }
  }

  speed_t CPosixSerCommLayer::getSpeed(const SSerialParameters &paSerialParameters) {
    speed_t speed;
    switch (paSerialParameters.mBaudRate) {
      case e50: speed = B50; break;
      case e75: speed = B75; break;
      case e110: speed |= B110; break;
      case e134C5: speed = B134; break;
      case e150: speed = B150; break;
      case e200: speed = B200; break;
      case e300: speed = B300; break;
      case e600: speed = B600; break;
      case e1200: speed = B1200; break;
      case e1800: speed = B1800; break;
      case e2400: speed = B2400; break;
      case e4800: speed = B4800; break;
      case e9600: speed = B9600; break;
      case e19200: speed = B19200; break;
      case e38400: speed = B38400; break;
      case e57600: speed = B57600; break;
      case e115200: speed = B115200; break;
#ifdef B1000000
      case e1000000: speed = B1000000; break;
#endif
      default: speed = B0; break;
    }
    return speed;
  }
} // namespace forte::arch
