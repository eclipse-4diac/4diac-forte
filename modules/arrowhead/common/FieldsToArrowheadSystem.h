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

#ifndef _FIELDSTOARROWHEADSYSTEM_H_
#define _FIELDSTOARROWHEADSYSTEM_H_

#include "forte/basicfb.h"
#include <forte_arrowheadsystem.h>
#include "forte/datatypes/forte_dint.h"
#include "forte/datatypes/forte_wstring.h"

class FORTE_FieldsToArrowheadSystem : public CBasicFB {
    DECLARE_FIRMWARE_FB(FORTE_FieldsToArrowheadSystem)

  private:
    CIEC_WSTRING &systemName() {
      return *static_cast<CIEC_WSTRING *>(getDI(0));
    };

    CIEC_WSTRING &address() {
      return *static_cast<CIEC_WSTRING *>(getDI(1));
    };

    CIEC_DINT &port() {
      return *static_cast<CIEC_DINT *>(getDI(2));
    };

    CIEC_WSTRING &authenticationInfo() {
      return *static_cast<CIEC_WSTRING *>(getDI(3));
    };

    CIEC_ArrowheadSystem &arrowheadSystem() {
      return *static_cast<CIEC_ArrowheadSystem *>(getDO(0));
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
    FORTE_FieldsToArrowheadSystem(forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_FieldsToArrowheadSystem() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
