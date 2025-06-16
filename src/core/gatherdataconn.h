/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Jobst - initial implementation
 *******************************************************************************/

#pragma once

#include "delegdataconn.h"
#include "forte_struct.h"

namespace forte::core::internal {

  class CGatheringDataConnection final : public CDelegatingDataConnection<CIEC_STRUCT> {
    public:
      CGatheringDataConnection(CFunctionBlock &paSrcFB,
                                  const TPortId paSrcPortId,
                                  CIEC_STRUCT &paValue) :
        CDelegatingDataConnection(paSrcFB, paSrcPortId, paValue) {
      }

      EMGMResponse connect(CFunctionBlock &paDstFB, std::span<const CStringDictionary::TStringId> paDstPortNameId) override;

      EMGMResponse connectToCFBInterface(CFunctionBlock &paDstFB, std::span<const CStringDictionary::TStringId> paDstPortNameId) override;

      EMGMResponse disconnect(CFunctionBlock &paDstFB, std::span<const CStringDictionary::TStringId> paDstPortNameId) override;

      void readData(CIEC_ANY &paValue) const override;

      EMGMResponse addMemberConnection(std::span<const CStringDictionary::TStringId> paMemberName,
                                 CDataConnection &paConnection);

      EMGMResponse removeMemberConnection(std::span<const CStringDictionary::TStringId> paMemberName);

      CDataConnection *getMemberConnection(std::span<const CStringDictionary::TStringId> paMemberName) override;

      [[nodiscard]] bool isGathering() const override {
        return true;
      }

      [[nodiscard]] bool empty() const {
        return mGatheringData.empty();
      }

    private:
      EMGMResponse addMemberConnection(CIEC_ANY *paMember, CDataConnection *paConnection, std::span<const CStringDictionary::TStringId> paMemberName);

      EMGMResponse removeMemberConnection(const CIEC_ANY *paMember);

      CDataConnection *getMemberConnection(const CIEC_ANY *paMember);

      struct SGatheringData {
        CIEC_ANY *mMember{};
        CDataConnection *mConnection{};
        TNameIdentifier mMemberName;
      };

      std::vector<SGatheringData> mGatheringData;
  };
} // namespace forte::core::internal
