/*******************************************************************************
 * Copyright (c) 2009 - 2013 ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Monika Wenger, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte/datatypes/forte_ulint.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(ULINT, "ULINT"_STRID)

  template CIEC_ULINT &CIEC_ULINT::operator= <>(const CIEC_USINT &paValue);

  template CIEC_ULINT &CIEC_ULINT::operator= <>(const CIEC_UINT &paValue);

  template CIEC_ULINT &CIEC_ULINT::operator= <>(const CIEC_UDINT &paValue);

  const StringId CDataTypeTrait<CIEC_ULINT>::scmDataTypeName = "ULINT"_STRID;
} // namespace forte
