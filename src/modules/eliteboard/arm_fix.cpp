/*******************************************************************************
 * Copyright (c) 2021, 2022 Jonathan Lainer (kontakt@lainer.co.at)
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *******************************************************************************/

#include <devlog.h>

extern "C" void __cxa_pure_virtual() {}

extern "C" int atexit(void (*func)()) {
  (void)func;
  return 0;
}

void logMessage(E_MsgLevel, const char *, ...) {}
