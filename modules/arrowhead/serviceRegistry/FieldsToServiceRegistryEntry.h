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

#ifndef _FIELDSTOSERVICEREGISTRYENTRY_H_
#define _FIELDSTOSERVICEREGISTRYENTRY_H_

#include "forte/basicfb.h"
#include <forte_arrowheadservice.h>
#include "forte/datatypes/forte_bool.h"
#include <forte_arrowheadsystem.h>
#include "forte/datatypes/forte_dint.h"
#include <forte_serviceregistryentry.h>
#include "forte/datatypes/forte_wstring.h"

class FORTE_FieldsToServiceRegistryEntry : public CBasicFB {
    DECLARE_FIRMWARE_FB(FORTE_FieldsToServiceRegistryEntry)

  private:
    CIEC_ArrowheadService &providedService() {
      return *static_cast<CIEC_ArrowheadService *>(getDI(0));
    };

    CIEC_ArrowheadSystem &provider() {
      return *static_cast<CIEC_ArrowheadSystem *>(getDI(1));
    };

    CIEC_WSTRING &serviceURI() {
      return *static_cast<CIEC_WSTRING *>(getDI(2));
    };

    CIEC_DINT &version() {
      return *static_cast<CIEC_DINT *>(getDI(3));
    };

    CIEC_BOOL &udp() {
      return *static_cast<CIEC_BOOL *>(getDI(4));
    };

    CIEC_DINT &ttl() {
      return *static_cast<CIEC_DINT *>(getDI(5));
    };

    CIEC_WSTRING &metadata() {
      return *static_cast<CIEC_WSTRING *>(getDI(6));
    };

    CIEC_ServiceRegistryEntry &serviceRegistryEntry() {
      return *static_cast<CIEC_ServiceRegistryEntry *>(getDO(0));
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
    FORTE_FieldsToServiceRegistryEntry(forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_FieldsToServiceRegistryEntry() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
