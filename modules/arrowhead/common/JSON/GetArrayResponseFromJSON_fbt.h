/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _GETARRAYRESPONSEFROMJSON_H_
#define _GETARRAYRESPONSEFROMJSON_H_

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_any.h"

class FORTE_GetArrayResponseFromJSON : public CFunctionBlock {
    DECLARE_FIRMWARE_FB(FORTE_GetArrayResponseFromJSON)

  private:
    CIEC_STRING &response() {
      return *static_cast<CIEC_STRING *>(getDI(0));
    };

    CIEC_ANY &output() {
      return *static_cast<CIEC_ANY *>(getDO(0));
    };

    CIEC_ARRAY<> &output_Array() {
      return *static_cast<CIEC_ARRAY<> *>(getDO(0));
    }

    static const TEventID scmEventREQID = 0;

    static const TEventID scmEventCNFID = 0;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    bool isResponseEmpty(char *paText);

  public:
    FORTE_GetArrayResponseFromJSON(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :

        ~FORTE_GetArrayResponseFromJSON() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
