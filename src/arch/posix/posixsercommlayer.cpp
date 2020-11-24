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
#include "posixsercommlayer.h"
#include "../devlog.h"
#include "../../core/cominfra/commfb.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sockhand.h>
#include <criticalregion.h>

CPosixSerCommLayer::CPosixSerCommLayer(forte::com_infra::CComLayer* paUpperLayer, forte::com_infra::CBaseCommFB* paFB) :
   CSerialComLayerBase(paUpperLayer, paFB){
}

CPosixSerCommLayer::~CPosixSerCommLayer(){
  closeConnection();
}

forte::com_infra::EComResponse CPosixSerCommLayer::sendData(void *paData, unsigned int paSize){
  if(CFDSelectHandler::scmInvalidFileDescriptor != getSerialHandler()){
    ssize_t nToSend = paSize;
    while(0 < nToSend){
      ssize_t nSentBytes = write(getSerialHandler(), paData, nToSend);
      if(nSentBytes <= 0){
        DEVLOG_ERROR("CSerCommLayer: Send failed: %s\n", strerror(errno));
        return forte::com_infra::e_ProcessDataSendFailed;
      }
      nToSend -= nSentBytes;
      paData = static_cast<char*>(paData) + nSentBytes;
    }
  }

  return forte::com_infra::e_ProcessDataOk;
}

forte::com_infra::EComResponse CPosixSerCommLayer::recvData(const void *, unsigned int){
  CCriticalRegion lock(mRecvLock);
  ssize_t nReadCount = read(getSerialHandler(), &mRecvBuffer[mBufFillSize], cg_unIPLayerRecvBufferSize - mBufFillSize);

  switch (nReadCount){
    case 0:
      DEVLOG_INFO("Connection closed by peer\n");
      mInterruptResp = forte::com_infra::e_InitTerminated;
      closeConnection();
      break;
    case -1:
      DEVLOG_ERROR("CSerCommLayer: read failed: %s\n", strerror(errno));
      mInterruptResp = forte::com_infra::e_ProcessDataRecvFaild;
      break;
    default:
      //we successfully received data
      mBufFillSize += nReadCount;
      mInterruptResp = forte::com_infra::e_ProcessDataOk;
      break;
  }

  m_poFb->interruptCommFB(this);
  return mInterruptResp;
}

