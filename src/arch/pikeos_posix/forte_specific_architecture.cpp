/*******************************************************************************
 * Copyright (c) 2024 Jose Cabral
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "arch/forte_specific_architecture.h"

/* CG: testing */
#include <sys/debug.h>

#ifdef POSIX_LWIP
#include <lwip/sockets.h>
#endif

int CForteSpecificArchitecture::initialize(int , char** ){
#ifdef POSIX_LWIP
  /* Initialize the lwIP stack. */
  if (auto result = _lwip_init(); result != 0) {
    DEVLOG_ERROR("Initialization of lwIP stack failed.\n");
    return result;
  }
#endif // POSIX_LWIP

  return 0;
}

int CForteSpecificArchitecture::deinitialize() {
  return 0;
}
