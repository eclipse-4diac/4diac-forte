/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ben Schneider - Initial contribution; vlan and prio configuration to support tsn for pub/sub
 *******************************************************************************/
  
#ifndef TSN_LAYER_H_
#define TSN_LAYER_H_

#include "ipcomlayer.h"

namespace forte {

  namespace com_infra {

    class CTSNLayer : public CIPComLayer{
      public:
        CTSNLayer(CComLayer* paUpperLayer, CBaseCommFB* paBaseCommFB);
        virtual ~CTSNLayer();

      protected:

      private:
        /*!\brief Configure the current layer and perform necessary means to setup the connection
         *
         * The TSN layer configures the publisher's UDP socket to include VLAN id and priority
         *
         * @param pa_acLayerParameter configuration data for this layer
         * @return status of the opening process
         *      - e_InitOk if the opening process was successful
         *      - e_InitInvalidId if the opening process was not successful
         */
        EComResponse openConnection(char *paLayerParameter);


        /*!\brief Set the VLAN priority (VLAN PCP) for the socket
         *
         * This function is used for setting the VLAN priority of the publisher's UDP socket
         *
         * @param paVlanPriority VLAN priority (VLAN PCP) that shall be set in the socket (value between 0 and 7)
         * @return status of setting the VLAN priority
         *      - e_InitOk if setting the VLAN priority was successful
         *      - e_InitInvalidId if setting the VLAN priority was not successful
         */
        EComResponse setVLANPriorityForSocket(unsigned int paVlanPriority);

        /*!\brief Set the VLAN ID for the socket and bind it to that virtual network interface
         *
         * This function is used for setting the VLAN id of the publisher's UDP socket
         * and binding the socket to the given virtual network interface
         *
         * @param paId VLAN id that shall be set in the socket (value between 1 and 4095)
         * @return status of setting the VLAN id
         *      - e_InitOk if setting the VLAN id was successful
         *      - e_InitInvalidId if setting the VLAN id was not successful
         */
        EComResponse setVLANIDForSocket(const char* paId);

        static const unsigned int scmNumParameters = 4;
        static const unsigned int scmMinVLANID = 1;
        static const unsigned int scmMaxVLANID = 4095;
        static const unsigned int scmMaxVLANPrio = 7;
    };
  }
}

#endif /* TSN_LAYER_H_ */
