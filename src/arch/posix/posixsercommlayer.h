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
#ifndef _SERCOMMLAYER_H_
#define _SERCOMMLAYER_H_

#include <termios.h>
#include "../../core/cominfra/serialcomlayerbase.h"
#include "../gensockhand.h"

class CPosixSerCommLayer : public CSerialComLayerBase<FORTE_SOCKET_TYPE, FORTE_INVALID_SOCKET>{
  public:
    CPosixSerCommLayer(forte::com_infra::CComLayer* paUpperLayer, forte::com_infra::CBaseCommFB * paFB);
    virtual ~CPosixSerCommLayer();

    virtual forte::com_infra::EComResponse sendData(void *paData, unsigned int paSize);
    virtual forte::com_infra::EComResponse recvData(const void *paData, unsigned int paSize);

  protected:
  private:
    virtual forte::com_infra::EComResponse openSerialConnection(const SSerialParameters& paSerialParameters, CSerialComLayerBase<FORTE_SOCKET_TYPE, FORTE_INVALID_SOCKET>::TSerialHandleType* paHandleResult);
    virtual void closeConnection();

    struct termios mOldTIO;    //!< buffer for the existing sercom settings
};

#endif /* CSERCOMMLAYER_H_ */
