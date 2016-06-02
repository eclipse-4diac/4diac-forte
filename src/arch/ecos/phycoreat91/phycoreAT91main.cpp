/*******************************************************************************
 * Copyright (c) 2006, 2007, 2010 ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <fortealloc.h>
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
