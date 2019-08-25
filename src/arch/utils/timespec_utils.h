/*******************************************************************************
 * Copyright (c) 2018 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_ARCH_UTILS_TIMESPEC_UTILS_H_
#define SRC_ARCH_UTILS_TIMESPEC_UTILS_H_

void timespecSub(const struct timespec *const minuend, const struct timespec *const subtrahend, struct timespec *const result);

void timespecAdd(const struct timespec *const start, const struct timespec *const end, struct timespec *const result);

bool timespecLessThan(const struct timespec *const start, const struct timespec *const end);


#endif /* SRC_ARCH_UTILS_TIMESPEC_UTILS_H_ */
