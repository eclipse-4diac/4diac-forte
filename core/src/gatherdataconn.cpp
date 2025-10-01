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

#include "gatherdataconn.h"

#include <algorithm>

#include "forte/funcbloc.h"

namespace forte::internal {
  EMGMResponse CGatheringDataConnection::connect(CFunctionBlock &, std::span<const StringId>) {
    return EMGMResponse::InvalidOperation; // can not explicitly connect gathering connection
  }

  EMGMResponse CGatheringDataConnection::connectToCFBInterface(CFunctionBlock &, std::span<const StringId>) {
    return EMGMResponse::InvalidOperation; // can not explicitly connect gathering connection
  }

  EMGMResponse CGatheringDataConnection::disconnect(CFunctionBlock &paDstFB,
                                                    std::span<const StringId> paDstPortNameId) {
    if (paDstPortNameId.size() != 1) {
      return EMGMResponse::NoSuchObject;
    }

    const TPortId dstPortId = paDstFB.getFBInterfaceSpec().getDIID(paDstPortNameId.front());
    if (dstPortId == cgInvalidPortId) {
      return EMGMResponse::NoSuchObject;
    }

    if (CConnectionPoint(paDstFB, dstPortId) != getSourceId()) {
      return EMGMResponse::NoSuchObject;
    }

    for (const auto [member, connection, memberName] : mGatheringData) {
      connection->disconnect(paDstFB, memberName);
      if (connection->isDelegating()) {
        delete connection;
      }
    }
    mGatheringData.clear();

    return EMGMResponse::Ready;
  }

  void CGatheringDataConnection::readData(CIEC_ANY &) const {
    for (const auto [member, connection, memberName] : mGatheringData) {
      connection->readData(*member);
    }
  }

  EMGMResponse CGatheringDataConnection::addMemberConnection(const std::span<const StringId> paMemberName,
                                                             CDataConnection &paConnection) {
    if (CIEC_ANY *member = getValue().getVar(paMemberName)) {
      return addMemberConnection(member, &paConnection, paMemberName);
    }
    return EMGMResponse::NoSuchObject;
  }

  EMGMResponse CGatheringDataConnection::removeMemberConnection(const std::span<const StringId> paMemberName) {
    if (const CIEC_ANY *member = getValue().getVar(paMemberName)) {
      return removeMemberConnection(member);
    }
    return EMGMResponse::NoSuchObject;
  }

  CDataConnection *CGatheringDataConnection::getMemberConnection(const std::span<const StringId> paMemberName) {
    if (const CIEC_ANY *member = getValue().getVar(paMemberName)) {
      return getMemberConnection(member);
    }
    return CDataConnection::getMemberConnection(paMemberName);
  }

  EMGMResponse CGatheringDataConnection::addMemberConnection(CIEC_ANY *paMember,
                                                             CDataConnection *paConnection,
                                                             const std::span<const StringId> paMemberName) {
    if (std::ranges::find_if(mGatheringData, [paMember](const SGatheringData &paData) {
          return paData.mMember == paMember;
        }) != mGatheringData.end()) {
      return EMGMResponse::InvalidState;
    }

    mGatheringData.emplace_back(paMember, paConnection, TNameIdentifier{paMemberName.begin(), paMemberName.end()});
    return EMGMResponse::Ready;
  }

  EMGMResponse CGatheringDataConnection::removeMemberConnection(const CIEC_ANY *paMember) {
    if (std::erase_if(mGatheringData,
                      [paMember](const SGatheringData &paData) { return paData.mMember == paMember; })) {
      return EMGMResponse::Ready;
    };
    return EMGMResponse::NoSuchObject;
  }

  CDataConnection *CGatheringDataConnection::getMemberConnection(const CIEC_ANY *paMember) {
    for (const auto [member, connection, memberName] : mGatheringData) {
      if (member == paMember) {
        return connection;
      }
    }
    return nullptr;
  }
} // namespace forte::internal
