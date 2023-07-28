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
#ifndef ESP32_SPIRAM_FORTEALLOC_H_
#define ESP32_SPIRAM_FORTEALLOC_H_

#include <stdlib.h>
#include "esp_heap_caps.h"
#include "datatype.h"

inline
void forte_free(void *paData) {
  free(paData);

}

inline
void *forte_malloc(size_t paSize) {
  return heap_caps_malloc(paSize, MALLOC_CAP_SPIRAM);

}

#endif /* ESP32_SPIRAM_FORTEALLOC_H_ */
