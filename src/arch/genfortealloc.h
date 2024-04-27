/*******************************************************************************
 * Copyright (c) 2010 - 2013 ACIN, nxtcontrol GmbH, fortiss GmbH
 *               2024 Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Stanislav Meduna, Monika Wenger
 *    - initial API and implementation and/or initial documentation
 *   Ketut Kumajaya - check the size argument does not exceed PTRDIFF_MAX
 *******************************************************************************/
#ifndef GENFORTEALLOC_H_
#define GENFORTEALLOC_H_

#include <stdlib.h>
#include <cstdint>


inline
void forte_free(void *paData){
  free(paData);
}

inline
void *forte_malloc(size_t paSize){
#if defined(__GNUC__) && !defined(__clang__)
  // BUG RESOLVED WONTFIX: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=85783
  if (paSize <= PTRDIFF_MAX) {
    return malloc(paSize);
  }
  return nullptr; // should never happen
#else
  return malloc(paSize);
#endif
}

#endif /* GENFORTEALLOC_H_ */
