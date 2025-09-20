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
#ifndef _SET_VALUE_H_
#define _SET_VALUE_H_

#include <mtypes.h> /* M1 include files    */
#include "forte/funcbloc.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_sint.h"
#include "forte/datatypes/forte_array.h"

// cppcheck-suppress noConstructor
class FORTE_SET_VALUE : public CFunctionBlock {
    DECLARE_FIRMWARE_FB(FORTE_SET_VALUE)

  private:
    CIEC_INT &MOD_NB() {
      return *static_cast<CIEC_INT *>(getDI(0));
    };

    CIEC_INT &CHAN_NB() {
      return *static_cast<CIEC_INT *>(getDI(1));
    };

    CIEC_SINT &VALUE() {
      return *static_cast<CIEC_SINT *>(getDI(2));
    };

    CIEC_INT &RET_CODE() {
      return *static_cast<CIEC_INT *>(getDO(0));
    };

    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventREQID = 1;

    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventCNFID = 1;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void *driverId;

    bool init();
    SINT32 write();

  public:
    FORTE_SET_VALUE(const forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_SET_VALUE() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
