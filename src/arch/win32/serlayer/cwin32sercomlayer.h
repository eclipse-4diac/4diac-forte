/*******************************************************************************
 * Copyright (c) 2013, 2017 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <winsock2.h> //to ensure that this is first included in the commlayermanager
#include <windows.h>

#include "../../../core/cominfra/serialcomlayerbase.h"

#ifndef CWIN32SERCOMLAYER_H_
#define CWIN32SERCOMLAYER_H_

class CWin32SerComLayer : public CSerialComLayerBase<HANDLE>{
  public:
    CWin32SerComLayer(forte::com_infra::CComLayer* paUpperLayer, forte::com_infra::CBaseCommFB * paFB);
    virtual ~CWin32SerComLayer ();

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
    virtual forte::com_infra::EComResponse openSerialConnection(const SSerialParameters& paSerialParameters, CSerialComLayerBase<HANDLE>::TSerialHandleType* paHandleResult);
    virtual void closeConnection();
};

#endif /* CWIN32SERCOMLAYER_H_ */
