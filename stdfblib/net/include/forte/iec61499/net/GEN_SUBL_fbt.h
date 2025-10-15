/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Rene Smodic, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny,
 *   Martin Melik Merkumians
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#pragma once

#include "forte/iec61499/net/GEN_SUBSCRIBE_fbt.h"

/*! \brief Implementation of the SUBL SIFB.
 *
 */

namespace forte::iec61499::net {
  class GEN_SUBL : public GEN_SUBSCRIBE {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_SUBL)
    public:
      GEN_SUBL(const StringId paInstanceNameId, CFBContainer &paContainer);

      bool configureFB(const char *paConfigString) override;

    protected:
      static const char *const scmLocalIDPrefix;
      static const char *const scmLocalIDSuffix;

    private:
      char *getDefaultIDString(const char *paID) override;
  };

} // namespace forte::iec61499::net
