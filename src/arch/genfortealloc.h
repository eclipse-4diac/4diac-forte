/*******************************************************************************
 * Copyright (c) 2010 - 2013 ACIN, nxtcontrol GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Stanislav Meduna, Monika Wenger
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef GENFORTEALLOC_H_
#define GENFORTEALLOC_H_

#include <stdlib.h>


inline
void forte_free(void *pa_pvData){
  free(pa_pvData);
}

inline
void *forte_malloc(size_t pa_nSize){
  return malloc(pa_nSize);
}

#endif /* GENFORTEALLOC_H_ */
