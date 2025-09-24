/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl,
 *    Ingo Hegny, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte/datatypes/forte_int.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(INT, "INT"_STRID)

  template CIEC_INT &CIEC_INT::operator= <>(const CIEC_SINT &paValue);

  template CIEC_INT &CIEC_INT::operator= <>(const CIEC_USINT &paValue);

  const StringId CDataTypeTrait<CIEC_INT>::scmDataTypeName = "INT"_STRID;
} // namespace forte
