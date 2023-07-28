/************************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Milan Vathoopan, Guru Chandrasekhara - initial API and implementation and/or initial documentation
 ************************************************************************************/
#ifndef FREERTOS_FORTEALLOC_H_
#define FREERTOS_FORTEALLOC_H_

//If generic malloc and free are to be used set the cmake Param FORTE_FREERTOS_ALLOC to GENFORTEALLOC
#include <FreeRTOS.h>

#include "datatype.h"
#include <stdlib.h>

inline
void forte_free(void *paData) {
  vPortFree(paData);
}

inline
void *forte_malloc(size_t paSize) {
  return pvPortMalloc(paSize);
}

#endif /* FREERTOS_FORTEALLOC_H_ */
