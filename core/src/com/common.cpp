/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
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

#include "forte/com/common.h"

namespace forte::com {
  namespace {
    std::size_t findClosingBracket(const std::string_view paString, const std::size_t paStart) {
      std::size_t bracketCount = 1;
      for (std::size_t i = paStart; i < paString.length(); i++) {
        if (paString[i] == '[') {
          bracketCount++;
        } else if (paString[i] == ']') {
          bracketCount--;
          if (bracketCount == 0) {
            return i;
          }
        }
      }
      return std::string_view::npos;
    }

    std::size_t
    parseChannelDescriptor(const std::string_view paID, const std::size_t paStart, ComChannelDescriptor &paResult) {
      const std::size_t openingBracket = paID.find('[', paStart);
      if (openingBracket == std::string_view::npos) {
        return std::string_view::npos;
      }
      const std::size_t closingBracket = findClosingBracket(paID, openingBracket + 1);
      if (closingBracket == std::string_view::npos) {
        return std::string_view::npos;
      }
      paResult = {
          .mChannel = StringId::lookup(paID.substr(paStart, openingBracket - paStart)),
          .mConfigString = paID.substr(openingBracket + 1, closingBracket - openingBracket - 1),
      };
      return closingBracket + 1;
    }
  } // namespace

  std::string_view getComResultString(const ComResult paResult) {
    switch (paResult) {
      case ComResult::Ok: return "OK";
      case ComResult::InvalidId: return "INVALID_ID";
      case ComResult::Terminated: return "TERMINATED";
      case ComResult::InvalidObject: return "INVALID_OBJECT";
      case ComResult::DataTypeError: return "DATA_TYPE_ERROR";
      case ComResult::Inhibited: return "INHIBITED";
      case ComResult::NoSocket: return "NO_SOCKET";
      case ComResult::SendFailed: return "SEND_FAILED";
      case ComResult::RecvFailed: return "RECV_FAILED";
      case ComResult::Async: return "ASYNC";
      case ComResult::More: return "MORE";
      case ComResult::Overflow: return "OVERFLOW";
      default: return "UNKNOWN";
    }
  }

  std::vector<ComChannelDescriptor> parseComId(const std::string_view paId) {
    std::vector<ComChannelDescriptor> result;
    std::size_t start = 0;
    do {
      ComChannelDescriptor descriptor;
      const std::size_t end = parseChannelDescriptor(paId, start, descriptor);
      if (end == std::string_view::npos) {
        return {};
      }
      result.emplace_back(descriptor);
      start = end;
    } while (start < paId.length() && paId[start++] == '.');
    if (start < paId.length()) {
      return {};
    }
    return result;
  }
} // namespace forte::com
