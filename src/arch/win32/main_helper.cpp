/*******************************************************************************
 * Copyright (c) 2024
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

// same as the one in arch/common, but without a SIGHUP which is not there in windows

#include <signal.h>

void (*sCallback)();

void callback(int){
  sCallback();
}

void hookEndForte(void (*paEndForte)()){
  sCallback = paEndForte;

  signal(SIGINT, callback);
  signal(SIGTERM, callback);
}
