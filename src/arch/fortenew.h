/*******************************************************************************
 * Copyright (c) 2010 - 2016 ACIN, nxtcontrol GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Stanislav Meduna, Monika Wenger
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_ARCH_FORTENEW_H_
#define SRC_ARCH_FORTENEW_H_

#include "datatype.h"
#include <fortealloc.h>
#include <new>


#ifndef FORTE_USE_DEFAULT_NEW_AND_DELETE
//With this define platforms can use the default new and delete operators provided by the standard library
//It should be set in the fortealloc.h file

/*!Collection of new and delete operators used by forte for dynamically allocating memory.
 *
 * The generic implementation uses malloc and free in order to not introduce any std c++ overhead
 * other implementations are also fine.
 *
 * TODO try std new with nothrow.
 */

#if __cplusplus >= 201103L //stdc11
inline
void* operator new(size_t paSize) {
  return forte_malloc(paSize);
}

inline
void* operator new[](size_t paSize) {
  return forte_malloc(paSize ? paSize : 1);
}

inline
void operator delete(void* paData) noexcept{
  if (paData) {
    forte_free(paData);
  }
}

inline
void operator delete[](void* paData) noexcept{
  if (paData) {
    forte_free(paData);
  }
}

# if __cplusplus >= 201402L //stdc14
inline
void operator delete(void* paData, std::size_t) noexcept{
  if (paData){
    forte_free(paData);
  }
}


inline
void operator delete[](void* paData, std::size_t) noexcept{
  if (paData) {
    forte_free(paData);
  }
}
# endif // __cplusplus >= 201402L //stdc14


#else  //__cplusplus >= 201103L //stdc11

inline
void* operator new(size_t paSize) throw (std::bad_alloc){
  return forte_malloc(paSize);
}

inline
void* operator new[](size_t paSize) throw (std::bad_alloc){
  return forte_malloc(paSize ? paSize : 1);
}


inline
void operator delete(void* paData) throw(){
  if (pa_pvData) {
    forte_free(paData);
  }
}

inline
void operator delete[](void* paData) throw(){
  if (pa_pvData) {
    forte_free(paData);
  }
}


#endif


/*! \brief Placement new operator
 *
 * Will use the given buffer as memory region. The need size will be ignored.
 * Objects created with this new must not deleted only invoke the destructor
 *
 * @param pa_pData
 */
inline
void * operator new(size_t, TForteByte *paData){
  return paData;
}

inline
void * operator new[](size_t, TForteByte *paData){
  return paData;
}

inline
void operator delete(void *, TForteByte *){
}

inline
void operator delete[](void *, TForteByte *){
}

#endif //FORTE_USE_DEFAULT_NEW_AND_DELETE

#endif /* SRC_ARCH_FORTENEW_H_ */
