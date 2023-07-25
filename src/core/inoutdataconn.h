/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation
 *******************************************************************************/
#pragma once
#ifndef _INOUTDATACONN_H_
#define _INOUTDATACONN_H_

#include "./datatypes/forte_any.h"
#include "dataconn.h"
#include "connectiondestinationtype.h"

class CInOutDataConnection : public CDataConnection {
  public:
    CInOutDataConnection(CFunctionBlock *paSrcFB, TPortId paSrcPortId, CIEC_ANY *paValue);

    EMGMResponse connect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId) override;

    EMGMResponse disconnect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId) override;

    void setValue(CIEC_ANY *paValue) override;

    bool isConnected() const override;

  protected:
    EMGMResponse establishDataConnection(CFunctionBlock *paDstFB, TPortId paDstPortId, CIEC_ANY *paDstDataPoint);
};

#endif /* _INOUTDATACONN_H_ */