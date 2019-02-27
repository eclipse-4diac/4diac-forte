/************************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 * Milan Vathoopan, Guru Chandrasekhara - initial API and implementation and/or initial documentation
 ************************************************************************************/
#ifndef FORTEALLOC_H_
#define FORTEALLOC_H_

//If generic malloc and free are to be used, uncomment the next line, and comment the following
//#include "../genfortealloc.h"
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

#endif /* FORTEALLOC_H_ */
