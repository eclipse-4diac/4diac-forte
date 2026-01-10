/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_ldate.h"
#include "forte/datatypes/forte_ldate_and_time.h"

namespace forte {
  CIEC_LDATE func_LDT_TO_LDATE(const CIEC_LDATE_AND_TIME &paVal);
} // namespace forte
