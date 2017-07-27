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

#include "../forte_printer.h"
#include <stdio.h>

int forte_snprintf(char* pa_stream, size_t pa_size, const char* pa_format, ...){
  int retval;
  va_list local_argv;
  va_start(local_argv, pa_format);
  retval = _snprintf(pa_stream, pa_size, pa_format, local_argv);
  va_end(local_argv);
  return retval;
}

int forte_vsnprintf(char* pa_stream, size_t pa_size, const char* pa_format, va_list pa_local_argv){
  return _vsnprintf(pa_stream, pa_size, pa_format, pa_local_argv);
}
