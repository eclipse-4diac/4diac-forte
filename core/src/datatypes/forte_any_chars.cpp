/*******************************************************************************
 * Copyright (c) 2025 Primetals Technologies Austria GmbH,
 *                    Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - extracted and reimplemnted from forte_any_string
 *   Martin Jobst - fix line feed and newline escape sequences
 *******************************************************************************/
#include <format>
#include "forte/datatypes/forte_any_chars.h"

using namespace forte;
using namespace std::literals::string_literals;

void CIEC_ANY_CHARS::dollarEscapeChar(std::string &paTargetBuf, char paSymbol, const EDataTypeID paTypeID) {
  switch (paSymbol) {
    case '$': paTargetBuf += "$$"s; break;
    case '\'': paTargetBuf += (paTypeID == e_STRING) ? "$'"s : "'"s; break;
    case '\n': paTargetBuf += "$l"s; break;
    case '\f': paTargetBuf += "$p"s; break;
    case '\r': paTargetBuf += "$r"s; break;
    case '\t': paTargetBuf += "$t"s; break;
    case '\"': paTargetBuf += (paTypeID == e_WSTRING) ? "$\"" : "\""s; break;
    default:
      if (!isprint(static_cast<unsigned char>(paSymbol)) && paTypeID == e_STRING) {
        std::format_to(std::back_inserter(paTargetBuf), "${:02X}", static_cast<unsigned char>(paSymbol));
      } else {
        paTargetBuf += paSymbol;
      }
      break;
  }
}
