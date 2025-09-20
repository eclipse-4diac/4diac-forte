/*******************************************************************************
 * Copyright (c) 2008, 2025 ACIN, fortiss GmbH, 2018 TU Vienna/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ingo Hegny, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - adds typifyAnyAdapter
 *******************************************************************************/
#include "forte/adapterconn.h"
#include "forte/funcbloc.h"
#include "forte/adapter.h"

namespace forte {
  CAdapterConnection::CAdapterConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId, IPlugPin &paPlug) :
      CConnection(paSrcFB, paSrcPortId),
      mPlug(paPlug),
      mSocket(nullptr) {
  }

  CAdapterConnection::~CAdapterConnection() {
    performDisconnect();
  }

  EMGMResponse CAdapterConnection::connect(CFunctionBlock &paDstFB, std::span<const StringId> paDstPortNameId) {
    if (mSocket != nullptr) {
      // we are already connected
      return EMGMResponse::InvalidState;
    }

    if (paDstPortNameId.size() != 1) {
      return EMGMResponse::NoSuchObject;
    }

    ISocketPin *socket = paDstFB.getSocketPin(paDstPortNameId.front());
    if (socket == nullptr) {
      return EMGMResponse::NoSuchObject;
    }

    if (socket->getAdapterCon() != nullptr) {
      // the socket is already connected
      return EMGMResponse::InvalidState;
    }

    if (!mPlug.isCompatible(*socket) && !socket->isCompatible(mPlug)) {
      return EMGMResponse::InvalidObject;
    }

    if (!socket->connect(*this)) {
      return EMGMResponse::Overflow;
    }

    mSocket = socket;
    paDstFB.incConnRefCount();
    getSourceId().getFB().incConnRefCount();
    return EMGMResponse::Ready;
  }

  EMGMResponse CAdapterConnection::connectToCFBInterface(CFunctionBlock &, std::span<const StringId>) {
    return EMGMResponse::InvalidOperation; // currently we are not supporting adapter connections accross interface
                                           // boundaries
  }

  EMGMResponse CAdapterConnection::disconnect(CFunctionBlock &paDstFB,
                                              const std::span<const StringId> paDstPortNameId) {
    if (mSocket == nullptr) {
      // we are not connected
      return EMGMResponse::InvalidState;
    }

    if (paDstPortNameId.size() != 1) {
      return EMGMResponse::NoSuchObject;
    }

    ISocketPin *socket = paDstFB.getSocketPin(paDstPortNameId.front());
    if (socket == nullptr || socket != mSocket) {
      return EMGMResponse::NoSuchObject;
    }

    performDisconnect();

    paDstFB.decConnRefCount();
    getSourceId().getFB().decConnRefCount();
    return EMGMResponse::Ready;
  }

  void CAdapterConnection::performDisconnect() {
    mPlug.setPeer(nullptr);

    if (mSocket != nullptr) {
      mSocket->disconnect();
      mSocket = nullptr;
    }
  }

  void CAdapterConnection::getSourcePortName(TNameIdentifier &paResult) const {
    paResult.push_back(mPlug.getAdapterBlock()->getInstanceNameId());
  }
} // namespace forte
