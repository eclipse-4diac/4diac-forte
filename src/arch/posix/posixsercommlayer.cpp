/*******************************************************************************
 * Copyright (c) 2012, 2017 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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

CPosixSerCommLayer::CPosixSerCommLayer(forte::com_infra::CComLayer* paUpperLayer, forte::com_infra::CBaseCommFB* paFB) :
   CSerialComLayerBase(paUpperLayer, paFB){
}

CPosixSerCommLayer::~CPosixSerCommLayer(){
  closeConnection();
}

forte::com_infra::EComResponse CPosixSerCommLayer::sendData(void *paData, unsigned int paSize){
  if(CFDSelectHandler::scm_nInvalidFileDescriptor != getSerialHandler()){
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

forte::com_infra::EComResponse CPosixSerCommLayer::openSerialConnection(const SSerialParameters& paSerialParameters, CSerialComLayerBase<CFDSelectHandler::TFileDescriptor, CFDSelectHandler::scm_nInvalidFileDescriptor>::TSerialHandleType* paHandleResult){
  forte::com_infra::EComResponse eRetVal = forte::com_infra::e_ProcessDataNoSocket;

  //as first shot take the serial interface device as param (e.g., /dev/ttyS0 )
  CFDSelectHandler::TFileDescriptor fileDescriptor = open(paSerialParameters.interfaceName.getValue(), O_RDWR | O_NOCTTY);

  if(CFDSelectHandler::scm_nInvalidFileDescriptor != fileDescriptor){
    tcgetattr(fileDescriptor, &mOldTIO);
    struct termios stNewTIO;
    memset(&stNewTIO, 0, sizeof(stNewTIO));


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
      default: return forte::com_infra::e_InitInvalidId; break;
    }

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
      case eNoParity: stNewTIO.c_cflag &= ~PARENB;  break;
      case eODD: stNewTIO.c_cflag |= PARENB | PARODD;  break;
      case eEven:
        stNewTIO.c_cflag |= PARENB;
        stNewTIO.c_cflag &= ~PARODD;
        break;
      default: return forte::com_infra::e_InitInvalidId; break;
    }


    stNewTIO.c_cflag = CLOCAL | CREAD; /* Local line - do not change "owner" of port |  Enable receiver*/

    stNewTIO.c_iflag = ICRNL; /* Map CR to NL | IGNPAR (was here before)  Ignore parity error. TODO: Should we delete this? It was on the old code*/ ;

    stNewTIO.c_oflag = 0; //raw output

    stNewTIO.c_lflag = ICANON; // enable canonical input, disable echo func and don't send signals to calling programm

    stNewTIO.c_cc[VINTR] = 0; /* Ctrl-c */
    stNewTIO.c_cc[VQUIT] = 0; /* Ctrl-\ */
    stNewTIO.c_cc[VERASE] = 0; /* del */
    stNewTIO.c_cc[VKILL] = 0; /* @ */
    stNewTIO.c_cc[VEOF] = 4; /* Ctrl-d */
    stNewTIO.c_cc[VTIME] = 0; /* inter-character timer unused */
    stNewTIO.c_cc[VMIN] = 1; /* blocking read until 1 character arrives */
    stNewTIO.c_cc[VSWTC] = 0; /* '\0' */
    stNewTIO.c_cc[VSTART] = 0; /* Ctrl-q */
    stNewTIO.c_cc[VSTOP] = 0; /* Ctrl-s */
    stNewTIO.c_cc[VSUSP] = 0; /* Ctrl-z */
    stNewTIO.c_cc[VEOL] = 0; /* '\0' */
    stNewTIO.c_cc[VREPRINT] = 0; /* Ctrl-r */
    stNewTIO.c_cc[VDISCARD] = 0; /* Ctrl-u */
    stNewTIO.c_cc[VWERASE] = 0; /* Ctrl-w */
    stNewTIO.c_cc[VLNEXT] = 0; /* Ctrl-v */
    stNewTIO.c_cc[VEOL2] = 0; /* '\0' */

    tcflush(fileDescriptor, TCIFLUSH);
    tcsetattr(fileDescriptor, TCSANOW, &stNewTIO);

    GET_HANDLER_FROM_COMM_LAYER(CFDSelectHandler)->addComCallback(fileDescriptor, this);
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
  if(CFDSelectHandler::scm_nInvalidFileDescriptor != fileDescriptor){
    GET_HANDLER_FROM_COMM_LAYER(CFDSelectHandler)->removeComCallback(fileDescriptor);
    tcsetattr(fileDescriptor, TCSANOW, &mOldTIO);
    close(fileDescriptor);
  }
}

