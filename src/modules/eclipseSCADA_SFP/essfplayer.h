/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef ESSFPLAYER_H_
#define ESSFPLAYER_H_

#include <comlayer.h>
#include <forte_any.h>

class CES_SFP_Layer : public forte::com_infra::CComLayer{
  public:
    CES_SFP_Layer(forte::com_infra::CComLayer* paUpperLayer, forte::com_infra::CCommFB* paComFB);
    virtual ~CES_SFP_Layer();

    virtual forte::com_infra::EComResponse sendData(void *paData, unsigned int paSize);
    virtual forte::com_infra::EComResponse recvData(const void *paData, unsigned int paSize);
    virtual forte::com_infra::EComResponse processInterrupt();

  private:
    virtual forte::com_infra::EComResponse openConnection(char *paLayerParameter);
    virtual void closeConnection();

    forte::com_infra::EComResponse createItems(CIEC_ANY *paDataArray, int paNumData, char *paLayerParameter);


    struct sfp_item **mSFPItem;  //!< pointer to the list of items managed by the EclipseSCADA SFP stack
    forte::com_infra::EComResponse mInterruptResp;

    static const char scmParameterSeperator = ';';
};

#endif /* ESSFPLAYER_H_ */
