/*******************************************************************************
 * Copyright (c) 2006 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Gerhard Ebenhofer,
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "EMB_RES.h"
#include "forte/stringid.h"

using namespace forte::literals;

namespace forte::iec61499::system {
  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = {},
      .mEITypeNames = {},
      .mEONames = {},
      .mEOTypeNames = {},
      .mDINames = {},
      .mDONames = {},
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };

  DEFINE_FIRMWARE_FB(EMB_RES, "iec61499::system::EMB_RES"_STRID);

  EMB_RES::EMB_RES(StringId paInstanceNameId, CFBContainer &paDevice) :
      CResource(paDevice, cFBInterfaceSpec, paInstanceNameId),
      fb_START("START"_STRID, *this) {
  }

  EMB_RES::~EMB_RES() = default;

  CIEC_ANY *EMB_RES::getDI(const size_t) {
    return nullptr;
  }

  CDataConnection **EMB_RES::getDIConUnchecked(const TPortId) {
    return nullptr;
  }
} // namespace forte::iec61499::system
