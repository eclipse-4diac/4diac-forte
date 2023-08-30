/*******************************************************************************
 * Copyright (c) 2012, 2013 ACIN and fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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

//--- SW-Module memory partition, needed for new operator
extern UINT32 FORTE_MemPart;

inline
void forte_free(void *paData){
  sys_MemXFree(paData);
}

inline
void *forte_malloc(size_t paSize){
  void * pTemp;

    pTemp = sys_MemXPAlloc(FORTE_MemPart, paSize);

    if (pTemp == nullptr)
    {
      /* no memory, suspend task to avoid a nullptr exception*/
      log_Err(
          "forte_malloc: unable to allocate 0x%x bytes in partition %d, Suspend task!",
          paSize, FORTE_MemPart);
      taskSuspend(taskIdSelf());
    }

    return pTemp;
}


#endif /* FORTEALLOC_H_ */
