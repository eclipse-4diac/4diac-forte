/*******************************************************************************
 * Copyright (c) 2011 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _SVIRECEIVE_1_H_
#define _SVIRECEIVE_1_H_

#include "BE_SVIFB.h"
#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_bool.h"

class FORTE_SVIreceive_1 : public BE_SVIFB {
    DECLARE_FIRMWARE_FB(FORTE_SVIreceive_1)

  private:
    CIEC_BOOL &QI() {
      return *static_cast<CIEC_BOOL *>(getDI(0));
    };

    CIEC_STRING &ID() {
      return *static_cast<CIEC_STRING *>(getDI(1));
    };

    CIEC_BOOL &QO() {
      return *static_cast<CIEC_BOOL *>(getDO(0));
    };

    CIEC_ANY &RD_1() {
      return *static_cast<CIEC_ANY *>(getDO(1));
    };

    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventREQID = 1;

    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventCNFID = 1;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  public:
    FORTE_SVIreceive_1(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
        ~FORTE_SVIreceive_1() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
