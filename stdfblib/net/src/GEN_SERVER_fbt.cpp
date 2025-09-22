/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Rene Smodic, THomas Strasser, Alois Zoitl, Gerhard Ebenhofer,
 *   Ingo Hegny, Martin Melik Merkumians
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "forte/iec61499/net/GEN_SERVER_fbt.h"

using namespace forte::literals;

namespace forte::iec61499::net {
  DEFINE_GENERIC_FIRMWARE_FB(GEN_SERVER, "iec61499::net::GEN_SERVER"_STRID)

  GEN_SERVER::GEN_SERVER(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCommFB(paInstanceNameId, paContainer, com_infra::e_Server) {
  }
} // namespace forte::iec61499::net
