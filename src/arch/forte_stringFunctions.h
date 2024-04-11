/*******************************************************************************
 * Copyright (c) 2024 Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Ketut Kumajaya - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef SRC_ARCH_FORTE_STRINGFUNCTIONS_H_
#define SRC_ARCH_FORTE_STRINGFUNCTIONS_H_

#include <string>
#include <codecvt>

inline std::wstring forte_stringToWstring(const std::string &paVal) {
  auto &converter = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(std::locale());
  std::mbstate_t mb{};
  std::wstring dst(paVal.size(), '\0');
  const char *from_next;
  wchar_t *to_next;
  converter.in(mb, paVal.data(), paVal.data() + paVal.length(), from_next,
                    dst.data(), dst.data() + dst.length(), to_next);
  // error checking skipped for brevity
  dst.resize(to_next - dst.data());
  return dst;
}

inline std::string forte_wstringToString(const std::wstring &paVal) {
  auto &converter = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(std::locale());
  std::mbstate_t mb{};
  std::string dst(paVal.size() * converter.max_length(), '\0');
  const wchar_t *from_next;
  char *to_next;
  converter.out(mb, paVal.data(), paVal.data() + paVal.length(), from_next,
                    dst.data(), dst.data() + dst.length(), to_next);
  // error checking skipped for brevity
  dst.resize(to_next - dst.data());
  return dst;
}

#endif /* SRC_ARCH_FORTE_STRINGFUNCTIONS_H_ */