forte::com_infra::EComResponse CPosixSerCommLayer::openSerialConnection(const SSerialParameters& paSerialParameters, CSerialComLayerBase<FORTE_SOCKET_TYPE, FORTE_INVALID_SOCKET>::TSerialHandleType* paHandleResult){
  forte::com_infra::EComResponse eRetVal = forte::com_infra::e_ProcessDataNoSocket;

  //as first shot take the serial interface device as param (e.g., /dev/ttyS0 )
  CFDSelectHandler::TFileDescriptor fileDescriptor = open(paSerialParameters.interfaceName.getValue(), O_RDWR | O_NOCTTY);

  if(CFDSelectHandler::scmInvalidFileDescriptor != fileDescriptor){
    tcgetattr(fileDescriptor, &mOldTIO);
    struct termios stNewTIO;
    memset(&stNewTIO, 0, sizeof(stNewTIO));

    stNewTIO.c_line = mOldTIO.c_line;

    switch (paSerialParameters.baudRate) {
      case e50: stNewTIO.c_cflag |= B50;  break;
      case e75: stNewTIO.c_cflag |= B75;  break;
      case e110: stNewTIO.c_cflag |= B110;  break;
      case e134C5: stNewTIO.c_cflag |= B134;  break;
      case e150: stNewTIO.c_cflag |= B150;  break;
      case e200: stNewTIO.c_cflag |= B200;  break;
      case e300: stNewTIO.c_cflag |= B300; break;
      case e600: stNewTIO.c_cflag |= B600; break;
      case e1200: stNewTIO.c_cflag |= B1200; break;
      case e1800: stNewTIO.c_cflag |= B1800; break;
      case e2400: stNewTIO.c_cflag |= B2400; break;
      case e4800: stNewTIO.c_cflag |= B4800; break;
      case e9600: stNewTIO.c_cflag |= B9600; break;
      case e19200: stNewTIO.c_cflag |= B19200; break;
      case e38400: stNewTIO.c_cflag |= B38400; break;
      case e57600: stNewTIO.c_cflag |= B57600; break;
      case e115200: stNewTIO.c_cflag |= B115200; break;
      case e1000000: stNewTIO.c_cflag |= B1000000; break;
      default: return forte::com_infra::e_InitInvalidId; break;
    }

    cfsetispeed(&stNewTIO, cfgetispeed(&mOldTIO));
    cfsetospeed(&stNewTIO, cfgetospeed(&mOldTIO));

    switch (paSerialParameters.byteSize) {
      case e5: stNewTIO.c_cflag |= CS5;  break;
      case e6: stNewTIO.c_cflag |= CS6;  break;
      case e7: stNewTIO.c_cflag |= CS7;  break;
      case e8: stNewTIO.c_cflag |= CS8;  break;
      default: return forte::com_infra::e_InitInvalidId; break;
    }

    switch(paSerialParameters.stopBits){
       case eOneBit: stNewTIO.c_cflag &= ~CSTOPB;  break;
       case eTwoBits: stNewTIO.c_cflag |= CSTOPB;  break;
       default: return forte::com_infra::e_InitInvalidId; break;
     }

    switch(paSerialParameters.parity){
      case eNoParity:
        stNewTIO.c_cflag &= ~(PARENB | PARODD | CMSPAR);
        break;
      case eODD: stNewTIO.c_cflag |= PARENB | PARODD;  break;
      case eEven:
        stNewTIO.c_cflag |= PARENB;
        stNewTIO.c_cflag &= ~PARODD;
        break;
      default: return forte::com_infra::e_InitInvalidId; break;
    }

    stNewTIO.c_cflag &= ~CRTSCTS;  // no hardware flow control

    stNewTIO.c_cflag |= (CLOCAL | CREAD); /* Local line - do not change "owner" of port |  Enable receiver*/

    stNewTIO.c_iflag = mOldTIO.c_iflag;
    stNewTIO.c_iflag |= IGNPAR; /* Map CR to NL | IGNPAR (was here before)  Ignore parity error. TODO: Should we delete this? It was on the old code*/ ;
    stNewTIO.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    stNewTIO.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | INPCK | IUCLC); // Disable any special handling of received bytes

    stNewTIO.c_oflag = mOldTIO.c_oflag;
    stNewTIO.c_oflag &= ~(OPOST | ONLCR | OCRNL);

    stNewTIO.c_lflag = mOldTIO.c_lflag;
    stNewTIO.c_lflag &= ~(ICANON| ECHO | ECHOE| ECHONL | ECHOK | ISIG | IEXTEN | CRTSCTS);

    stNewTIO.c_cc[VINTR] = _POSIX_VDISABLE; /* Ctrl-c */
    stNewTIO.c_cc[VQUIT] = _POSIX_VDISABLE; /* Ctrl-\ */
    stNewTIO.c_cc[VERASE] = _POSIX_VDISABLE; /* del */
    stNewTIO.c_cc[VKILL] = _POSIX_VDISABLE; /* @ */
    stNewTIO.c_cc[VEOF] = _POSIX_VDISABLE; /* Ctrl-d */
    stNewTIO.c_cc[VTIME] = 10; /* inter-character timer unused */
    stNewTIO.c_cc[VMIN] = 1; /* blocking read until 1 character arrives */
    stNewTIO.c_cc[VSWTC] = _POSIX_VDISABLE; /* '\0' */
    stNewTIO.c_cc[VSTART] = _POSIX_VDISABLE; /* Ctrl-q */
    stNewTIO.c_cc[VSTOP] = _POSIX_VDISABLE; /* Ctrl-s */
    stNewTIO.c_cc[VSUSP] = _POSIX_VDISABLE; /* Ctrl-z */
    stNewTIO.c_cc[VEOL] = _POSIX_VDISABLE; /* '\0' */
    stNewTIO.c_cc[VREPRINT] = _POSIX_VDISABLE; /* Ctrl-r */
    stNewTIO.c_cc[VDISCARD] = _POSIX_VDISABLE; /* Ctrl-u */
    stNewTIO.c_cc[VWERASE] = _POSIX_VDISABLE; /* Ctrl-w */
    stNewTIO.c_cc[VLNEXT] = _POSIX_VDISABLE; /* Ctrl-v */
    stNewTIO.c_cc[VEOL2] = _POSIX_VDISABLE; /* '\0' */

    tcflush(fileDescriptor, TCIFLUSH);
    tcsetattr(fileDescriptor, TCSANOW, &stNewTIO);

    getExtEvHandler<CFDSelectHandler>().addComCallback(fileDescriptor, this);
    *paHandleResult = fileDescriptor;
    eRetVal = forte::com_infra::e_InitOk;

  }
  else{
    eRetVal = forte::com_infra::e_ProcessDataInvalidObject;
    DEVLOG_ERROR("CSerCommLayer: open failed: %s\n", strerror(errno));
  }

  return eRetVal;
}

void CPosixSerCommLayer::closeConnection(){
  CFDSelectHandler::TFileDescriptor fileDescriptor = getSerialHandler();
  if(CFDSelectHandler::scmInvalidFileDescriptor != fileDescriptor){
    getExtEvHandler<CFDSelectHandler>().removeComCallback(fileDescriptor);
    tcsetattr(fileDescriptor, TCSANOW, &mOldTIO);
    close(fileDescriptor);
  }
}

