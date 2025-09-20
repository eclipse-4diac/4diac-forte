/*******************************************************************************
 * Copyright (c) 2008, 2025 ACIN, fortiss GmbH, 2018 TU Vienna/ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ingo Hegny, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - fixes connect, prepares for working AnyAdapter
 *******************************************************************************/
#include "forte/adapter.h"
#include "forte/funcbloc.h"
#include "forte/ecet.h"

namespace forte {
  CAdapter::CAdapter(CFBContainer &paContainer,
                     const SFBInterfaceSpec &paInterfaceSpec,
                     const StringId paInstanceNameId,
                     TForteUInt8 paParentAdapterlistID) :
      CFunctionBlock(paContainer, paInterfaceSpec, paInstanceNameId),
      mParentAdapterListEventID(static_cast<TForteUInt16>((paParentAdapterlistID + 1) << 8)),
      mPeer(nullptr) {
  }

  void CAdapter::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (mPeer != nullptr) {
      if (mPeer->getParentAdapterListEventID()) {
        paECET->addEventEntry(TEventEntry(static_cast<CFunctionBlock &>(mPeer->getParent()),
                                          mPeer->getParentAdapterListEventID() + paEIID));
      } else {
        mPeer->sendOutputEvent(paEIID, paECET);
      }
    }
  }
} // namespace forte
