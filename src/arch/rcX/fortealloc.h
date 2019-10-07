/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef FORTEALLOC_H_
#define FORTEALLOC_H_

#include <rX_Includes.h>

inline
void forte_free(void *paData){
  rX_MemFreeMemory(paData);
}

inline
void *forte_malloc(size_t paSize){
  void *memory;
  rX_MemAllocateMemory(&memory, paSize);
  //TODO add error handling for the return value
  return memory;
}


#endif /* FORTEALLOC_H_ */
