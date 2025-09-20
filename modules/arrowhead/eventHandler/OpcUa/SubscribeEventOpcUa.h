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

#ifndef _SUBSCRIBEEVENTOPCUA_H_
#define _SUBSCRIBEEVENTOPCUA_H_

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "SubscribeEventAdp_adp.h"

class FORTE_SubscribeEventOpcUa : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_SubscribeEventOpcUa)

  private:
    FORTE_SubscribeEventAdp &SubscribeEventAdp() {
      return (*static_cast<FORTE_SubscribeEventAdp *>(getAdapterUnchecked(0)));
    };
    static const int scmSubscribeEventAdpAdpNum = 0;

  public:
    FORTE_SubscribeEventOpcUa(const forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_SubscribeEventOpcUa() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
