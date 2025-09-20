/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure,
 *******************************************************************************/
#include "forte/datatypes/forte_state.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(STATE, "STATE"_STRID)

  const CIEC_STATE::TValueType CIEC_STATE::scmMaxVal = std::numeric_limits<TValueType>::max();
} // namespace forte
