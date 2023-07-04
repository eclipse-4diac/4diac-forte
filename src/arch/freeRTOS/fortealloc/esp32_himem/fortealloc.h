/************************************************************************************
 * Copyright (c) 2022-2023 HR Agrartechnik GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * FRanz Hoepfinger - initial API and implementation and/or initial documentation
 ************************************************************************************/
#ifndef ESP32_HIMEM_FORTEALLOC_H_
#define ESP32_HIMEM_FORTEALLOC_H_

#include "datatype.h"
#include <stdlib.h>

#error FORTE_ALLOC_HIMEM not available yet

inline
void forte_free(void *paData) {

}

inline
void *forte_malloc(size_t paSize) {
  return nullptr;
}

#endif /* ESP32_HIMEM_FORTEALLOC_H_ */
