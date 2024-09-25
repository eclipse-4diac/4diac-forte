/*******************************************************************************
 * Copyright (c) 2006, 2024 ACIN, Profactor GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Rene Smodic, Gerhard Ebenhofer, Martin Melik Merkumians
 *    - initial API and implementation and/or initial documentation
 *   Martin Erich Jobst
 *    - add generic event accessors
 *******************************************************************************/
#ifndef _GEN_PUBLISH_H_
#define _GEN_PUBLISH_H_

#include "../../core/cominfra/commfb.h"

/*! \brief Implementation of the PUBLISH SIFB.
 */

class GEN_PUBLISH : public forte::com_infra::CCommFB {
  DECLARE_GENERIC_FIRMWARE_FB(GEN_PUBLISH)

  public:
    GEN_PUBLISH(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    ~GEN_PUBLISH() override = default;

    template<typename ...Args>
    void evt_INIT(Args&& ...paArgs) {
      writeInputArguments(std::forward<Args>(paArgs)...);
      receiveInputEvent(scmEventINITID, nullptr);
      readOutputArguments(std::forward<Args>(paArgs)...);
    }

    template<typename ...Args>
    void evt_REQ(Args&& ...paArgs) {
      writeInputArguments(std::forward<Args>(paArgs)...);
      receiveInputEvent(scmSendNotificationEventID, nullptr);
      readOutputArguments(std::forward<Args>(paArgs)...);
    }

    template<typename ...Args>
    void operator()(Args&& ...paArgs) {
      evt_INIT(std::forward<Args>(paArgs)...);
    }
};

#endif //_GEN_PUBLISH_H_


