/*******************************************************************************
 * Copyright (c) 2007 - 2011 TU Wien ACIN and Profactor GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl - initial implementation and rework communication infrastructure
 *    Michael Hofmann - communication infrastructure based on layer
 *******************************************************************************/
#ifndef _COMLAYERSMANAGER_H_
#define _COMLAYERSMANAGER_H_

namespace forte {
  namespace com_infra {
    
  class CBaseCommFB;
    class CComLayer;

    /*!\brief Communication Layers Manager for communication with Server/Client and Publish/Subscriber
     *
     * The communication interface is separated into two part, the handler and the protocol
     * The Hander is the interface for all function blocks which wants to communicate over
     * a network. Every function block which wants to communicate has to be registered at the
     * handler. A registration does not open any connection. Therefore the registration can be
     * handled in the function block creation. The connection with a network is established with
     * a INIT+ event.
     * The protocol is a layered protocol-stack which has two interfaces. One for the communication
     * with the upper layer and one with the underneath layer. The top layer is the network handler
     * and the bottom layer is either the operating system or a protocol layer which is able to communicate
     * directly with the network.
     * The memory-allocation is always done by the underneath layer. Thus the upper layer can simple send
     * data to a underlying layer. The underneath layer can notify the upper layer by calling the
     * newDataAvailable-function. Then the upper layer has to get the data by calling the getDataPackage-
     * function. The returned pointer has to be deleted by the upper layer. A smart-pointer like design
     * is desirable.
     */

    class CComLayersManager{
      public:
        static CComLayer* createCommunicationLayer(char *pa_acLayerIdentifier, CComLayer* pa_poUpperLayer, CBaseCommFB * pa_poComFB);
      private:
        CComLayersManager(void);
        CComLayersManager(const CComLayersManager& pa_roVal);
    };
  }
}
#endif
