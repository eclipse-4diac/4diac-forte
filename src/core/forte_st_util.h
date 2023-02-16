/*******************************************************************************
 * Copyright (c) 2022 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Jobst
 *      - initial implementation
 *******************************************************************************/

#ifndef FORTE_FORTE_ST_UTIL_H
#define FORTE_FORTE_ST_UTIL_H

template<typename T>
T& ST_IGNORE_OUT_PARAM(T&& value) {
  return value;
}

/** @brief Extends lifetime of a temporary rvalue for function calls
 *
 * This extended the percieved lifetime of a temporary object for the compiler.
 * Its needed for Partials and negation of output parameters.
 *
 * Its the same as ST_IGNORE_OUT_PARAM, but a different name, as it has a different
 * reason in the code. Currently, aliasing of template functions does not work, so the
 * code was duplicated.
 */
template<typename T>
T &ST_EXTEND_LIFETIME(T &&value) {
  return value;
}
#endif //FORTE_FORTE_ST_UTIL_H
