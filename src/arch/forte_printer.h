/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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
