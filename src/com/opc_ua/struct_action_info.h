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
 *    Alois Zoitl - initial implementation
 *******************************************************************************/

#pragma once

#include "opcua_action_info.h"
#include "opcua_layer.h"

class CStructMemberActionInfo : public CActionInfo {
  public:

    explicit CStructMemberActionInfo(COPC_UA_Layer &paLayer, UA_ActionType paAction, const std::string &paEndpoint):
        CActionInfo(paLayer, paAction, paEndpoint){
    }

    const CIEC_ANY *const *getDataToSend() override {
      mMemberArr[0] = getLayer().getObjectStructMember(*this, true);
      return mMemberArr;
    }

  private:
    CIEC_ANY const* mMemberArr[1];

};
