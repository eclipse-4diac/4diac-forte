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
