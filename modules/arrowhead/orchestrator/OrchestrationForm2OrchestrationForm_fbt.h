/*******************************************************************************
 * Copyright (c) 2019, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _ORCHESTRATIONFORM2ORCHESTRATIONFORM_H_
#define _ORCHESTRATIONFORM2ORCHESTRATIONFORM_H_

#include "forte/funcbloc.h"
#include <forte_orchestrationform.h>

class FORTE_OrchestrationForm2OrchestrationForm : public CFunctionBlock {
    DECLARE_FIRMWARE_FB(FORTE_OrchestrationForm2OrchestrationForm)

  private:
    CIEC_OrchestrationForm &st_IN() {
      return *static_cast<CIEC_OrchestrationForm *>(getDI(0));
    };

    CIEC_OrchestrationForm &st_OUT() {
      return *static_cast<CIEC_OrchestrationForm *>(getDO(0));
    };

    static const TEventID scmEventREQID = 0;

    static const TEventID scmEventCNFID = 0;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  public:
    FORTE_OrchestrationForm2OrchestrationForm(const forte::StringId paInstanceNameId,
                                              forte::CFBContainer &paContainer) :

        ~FORTE_OrchestrationForm2OrchestrationForm() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
