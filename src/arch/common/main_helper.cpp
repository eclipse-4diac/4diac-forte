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

#include <signal.h>

#include <functional>

void (*sCallback)();


void callback(int){
  sCallback();
}

void hookEndForte(void (*paEndForte)()){
  sCallback = paEndForte;

  signal(SIGINT, callback);
  signal(SIGTERM, callback);
  signal(SIGHUP, callback);
}
