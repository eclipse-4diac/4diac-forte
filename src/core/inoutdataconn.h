/*******************************************************************************
 * Copyright (c) 2023, 2025 Primetals Technologies Austria GmbH
 *                          Martin Erich Jobst
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
 *    Martin Erich Jobst
 *      - templated connection rework
 *******************************************************************************/
#pragma once

#include "./datatypes/forte_any.h"
#include "dataconn.h"
#include "connectiondestinationtype.h"

class CInOutDataConnection : public CDataConnection {
  public:
    CInOutDataConnection(CFunctionBlock *paSrcFB, TPortId paSrcPortId, CIEC_ANY *paValue)
      : CDataConnection(paSrcFB, paSrcPortId, paValue) {
    }

    EMGMResponse connect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId) override;

    EMGMResponse disconnect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId) override;

    void setValue(CIEC_ANY *paValue) override;

    bool isConnected() const override {
      return true;
    };

  protected:
    EMGMResponse
    establishDataConnection(CFunctionBlock *paDstFB, TPortId paDstPortId, const CIEC_ANY &paDstDataPoint) override;
};

template<typename T>
class COutInOutDataConnection final : public CInOutDataConnection {
  public:
    COutInOutDataConnection(CFunctionBlock *paSrcFB, TPortId paSrcPortId, const T &paValue)
      : CInOutDataConnection(paSrcFB, paSrcPortId, &mValue), mValue(paValue) {
    }

  private:
    T mValue;
};
