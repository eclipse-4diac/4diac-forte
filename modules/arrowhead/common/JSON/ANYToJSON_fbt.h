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

#ifndef _ANYTOJSON_H_
#define _ANYTOJSON_H_

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_any.h"

class FORTE_ANYToJSON : public CFunctionBlock {
    DECLARE_FIRMWARE_FB(FORTE_ANYToJSON)

  private:
    CIEC_ANY &input() {
      return *static_cast<CIEC_ANY *>(getDI(0));
    };

    CIEC_STRING &output() {
      return *static_cast<CIEC_STRING *>(getDO(0));
    };

    static const TEventID scmEventREQID = 0;

    static const TEventID scmEventCNFID = 0;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  public:
    FORTE_ANYToJSON(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :

        ~FORTE_ANYToJSON() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
