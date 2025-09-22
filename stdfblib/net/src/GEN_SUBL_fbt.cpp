/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Rene Smodic, Thomas Strasser, Alois Zoitl, Gerhard Ebenhofer,
 *   Ingo Hegny, Martin Melik Merkumians
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "forte/iec61499/net/GEN_SUBL_fbt.h"
#include "forte/cominfra/comlayersmanager.h"

using namespace forte::literals;

namespace forte::iec61499::net {
  DEFINE_GENERIC_FIRMWARE_FB(GEN_SUBL, "iec61499::net::GEN_SUBL"_STRID)

  const char *const GEN_SUBL::scmLocalIDPrefix = "loc[";
  const char *const GEN_SUBL::scmLocalIDSuffix = "]";

  GEN_SUBL::GEN_SUBL(const StringId paInstanceNameId, CFBContainer &paContainer) :
      GEN_SUBSCRIBE(paInstanceNameId, paContainer) {
  }

  bool GEN_SUBL::configureFB(const char *paConfigString) {
    bool bRetVal = GEN_SUBSCRIBE::configureFB(paConfigString);
    // subls normally don't show the QI in the tool
    QI() = true_BOOL;
    return bRetVal;
  }

  char *GEN_SUBL::getDefaultIDString(const char *paID) {
    return buildIDString(scmLocalIDPrefix, paID, scmLocalIDSuffix);
  }
} // namespace forte::iec61499::net
