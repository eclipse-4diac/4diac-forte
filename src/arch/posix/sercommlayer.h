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
#ifndef _SERCOMMLAYER_H_
#define _SERCOMMLAYER_H_

#include "comlayer.h"
#include "../fdselecthand.h"
#include <termios.h>

class CSerCommLayer : public forte::com_infra::CComLayer{
  public:
    CSerCommLayer(forte::com_infra::CComLayer* pa_poUpperLayer, forte::com_infra::CCommFB * pa_poFB);
    virtual ~CSerCommLayer();

    virtual void closeConnection();
    virtual forte::com_infra::EComResponse sendData(void *pa_pvData, unsigned int pa_unSize);
    virtual forte::com_infra::EComResponse recvData(const void *pa_pvData, unsigned int pa_unSize);

    virtual forte::com_infra::EComResponse processInterrupt();

  protected:
  private:
    virtual forte::com_infra::EComResponse openConnection(char *pa_acLayerParameter);

    CFDSelectHandler::TFileDescriptor m_nFD; //!< file descriptor for accessing the serial device

    forte::com_infra::EComResponse m_eInterruptResp;
    char m_acRecvBuffer[cg_unIPLayerRecvBufferSize];
    unsigned int m_unBufFillSize;
    struct termios m_stOldTIO;    //!< buffer for the existing sercom settings

};

#endif /* CSERCOMMLAYER_H_ */
