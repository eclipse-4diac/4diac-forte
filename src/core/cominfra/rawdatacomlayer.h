/*******************************************************************************
 * Copyright (c) 2013-2015 TU Wien ACIN and fortiss.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Martin Melik Merkumians, Monika Wenger
 *     - initial implementation
 *******************************************************************************/
#ifndef RAWDATACOMLAYER_H_
#define RAWDATACOMLAYER_H_

#include "comlayer.h"
#include <commfb.h>

namespace forte {
  namespace com_infra {
    class CRawDataComLayer : public CComLayer{
      public:
        CRawDataComLayer(CComLayer* pa_poUpperLayer, CBaseCommFB * pa_poFB);
        virtual ~CRawDataComLayer();
        virtual EComResponse sendData(void *paData, unsigned int paSize);
        virtual EComResponse recvData(const void *paData, unsigned int paSize);

      private:
        virtual EComResponse openConnection(char *pa_acLayerParameter);
        virtual void closeConnection();
    };
  } /* namespace com_infra */
} /* namespace forte */
#endif /* RAWDATACOMLAYER_H_ */
