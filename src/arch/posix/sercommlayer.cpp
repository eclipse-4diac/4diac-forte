/*******************************************************************************
 * Copyright (c) 2012, 2014 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "sercommlayer.h"
#include "commfb.h"
#include "../devlog.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace forte::com_infra;

CSerCommLayer::CSerCommLayer(CComLayer* pa_poUpperLayer, CCommFB * pa_poFB) :
    CComLayer(pa_poUpperLayer, pa_poFB), m_nFD(CFDSelectHandler::scm_nInvalidFileDescriptor), m_unBufFillSize(0){

}

CSerCommLayer::~CSerCommLayer(){
  closeConnection();
}

void CSerCommLayer::closeConnection(){
  if(CFDSelectHandler::scm_nInvalidFileDescriptor != m_nFD){
    CFDSelectHandler::getInstance().removeComCallback(m_nFD);
    tcsetattr(m_nFD, TCSANOW, &m_stOldTIO);
    close(m_nFD);
    m_nFD = CFDSelectHandler::scm_nInvalidFileDescriptor;
  }
}

forte::com_infra::EComResponse CSerCommLayer::sendData(void *pa_pvData, unsigned int pa_unSize){
  if(CFDSelectHandler::scm_nInvalidFileDescriptor != m_nFD){
    ssize_t nToSend = pa_unSize;
    while(0 < nToSend){
      ssize_t nSentBytes = write(m_nFD, pa_pvData, nToSend);
      if(nSentBytes <= 0){
        DEVLOG_ERROR("CSerCommLayer: Send failed: %s\n", strerror(errno));
        return e_ProcessDataSendFailed;
      }
      nToSend -= nSentBytes;
      pa_pvData = static_cast<char*>(pa_pvData) + nSentBytes;
    }
  }

  return e_ProcessDataOk;
}

forte::com_infra::EComResponse CSerCommLayer::recvData(const void *, unsigned int){
  ssize_t nReadCount = read(m_nFD, &m_acRecvBuffer[m_unBufFillSize], cg_unIPLayerRecvBufferSize - m_unBufFillSize);

  switch (nReadCount){
    case 0:
      DEVLOG_INFO("Connection closed by peer\n");
      m_eInterruptResp = e_InitTerminated;
      closeConnection();
      break;
    case -1:
      DEVLOG_ERROR("CSerCommLayer: read failed: %s\n", strerror(errno));
      m_eInterruptResp = e_ProcessDataRecvFaild;
      break;
    default:
      //we successfully received data
      m_unBufFillSize += nReadCount;
      m_eInterruptResp = e_ProcessDataOk;
      break;
  }

  m_poFb->interruptCommFB(this);
  return m_eInterruptResp;
}

forte::com_infra::EComResponse CSerCommLayer::processInterrupt(){
  if(e_ProcessDataOk == m_eInterruptResp){
    m_eInterruptResp = m_poTopLayer->recvData(m_acRecvBuffer, m_unBufFillSize);
    m_unBufFillSize = 0;
  }
  return m_eInterruptResp;
}

forte::com_infra::EComResponse CSerCommLayer::openConnection(char *pa_acLayerParameter){
  EComResponse eRetVal = e_InitInvalidId;

  //as first shot take the serial interface device as param (e.g., /dev/ttyS0 )
  m_nFD = open(pa_acLayerParameter, O_RDWR | O_NOCTTY);

  if(CFDSelectHandler::scm_nInvalidFileDescriptor != m_nFD){
    tcgetattr(m_nFD, &m_stOldTIO);
    struct termios stNewTIO;
    memset(&stNewTIO, 0, sizeof(stNewTIO));

    //TODO make more of these flags setable by the user, current baudrate is suitable for lms usb serial communication
    stNewTIO.c_cflag = B115200 | CS8 | CLOCAL | CREAD;

    stNewTIO.c_iflag = IGNPAR | ICRNL;

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

    tcflush(m_nFD, TCIFLUSH);
    tcsetattr(m_nFD, TCSANOW, &stNewTIO);

    CFDSelectHandler::getInstance().addComCallback(m_nFD, this);
    eRetVal = e_InitOk;

  }
  else{
    DEVLOG_ERROR("CSerCommLayer: open failed: %s\n", strerror(errno));
  }

  return eRetVal;
}

