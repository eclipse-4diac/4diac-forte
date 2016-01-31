/*******************************************************************************
 * Copyright (c) 2010 - 2013 ACIN, nxtcontrol GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Stanislav Meduna, Monika Wenger
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef GENFORTEALLOC_H_
#define GENFORTEALLOC_H_

#include "datatype.h"
#include <stdlib.h>
#include <new>


inline
void forte_free(void *pa_pvData){
  free(pa_pvData);
}

inline
void *forte_malloc(size_t pa_nSize){
  return malloc(pa_nSize);
}

inline
void *forte_realloc(void *pa_pvData, size_t pa_nSize){
  return realloc(pa_pvData, pa_nSize);
}

/*! \file genfortealloc.h
 * Collection of new and delete operators used by forte for dynamically allocating memory.
 *
 * The generic implementation uses malloc and free in order to not introduce any std c++ overhead
 * other implementations are also fine.
 *
 * TODO try std new with nothrow.
 */

inline
void* operator new(size_t pa_nSize) throw (std::bad_alloc){
  return malloc(pa_nSize);
}

inline
void* operator new[](size_t pa_nSize) throw (std::bad_alloc){
  return malloc(pa_nSize ? pa_nSize : 1);
}

inline
void operator delete(void* pa_pvData) throw(){
  if (pa_pvData)
    free(pa_pvData);
}

inline
void operator delete[](void* pa_pvData) throw(){
  if (pa_pvData)
    free(pa_pvData);
}

/*! \brief Placement new operator
 *
 * Will use the given buffer as memory region. The need size will be ignored.
 * Objects created with this new must not deleted only invoke the destructor
 *
 * @param pa_pData
 */
inline
void * operator new(size_t, TForteByte *pa_pData){
  return pa_pData;
}

inline
void * operator new[](size_t, TForteByte *pa_pData){
  return pa_pData;
}

inline
void operator delete(void *, TForteByte *){
}

inline
void operator delete[](void *, TForteByte *){
}

#endif /* GENFORTEALLOC_H_ */
