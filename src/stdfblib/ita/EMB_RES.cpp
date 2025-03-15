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
#include <stringdict.h>
#include "EMB_RES.h"

USE_STRING_ID(EMB_RES);
USE_STRING_ID(START);



DEFINE_FIRMWARE_FB(EMB_RES, STRID(EMB_RES));

const SFBInterfaceSpec EMB_RES::scmFBInterfaceSpec = {
  0, nullptr, nullptr, nullptr, nullptr,
  0, nullptr, nullptr, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

EMB_RES::EMB_RES(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paDevice) :
    CResource(paDevice, scmFBInterfaceSpec, paInstanceNameId),
    fb_START(STRID(START), *this) {
}

EMB_RES::~EMB_RES() = default;

CIEC_ANY *EMB_RES::getDI(const size_t) {
  return nullptr;
}

CDataConnection **EMB_RES::getDIConUnchecked(const TPortId) {
  return nullptr;
}

