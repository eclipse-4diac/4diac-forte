/*******************************************************************************
 * Copyright (c) 2010-2014 fortiss, TU Wien ACIN and Profactor GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl - initial implementation and rework communication infrastructure
 *    Michael Hofmann - rework communication infrastructure
 *******************************************************************************/
#include "forte/cominfra/comlayer.h"
#include "forte/cominfra/commfb.h"

namespace forte::com_infra {
  CComLayer::CComLayer(CComLayer *paUpperLayer, CBaseCommFB *paComFB) :
      mConnectionState(e_Disconnected),
      mTopLayer(paUpperLayer),
      mBottomLayer(nullptr),
      mFb(paComFB) {
    if (nullptr != mTopLayer) {
      mTopLayer->setBottomLayer(this);
    }
  }

  CComLayer::~CComLayer() {
    if (mBottomLayer != nullptr) {
      mBottomLayer->closeConnection();
      delete mBottomLayer;
    }
  }

  EComResponse CComLayer::processInterrupt() {
    return e_Nothing;
  }

  const CIEC_ANY &CComLayer::getSDx(void *paData, int paSdNum) {
    const CIEC_ANY **sds = static_cast<const CIEC_ANY **>(paData);
    return sds[paSdNum]->unwrap();
  }
} // namespace forte::com_infra
