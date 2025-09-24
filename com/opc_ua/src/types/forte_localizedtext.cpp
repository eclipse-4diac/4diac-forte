/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "forte_localizedtext.h"

#include "forte/com/opc_ua/opcua_types.h"

using namespace forte::literals;
using namespace std::string_literals;

namespace forte::com_infra::opc_ua {
  namespace {
    [[maybe_unused]] const OPC_UA_External_Types::Entry entry("LocalizedText"_STRID, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
  }

  DEFINE_FIRMWARE_DATATYPE(LocalizedText, "LocalizedText"_STRID);

  CIEC_LocalizedText::CIEC_LocalizedText() : CIEC_STRUCT(), var_locale(""s), var_text(""s) {
  }

  StringId CIEC_LocalizedText::getStructTypeNameID() const {
    return "LocalizedText"_STRID;
  }

  CIEC_ANY *CIEC_LocalizedText::getMember(size_t paMemberIndex) {
    switch (paMemberIndex) {
      case 0: return &var_locale;
      case 1: return &var_text;
    }
    return nullptr;
  }

  const CIEC_ANY *CIEC_LocalizedText::getMember(size_t paMemberIndex) const {
    switch (paMemberIndex) {
      case 0: return &var_locale;
      case 1: return &var_text;
    }
    return nullptr;
  }

  const StringId CIEC_LocalizedText::scmElementNames[] = {"locale"_STRID, "text"_STRID};
} // namespace forte::com_infra::opc_ua
