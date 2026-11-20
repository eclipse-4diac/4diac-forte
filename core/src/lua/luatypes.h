/*******************************************************************************
 * Copyright (c) 2026 Aimirim STI
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * AI Disclosure: This file was largely AI-generated. The AI-generated
 * portions are made available under CC0-1.0 and not subject to the
 * project's licence. The human contributor has reviewed and verified
 * that the code is correct.
 *
 * SPDX-License-Identifier: EPL-2.0 AND CC0-1.0
 * Assisted-by: Anthropic Claude Opus 5.5 (claude-opus-5-5)
 *
 * Contributors:
 *   Pedro Ricardo C. Souza - initial implementation
 *******************************************************************************/

#pragma once

#include <string>

#include "forte/mgmcmd.h"
#include "forte/stringid.h"

namespace forte::lua {

  class CLuaEngine;

  /*!\brief Creates an FB type from a Lua module and registers it
   *
   * A module with an ECC function defines a basic FB type, a module with an fbnSpec table a composite FB type.
   * The module is read in paEngine, the device's definition state, which is kept between types.
   *
   * @return Ready, also when the type exists with the same definition;
   *         InvalidObject if the module does not load; BadParams for a malformed module or type name;
   *         UnsupportedType for an unknown data or adapter type; InvalidOperation for the name of a compiled type;
   *         InvalidState if the type exists with another definition and has instances;
   *         Overflow if no Lua state could be created
   */
  EMGMResponse createFBType(CLuaEngine &paEngine, StringId paTypeNameId, std::string paSource);

  //! Creates an adapter type from a Lua module and registers it, with the results of createFBType
  EMGMResponse createAdapterType(CLuaEngine &paEngine, StringId paTypeNameId, std::string paSource);

} // namespace forte::lua
