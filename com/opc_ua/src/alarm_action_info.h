/*******************************************************************************
 * Copyright (c) 2025 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Markus Meingast - initial implementation
 *******************************************************************************/

#pragma once

#include "opcua_action_info.h"
#include "opcua_layer.h"
#include "opcua_objectstruct_helper.h"

namespace forte::com_infra::opc_ua {
  class CAlarmActionInfo : public CActionInfo {
    public:
      explicit CAlarmActionInfo(COPC_UA_Layer &paLayer,
                                UA_ActionType paAction,
                                const std::string &paEndpoint,
                                const size_t firstDataIndex) :
          CActionInfo(paLayer, paAction, paEndpoint),
          mFirstDataIndex(firstDataIndex) {
      }

      const CIEC_ANY *const *getDataToSend() override {
        if (mFirstDataIndex >= getLayer().getCommFB()->getNumSD()) {
          return nullptr;
        }
        CIEC_ANY **dataToSend = getLayer().getCommFB()->getSDs();
        return std::next(dataToSend, mFirstDataIndex);
      }

    private:
      const size_t mFirstDataIndex;
  };
} // namespace forte::com_infra::opc_ua
