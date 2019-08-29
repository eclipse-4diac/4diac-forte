/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <stdlib.h>
#include "forte_realFunctions.h"

TForteFloat forte_stringToFloat(const char* paStr, char** paEndPtr) {
#if defined(WIN32) || defined(__ECOS) || defined(VXWORKS)
  return static_cast<TForteFloat>(strtod(paStr, paEndPtr));
#else
  return strtof(paStr, paEndPtr);
#endif
}
