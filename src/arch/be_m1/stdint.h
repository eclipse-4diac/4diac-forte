/*******************************************************************************
 * Copyright (c) 2012 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Stefano Campanelli - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _STDINT_H_
#define _STDINT_H_

#include <types/vxTypes.h>

#define UINT8_MIN _ARCH_UCHAR_MIN
#define UINT8_MAX _ARCH_UCHAR_MAX
#define INT8_MIN _ARCH_CHAR_MIN
#define INT8_MAX _ARCH_CHAR_MAX

#define INT16_MIN _ARCH_SHRT_MIN
#define INT16_MAX _ARCH_SHRT_MAX
#define UINT16_MIN _ARCH_USHRT_MIN
#define UINT16_MAX _ARCH_USHRT_MAX

#define INT32_MIN _ARCH_INT_MIN
#define INT32_MAX _ARCH_INT_MAX
#define UINT32_MIN _ARCH_UINT_MIN
#define UINT32_MAX _ARCH_UINT_MAX

#define UINT64_MIN 0ULL // 0
#define UINT64_MAX 18446744073709551615ULL // 2^64 -1
#define INT64_MIN (-9223372036854775807LL - 1) // -2^63
#define INT64_MAX 9223372036854775807LL // 2^63-1

#endif
