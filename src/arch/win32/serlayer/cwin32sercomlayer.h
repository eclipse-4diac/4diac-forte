/*******************************************************************************
 * Copyright (c) 2013, 2014 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Martin Melik-Merkumians, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "../../../core/cominfra/comlayer.h"
#include "../../../core/cominfra/commfb.h"

#include <winsock2.h> //to ensure that this is first included in the commlayermanager
#include <Windows.h>

#ifndef CWIN32SERCOMLAYER_H_
#define CWIN32SERCOMLAYER_H_

class CWin32SerComLayer : public forte::com_infra::CComLayer{
  public:
    CWin32SerComLayer(forte::com_infra::CComLayer* paUpperLayer, forte::com_infra::CBaseCommFB * paFB);
    virtual ~CWin32SerComLayer ();

    virtual forte::com_infra::EComResponse processInterrupt();

    /*! \brief Perform send to serial interface
    *   \param pa_pvData Sendable payload
    *   \param pa_unSize Payload size in bytes
    *
    *   \return ComLayer response
    */
    virtual forte::com_infra::EComResponse sendData(void *paData, unsigned int paSize);
    
    /*! \brief Perform reading from serial interface
     *
     * @return if not e_Nothing something was read and the FB should get an external event
     */
    virtual forte::com_infra::EComResponse recvData(const void *paData, unsigned int paSize);

  protected:
  private:
	char mTerminationSymbol[3]; //**< Space for CR, LF, or CR/LF + Terminating \0
    virtual forte::com_infra::EComResponse openConnection(char *paLayerParameter);
    virtual void closeConnection();
    HANDLE mSerial;
	static const unsigned int mMaxRecvBuffer = 22;
	static const unsigned int mNoOfParameters = 6;

    forte::com_infra::EComResponse mInterruptResp;
    char mRecvBuffer[mMaxRecvBuffer];
    unsigned int mBufFillSize;

    enum ESerComParameter{
      eInterface = 0, eBaudrate, eByteSize, eStopBits, eParity, eTerminationSymbol
    };

};

#endif /* CWIN32SERCOMLAYER_H_ */
