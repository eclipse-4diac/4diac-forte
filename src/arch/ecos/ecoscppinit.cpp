/*******************************************************************************
 * Copyright (c) 2010 - 2024 ACIN, Jose Cabral
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <cyg/kernel/kapi.h>

//Workaround for an ecos problem
extern "C" void
__cxa_pure_virtual(void) {
  //TODO maybe add some exception handling reporting here
}

extern "C" void abort(int ) { while(1) ; }

void *__dso_handle = 0;

extern "C" int _getpid(void) {
  return 1;
}

extern "C" void _kill(int ) { while(1) ; }