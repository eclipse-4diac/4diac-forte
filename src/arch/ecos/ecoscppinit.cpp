/*******************************************************************************
 * Copyright (c) 2010 - 2011 ACIN
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

//current tests have shown that this works only if it is in the main.cpp file
//I don't know why. The best is to copy or include it into your specific main file

//Workaround for an ecos problem
extern "C" void
__cxa_pure_virtual(void) {
  //TODO maybe add some exception handling reporting here
}
