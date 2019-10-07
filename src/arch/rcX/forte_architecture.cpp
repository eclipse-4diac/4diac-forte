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
 *  Alois Zoitl - adapted architecture class for generic use
 *******************************************************************************/

#include "../forte_architecture.h"
#include "forteinit.h"

#include "rcXSocketInterface.h"

//this keeps away a lot of rtti and exception handling stuff
extern "C" void __cxa_pure_virtual(void){
  //TODO maybe add some reporting here
  //Although we should never get here
  //if we are here something very very bad has happened e.g., stack overflow or other memory corruption

}

void *__dso_handle = 0;

extern "C" int _getpid(void) {
  return 1;
}

extern "C" void _kill(int ) { while(1) ; }

extern "C" int _exit() { return 0; }

bool CForteArchitecture::mInitialized = false;

typedef void (*func_ptr)(void);

extern func_ptr _init_array_start_forte[0], _init_array_end_forte[0];
extern func_ptr _fini_array_start_forte[0], _fini_array_end_forte[0];

void initialize_constructors(void)
{
  func_ptr* func;
  for ( func = _init_array_start_forte; func != _init_array_end_forte; func++ )
    (*func)();
}

void initialize_destructors(void)
{
  func_ptr* func;
  for ( func = _fini_array_start_forte; func != _fini_array_end_forte; func++ )
    (*func)();
}

/*func_ptr _init_array_start_forte[0] __attribute__ ((used, section(".init_array"), aligned(sizeof(func_ptr)))) = { };
func_ptr _fini_array_star_fortet[0] __attribute__ ((used, section(".fini_array"), aligned(sizeof(func_ptr)))) = { };*/

bool CForteArchitecture::initialize(){
  if (!mInitialized){
    initialize_constructors();
    initForte();
    CrcXSocketInterface::getInstance();
    mInitialized = true;
  }
  return true;
}

void CForteArchitecture::deinitialize(){
  if(mInitialized){
    initialize_destructors(); //TODO: is it really necessary? When called, _fini_array_start_forte == _fini_array_end_forte, so no function is actually called
    mInitialized = false;
  }
}
