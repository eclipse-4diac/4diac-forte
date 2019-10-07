/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial implementation and rework communication infrastructure
 *******************************************************************************/

#ifndef SRC_CORE_COMINFRA_COMCALLBACK_H_
#define SRC_CORE_COMINFRA_COMCALLBACK_H_

#include "comtypes.h"

namespace forte {
  namespace com_infra{

class CBaseCommFB;

class CComCallback {

  public:

    virtual ~CComCallback() {};

    /*!\brief Take the given data and perform the necessary process for receiving data
     *
     * This function is called for processing the received data from bottom to top. Therefore
     * if necessary invoke the top layer's receiveData function to hand on the processed data.
     *
     * @param paData pointer to the data received
     * @param paSize size of the data received
     * @return status of the receiving process depends on if the layer is the bottom most layer:
     *    - Normal Layer:
     *        - e_ProcessDataOk ... if receiving process was successful
     *    - Bottom most layer
     *        - e_Nothing ...  if the processing of the data does not require to send an external event to the FB
     *        - any other value requires that the ComFB is informed with an external event to further handle the received message
     */
    virtual EComResponse recvData(const void *paData, unsigned int paSize) = 0;

    virtual CBaseCommFB *getCommFB() const{
      return 0;
    }
};

  } //namespace com_infra
} //namespace forte

#endif /* SRC_CORE_COMINFRA_COMCALLBACK_H_ */
