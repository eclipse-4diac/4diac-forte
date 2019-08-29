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

#include "../forte_printer.h"
#include <stdio.h>

int forte_snprintf(char* pa_stream, size_t pa_size, const char* pa_format, ...){
  int retVal;
  va_list local_argv;
  va_start(local_argv, pa_format);
  retVal = _vsnprintf(pa_stream, pa_size, pa_format, local_argv);
  va_end(local_argv);
  return retVal;
}

int forte_vsnprintf(char* pa_stream, size_t pa_size, const char* pa_format, va_list pa_local_argv){
  return _vsnprintf(pa_stream, pa_size, pa_format, pa_local_argv);
}
