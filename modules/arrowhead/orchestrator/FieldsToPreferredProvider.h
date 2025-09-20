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

#ifndef _FIELDSTOPREFERREDPROVIDER_H_
#define _FIELDSTOPREFERREDPROVIDER_H_

#include "forte/basicfb.h"
#include <forte_preferredprovider.h>
#include <forte_arrowheadcloud.h>
#include <forte_arrowheadsystem.h>

class FORTE_FieldsToPreferredProvider : public CBasicFB {
    DECLARE_FIRMWARE_FB(FORTE_FieldsToPreferredProvider)

  private:
    CIEC_ArrowheadSystem &providerSystem() {
      return *static_cast<CIEC_ArrowheadSystem *>(getDI(0));
    };

    CIEC_ArrowheadCloud &providerCloud() {
      return *static_cast<CIEC_ArrowheadCloud *>(getDI(1));
    };

    CIEC_PreferredProvider &preferredProvider() {
      return *static_cast<CIEC_PreferredProvider *>(getDO(0));
    };

    static const TEventID scmEventREQID = 0;

    static const TEventID scmEventCNFID = 0;

    void alg_REQ();
    static const TForteInt16 scmStateSTART = 0;
    static const TForteInt16 scmStateREQ = 1;

    void enterStateSTART(CEventChainExecutionThread *const paECET);
    void enterStateREQ(CEventChainExecutionThread *const paECET);

    virtual void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  public:
    FORTE_FieldsToPreferredProvider(forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_FieldsToPreferredProvider() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
