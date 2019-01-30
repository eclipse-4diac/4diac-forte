/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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
