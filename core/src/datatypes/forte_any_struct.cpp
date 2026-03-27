/*******************************************************************************
 * Copyright (c) 2026 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#include "forte/datatypes/forte_any_struct.h"

using namespace forte::literals;
using namespace std::string_literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(ANY_STRUCT, "ANY_STRUCT"_STRID);

  CIEC_ANY_STRUCT::CIEC_ANY_STRUCT() : CIEC_STRUCT() {
  }

  StringId CIEC_ANY_STRUCT::getStructTypeNameID() const {
    return "ANY_STRUCT"_STRID;
  }

  CIEC_ANY *CIEC_ANY_STRUCT::getMember(size_t paMemberIndex) {
    return nullptr;
  }

  const CIEC_ANY *CIEC_ANY_STRUCT::getMember(size_t paMemberIndex) const {
    return nullptr;
  }
} // namespace forte
