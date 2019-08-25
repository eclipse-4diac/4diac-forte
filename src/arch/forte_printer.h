/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_ARCH_FORTE_PRINTER_H_
#define SRC_ARCH_FORTE_PRINTER_H_

#include <stddef.h>
#include <stdarg.h>

int forte_snprintf(char* pa_stream, size_t pa_size, const char* pa_format, ...);

int forte_vsnprintf(char* pa_stream, size_t pa_size, const char* pa_format, va_list pa_local_argv);

#endif /* SRC_ARCH_WIN32_FORTE_PRINTER_H_ */
