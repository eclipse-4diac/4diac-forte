/*******************************************************************************
 * Copyright (c) 2006, 2007, 2010 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <fortenew.h>
#include "../../../stdfblib/ita/RMT_DEV.h"
#include <cyg/kernel/kapi.h>
#include <network.h>
#include <cyg/hal/hal_io.h>             // IO macros

externC void
cyg_user_start( void );

void cyg_user_start(void){
  RMT_DEV *dev = new RMT_DEV;  //otherwise we would loose the var if it is only on the stack
  dev->startDevice();
}

//get the stuff that fixes some ecos cpp problems which have to be near the main
#include "../ecoscppinit.cpp"
