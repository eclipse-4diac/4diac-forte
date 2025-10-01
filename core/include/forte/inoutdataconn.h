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

#include "forte/datatypes/forte_any.h"
#include "forte/dataconn.h"

namespace forte {
  class CInOutDataConnection : public CDataConnection {
    public:
      CInOutDataConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId, CIEC_ANY *paValue) :
          CDataConnection(paSrcFB, paSrcPortId),
          mValue(paValue) {
      }

      EMGMResponse connect(CFunctionBlock &paDstFB, std::span<const StringId> paDstPortNameId) override;

      EMGMResponse disconnect(CFunctionBlock &paDstFB, std::span<const StringId> paDstPortNameId) override;

      void getSourcePortName(TNameIdentifier &paResult) const override;

      void writeData(const CIEC_ANY &paValue) override {
        if (mValue) {
          mValue->setValue(paValue.unwrap());
        }
      }

      void readData(CIEC_ANY &paValue) const override {
        if (mValue) {
          paValue.setValue(mValue->unwrap());
        }
      }

      void setValue(CIEC_ANY *paValue);

      CIEC_ANY &getValue() override {
        return *mValue;
      }

      bool isInOut() const override {
        return true;
      }

    protected:
      EMGMResponse establishDataConnection(CFunctionBlock &paDstFB,
                                           const TPortId paDstPortId,
                                           const CIEC_ANY &paDstDataPoint) override;

    private:
      CIEC_ANY *mValue;
      std::vector<CConnectionPoint> mInOutDestinationIds;
  };

  template<typename T>
  class COutInOutDataConnection final : public CInOutDataConnection {
    public:
      COutInOutDataConnection(CFunctionBlock &paSrcFB, TPortId paSrcPortId, const T &paValue) :
          CInOutDataConnection(paSrcFB, paSrcPortId, &mValue),
          mValue(paValue) {
      }

    private:
      T mValue;
  };
} // namespace forte
