/*******************************************************************************
 * Copyright (c) 2024 Jose Cabral, HR Agrartechnik GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral:
 *      - initial implementation and rework communication infrastructure
 *    Franz Höpfinger - string_view for getResponseText
 *******************************************************************************/

#include "forte/mgmcmd.h"

#include <type_traits>

using namespace std::literals;

namespace forte::mgm_cmd {

  /*!\brief Type for the response of MGM command messages
   *
   * TODO fully define all responses as defined in IEC 61499 inc. numbers.
   */
  constexpr std::string_view scmMGMResponseTexts[] = {"RDY"sv,
                                                      "BAD_PARAMS"sv,
                                                      "LOCAL_TERMINATION"sv,
                                                      "SYSTEM_TERMINATION"sv,
                                                      "NOT_READY"sv,
                                                      "UNSUPPORTED_CMD"sv,
                                                      "UNSUPPORTED_TYPE"sv,
                                                      "NO_SUCH_OBJECT"sv,
                                                      "INVALID_OBJECT"sv,
                                                      "INVALID_OPERATION"sv,
                                                      "INVALID_STATE"sv,
                                                      "OVERFLOW"sv,
                                                      "INVALID_DST"sv};

  const std::string_view getResponseText(EMGMResponse paResp) {
    return scmMGMResponseTexts[static_cast<std::underlying_type_t<EMGMResponse>>(paResp)];
  }

} // namespace forte::mgm_cmd
