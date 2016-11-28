/*******************************************************************************
  * Copyright (c) 2010-2014 fortiss, TU Wien ACIN and Profactor GmbH.
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl - initial implementation and rework communication infrastructure
  *    Michael Hofmann - rework communication infrastructure
  *    Ingo Hegny - fix doxygen-documentation inconsistencies
  *    Martin Melik-Merkumians - add serial communication layer for ONE input and ONE output
  *******************************************************************************/
#ifndef _COMLAYER_H_
#define _COMLAYER_H_

#include "comtypes.h"

namespace forte {
  namespace com_infra {

    class CCommFB;

    class CComLayer{
      public:
        virtual ~CComLayer();

        /*!\brief Open the connection and create the whole communication stack
         *
         * This function will configure this layer and if necessary create necessary bottom layers.
         *
         * \param pa_acConnectionParams configuration data for this layer
         * \param pa_acRemainingConnectionID layer ids configuration data of layers below this one
         *                                    if 0 or *pa_acRemainingConnectionID equals '0x00' than there is no
         *                                    further layer below this one.
         * \return status of the opening process
         *      - e_InitOk of the opening process was successful
         */
        EComResponse openConnection(char *pa_acConnectionParams, char *pa_acRemainingConnectionID);

        /*!\brief Close this layer.
         *
         * Implementations of this function should perform the actions necessary for closing the layer and than call
         * the closeConnection() of the bottom layer.
         */
        virtual void closeConnection() = 0;

        /*!\brief Take the given data and perform the necessary process for sending
         *
         * If necessary invoke bottom layer sendData functions.
         *
         * @param pa_pvData pointer to the data to be sent
         * @param pa_unSize size of the data to be sent
         * @return status of the sending process:
         *    - e_ProcessDataOk ... if sending process was successful
         */
        virtual EComResponse sendData(void *pa_pvData, unsigned int pa_unSize) = 0;

        /*!\brief Take the given data and perform the necessary process for receiving data
         *
         * This function is called for processing the received data from bottom to top. Therefore
         * if necessary invoke the top layer's receiveData function to hand on the processed data.
         *
         * @param pa_pvData pointer to the data received
         * @param pa_unSize size of the data received
         * @return status of the receiving process depends on if the layer is the bottom most layer:
         *    - Normal Layer:
         *        - e_ProcessDataOk ... if receiving process was successful
         *    - Bottom most layer
         *        - e_Nothing ...  if the processing of the data does not require to send an external event to the FB
         *        - any other value requires that the ComFB is informed with an external event to further handle the received message
         */
        virtual EComResponse recvData(const void *pa_pvData, unsigned int pa_unSize) = 0;

        /*!\brief Finish to process the data received in a context outside the communication interrupt i.e. within the event chain of the ComFB.
         *
         * This function shall be used for finishing the data reception.
         */
        virtual EComResponse processInterrupt();


        /*!\brief get the FB of this layer
         */
        CCommFB *getCommFB() const {
          return m_poFb;
        }

        static char* extractLayerIdAndParams(char *pa_acID, char **pa_pacParameter);

      protected:
        CComLayer(CComLayer* pa_poUpperLayer, CCommFB* pa_poComFB);

        EComConnectionState m_eConnectionState;

        CComLayer *m_poTopLayer;
        CComLayer *m_poBottomLayer;
        CCommFB *m_poFb;
      private:
        /*!\brief Configure the current layer and perform necessary means to setup the connection
         *
         * Depending on the layers functionality different things have to be performed here.
         * This can range from doing nothing to establishing an TCP session.
         *
         * @param pa_acLayerParameter configuration data for this layer
         * @return status of the opening process
         *      - e_InitOk of the opening process was successful
         */
        virtual EComResponse openConnection(char *pa_acLayerParameter) = 0;
    };

  }
}

#endif
