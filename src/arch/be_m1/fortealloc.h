/*******************************************************************************
 * Copyright (c) 2012, 2013 ACIN and fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef FORTEALLOC_H_
#define FORTEALLOC_H_

#include "datatype.h"
#include <mtypes.h>
#include <taskLib.h>
#include <msys_e.h>
#include <log_e.h>
#include <new>

//--- SW-Module memory partition, needed for new operator
extern UINT32 FORTE_MemPart;

inline
void forte_free(void *pa_pvData){
  sys_MemXFree(pa_pvData);
}

inline
void *forte_malloc(size_t pa_nSize){
  void * pTemp;

    pTemp = sys_MemXPAlloc(FORTE_MemPart, pa_nSize);

    if (pTemp == NULL)
    {
      /* no memory, suspend task to avoid a NULL exception*/
      log_Err(
          "forte_malloc: unable to allocate 0x%x bytes in partition %d, Suspend task!",
          pa_nSize, FORTE_MemPart);
      taskSuspend(taskIdSelf());
    }

    return pTemp;
}

inline
void *forte_realloc(void *pa_pvData, size_t pa_nSize){
  //FIXME add realloc implementation for BE_M1 platform
  return realloc(pa_pvData, pa_nSize);
}


inline
void* operator new(size_t pa_nSize) throw(std::bad_alloc) {
	return forte_malloc(pa_nSize);
}

inline
void* operator new[](size_t pa_nSize) throw(std::bad_alloc) {
  return forte_malloc(pa_nSize);
}

inline
void operator delete(void* pa_pvData) throw() {
	sys_MemXFree(pa_pvData);
}

inline
void operator delete[](void* pa_pvData) throw() {
	sys_MemXFree(pa_pvData);
}

/*! \brief Placement new operator
 *
 * Will use the given buffer as memory region. The need size will be ignored.
 * Objects created with this new must not deleted only invoke the destructor
 *
 * @param pa_pData
 */
inline
void * operator new(size_t, TForteByte *pa_pData) throw() {
	return pa_pData;
}

inline
void * operator new[](size_t, TForteByte *pa_pData) throw() {
	return pa_pData;
}

inline
void operator delete(void *, TForteByte *) throw() {
}

inline
void operator delete[](void *, TForteByte *) throw() {
}

#endif /* FORTEALLOC_H_ */
